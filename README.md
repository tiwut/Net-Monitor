<div align="center">
  
[![License](https://img.shields.io/github/license/tiwut/Net-Monitor?style=flat-square&color=blue)](https://github.com/tiwut/Net-Monitor/blob/main/LICENSE)
[![Vulnerabilities](https://img.shields.io/badge/Vulnerabilities-0-brightgreen?style=flat-square)](#)
[![GitHub stars](https://img.shields.io/github/stars/tiwut/Net-Monitor?style=flat-square&color=gold)](https://github.com/tiwut/Net-Monitor/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/tiwut/Net-Monitor?style=flat-square&color=orange)](https://github.com/tiwut/Net-Monitor/network/members)
[![GitHub last commit](https://img.shields.io/github/last-commit/tiwut/Net-Monitor?style=flat-square)](https://github.com/tiwut/Net-Monitor/commits/main)
  
</div>

# ⚡ Uptime & Auto-Speedtest Monitor 

A self-hosted, ultra-lightweight dashboard that tracks your internet connection stability. Written in C++ and containerized in an Alpine Linux Docker image for zero vulnerabilities and minimal overhead.

<img width="1202" height="628" alt="image" src="https://github.com/user-attachments/assets/7fc43791-fdd0-4a9a-bd50-6dcda53bcc85" />

## 🚀 Features
- **Live Latency Ping:** Pings Cloudflare, Google, and GitHub every 5 seconds.
- **Automated Speedtests:** Runs a full Ookla-based internet speedtest in the background every 6 hours.
- **Historical Charting:** Visualizes your Download and Upload speeds over time using Chart.js.
- **True Multi-threading:** The web UI remains lightning fast while background tests are running.

## 🐳 How to Run

1. Clone or download this repository.
2. Run Docker Compose:

```bash
docker compose up -d --build
```
3. Open your browser and navigate to `http://<your-server-ip>:8080`

## 🛠️ Tech Stack
- **Backend:** C++ (cpp-httplib, nlohmann/json)
- **Frontend:** HTML, Tailwind CSS, Chart.js
- **OS Base:** Alpine Linux (`speedtest-cli`)
