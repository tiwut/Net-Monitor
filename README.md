<div align="center">

[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
![Vulnerabilities](https://img.shields.io/badge/Vulnerabilities-0-brightgreen)

</div>

# ⚡ Uptime & Auto-Speedtest Monitor 

A self-hosted, ultra-lightweight dashboard that tracks your internet connection stability. Written in C++ and containerized in an Alpine Linux Docker image for zero vulnerabilities and minimal overhead.

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
