#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cassert>
#include "linux_parser.h"
#include <iostream>
#include <unistd.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

#define MEMORY_UTIL_NUM_LINES 2 // only the first 4 lines are used to calculate memory

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
// NOTE: I modified the Kernel function to actually get the kernel number. On the website it's
// Kernel: Version
// which is ... pointless because that doesn't tell me what kernel version it is LOL
string LinuxParser::Kernel() {
  string os, kernel, version_word; // LMAO
  // but the current implemen
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version_word >> kernel;
  }
  return kernel;
}

// DONE: Read and return the system memory utilization
// ASSUMPTION: only deals with kB. If they were different states, define an
// enum and combine that with a switch-case to modify the values to standardize
float LinuxParser::MemoryUtilization() {
    string line;
    std::vector<long> memory(MEMORY_UTIL_NUM_LINES); // Pre-allocate space for 4
    // MemTotal, MemFree, MemAvailable, Buffers
    // extract the information
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()){
        for (int i = 0; i < MEMORY_UTIL_NUM_LINES; ++i){
            string name;
            int mem;
            std::getline(filestream, line);
            std::istringstream linestream(line);
            linestream >> name >> mem;
            memory[i] = mem;
        }
    }

    float usedMemory = memory[0] - memory[1];
    return usedMemory / memory[0];
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
    string line;
    long uptime{-1};
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
    if (filestream.is_open()){
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> uptime ;
        return uptime;
    }
    return uptime;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    string line;
    vector<string> to_return;
    int skip_first_col = 0; // First col is CPU, so skip and don't insert it in.
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()){
        std::getline(filestream, line);
        std::istringstream linestream(line);
        string datum;
        while (linestream >> datum){
            if (skip_first_col != 0){
                to_return.push_back(datum);
            }
            skip_first_col++;

        }
        return to_return;
    }
    return to_return;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    const std::string title = "processes";
    return LinuxParser::FindLineWithTitle(title, kStatFilename);
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    const std::string title = "procs_running";
    return LinuxParser::FindLineWithTitle(title, kStatFilename);
}

int LinuxParser::FindLineWithTitle(const string title, const string f_name){
    string line;
    string key;
    int value{-1};
    std::ifstream filestream(kProcDirectory + f_name);

    if (filestream.is_open()){

        while (std::getline(filestream, line)){
//            if (title == "Uid:"){
//                std::cout << "Filestream open for " << title << "\t" << f_name << "\t" << line << "\n";
//            }
            std::istringstream linestream(line);
            while (linestream >> key >> value){
                if (key == title){
                    return value;
                }
            }
        }
    }
    return value;
}


/*
 * Process stuff
 */

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
    vector<int> pids;
    DIR* directory = opendir(kProcDirectory.c_str());
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr) {
        // Is this a directory?
        if (file->d_type == DT_DIR) {
            // Is every character of the name a digit?
            string filename(file->d_name);
            if (std::all_of(filename.begin(), filename.end(), isdigit)) {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string line;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()){
        std::getline(filestream, line);
        return line;
    }
    return "DEBUG";
}


// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string line;
    int title = LinuxParser::FindLineWithTitle("Uid:", std::to_string(pid) + kStatusFilename);
    return std::to_string(title);
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid){
    string line;
    string name{"Ded"};
    string uid_str;
    const string ref_uid = LinuxParser::Uid(pid);
    string x; // literally match the x bc c++ doesn't have _
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()){
        while (std::getline(filestream, line)){
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> name >> x >> uid_str;
            if (uid_str == ref_uid){
                return name;
            }
        }
    }
    return name;
}


// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    int vmsize = LinuxParser::FindLineWithTitle("vMSize:",  std::to_string(pid) + kStatusFilename);
    return to_string(vmsize / 1000.f);
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
    string line, ph;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    int cnt = 0;
    if (stream.is_open()) {
        getline(stream, line);
        std::istringstream linestream(line);
        while(linestream >> ph){
            cnt += 1;
            if (cnt == 22){
                return std::stol(ph) / sysconf(_SC_CLK_TCK);
            }
        }
    }
    return -1;
}

float LinuxParser::ProcessCPUUtil(int pid){
    int total_time;
    float seconds;
    string line, tmp_var;
    vector <string> holder; // create temporary holder to store all the garbage we need

    // Read the file for everything
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        getline(stream, line);
        std::istringstream linestream(line);
        while(linestream >> tmp_var){
            holder.emplace_back(tmp_var);
        }
    }

    // Use the formula found on https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599

    total_time = std::stoi(holder[13]) + std::stoi(holder[14]) + std::stoi(holder[15]) + std::stoi(holder[16]);  // Get the total time

    seconds = LinuxParser::UpTime() - (std::stof(holder[21]) / sysconf(_SC_CLK_TCK));
    return 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
}