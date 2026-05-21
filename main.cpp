#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <memory>
#include <array>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

struct PingTarget {
    string name;
    string ip;
    double current_ms;
    bool online;
};

struct SpeedtestResult {
    string timestamp;
    double download_mbps;
    double upload_mbps;
    double ping_ms;
};

mutex data_mutex;
vector<PingTarget> targets = {
    {"Cloudflare", "1.1.1.1", 0.0, false},
    {"Google", "8.8.8.8", 0.0, false},
    {"GitHub", "github.com", 0.0, false}
};
vector<SpeedtestResult> speedtest_history;
bool speedtest_running = false;

string exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "";
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void pingLoop() {
    while (true) {
        for (auto& target : targets) {
            string cmd = "ping -c 1 -W 1 " + target.ip;
            string output = exec(cmd.c_str());
            
            double ms = 0.0;
            bool online = false;
            
            size_t pos = output.find("time=");
            if (pos != string::npos) {
                size_t end_pos = output.find(" ms", pos);
                if (end_pos != string::npos) {
                    try {
                        ms = stod(output.substr(pos + 5, end_pos - (pos + 5)));
                        online = true;
                    } catch (...) {}
                }
            }
            
            lock_guard<mutex> lock(data_mutex);
            target.current_ms = ms;
            target.online = online;
        }
        this_thread::sleep_for(chrono::seconds(5));
    }
}

string getCurrentTime() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    char buf[80];
    strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now_time));
    return string(buf);
}

void runSpeedtest() {
    {
        lock_guard<mutex> lock(data_mutex);
        if (speedtest_running) return;
        speedtest_running = true;
    }

    string output = exec("librespeed-cli --telemetry-level disabled --json 2>/dev/null");
    
    lock_guard<mutex> lock(data_mutex);
    try {
        if (!output.empty()) {
            json res_array = json::parse(output);
            
            if (res_array.is_array() && !res_array.empty()) {
                json res = res_array[0];
                SpeedtestResult result;
                result.timestamp = getCurrentTime();
                
                result.download_mbps = res["download"].get<double>();
                result.upload_mbps = res["upload"].get<double>();
                result.ping_ms = res["ping"].get<double>();
                
                speedtest_history.push_back(result);
                if (speedtest_history.size() > 20) speedtest_history.erase(speedtest_history.begin());
            }
        }
    } catch (...) {
        cout << "Speedtest JSON parse failed. Output was: " << output << endl;
    }
    speedtest_running = false;
}

void speedtestLoop() {
    while (true) {
        runSpeedtest();
        this_thread::sleep_for(chrono::hours(6));
    }
}

int main() {
    thread t_ping(pingLoop);
    t_ping.detach();
    thread t_speed(speedtestLoop);
    t_speed.detach();

    httplib::Server svr;

    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        string html = exec("cat index.html");
        res.set_content(html, "text/html");
    });

    svr.Get("/api/status", [](const httplib::Request &, httplib::Response &res) {
        lock_guard<mutex> lock(data_mutex);
        json response;
        
        json ping_arr = json::array();
        for (const auto& t : targets) {
            ping_arr.push_back({{"name", t.name}, {"ip", t.ip}, {"ms", t.current_ms}, {"online", t.online}});
        }
        
        json st_arr = json::array();
        for (const auto& s : speedtest_history) {
            st_arr.push_back({{"time", s.timestamp}, {"down", s.download_mbps}, {"up", s.upload_mbps}, {"ping", s.ping_ms}});
        }

        response["pings"] = ping_arr;
        response["speedtests"] = st_arr;
        response["speedtest_running"] = speedtest_running;
        
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(response.dump(), "application/json");
    });

    svr.Post("/api/force-speedtest", [](const httplib::Request &, httplib::Response &res) {
        thread t(runSpeedtest);
        t.detach();
        res.set_content("{\"status\":\"started\"}", "application/json");
    });

    cout << "Network Monitor listening on http://0.0.0.0:8080" << endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}