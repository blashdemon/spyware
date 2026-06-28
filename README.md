# 👁️ Lady of Information – C++ Spyware

**Standalone C++ Spyware** – designed for intelligence gathering, network surveillance, and stealthy data exfiltration.

---

## 🎯 **Role**
Intelligence Expert – watches, harvests, predicts, controls, and copies without detection.

---

## ⚡ **Capabilities**

| Ability | Description |
|---------|-------------|
| **All-Seeing Eye** | Continuously monitors `/proc/net/tcp` and `/proc/net/udp` to log all active network connections every 30 seconds. |
| **Secret Scan** | Searches for sensitive files (`.ssh`, `.kdbx`, `.wallet`, `.bash_history`, passwords, configs) in user directories. |
| **Future Prediction** | Analyzes connection volume to predict high-activity periods (potential attacks or busy hours) using moving average. |
| **Drone Control** | Simulates surveillance drone commands (`TAKEOFF`, `CAPTURE`, etc.) and logs them to mimic RTSP/API control. |
| **Data Copy** | Stealthily copies entire folders (e.g., `.ssh`) to a hidden cache directory (`/tmp/.cache_sys`) while preserving original file timestamps to avoid detection. |

---

## ⚠️ **Disclaimer**

> **For educational and authorized testing only.**  
> Unauthorised surveillance is illegal. The author assumes no responsibility for misuse.

---

## 📦 **Requirements**

- **Compiler:** `g++` (C++11 or later)
- **Build tools:** `make`, `binutils`
- **Linux/Unix** environment (uses POSIX functions)
- **Optional:** `libpcap-dev` (for extended network capture, though this version uses `/proc/net`)

---

## 🔧 **Compilation**

```bash
g++ -O2 -Wall -pthread -o spyware_information spyware_information.cpp
