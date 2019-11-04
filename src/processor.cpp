#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

// DID: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> cpu_util = LinuxParser::CpuUtilization();

    // V1: accept low non-accurate htop

    /*
    user = std::stoi(cpu_util[0]);
    nice = std::stoi(cpu_util[0]);
    system = std::stoi(cpu_util[0]);
    idle = std::stoi(cpu_util[0]);
    iowait = std::stoi(cpu_util[0]);
    irq = std::stoi(cpu_util[0]);
    softirq = std::stoi(cpu_util[0]);
    steal = std::stoi(cpu_util[0]);
    guest = std::stoi(cpu_util[0]);
    guest_nice = std::stoi(cpu_util[0]);

    // formula from https://github.com/Leo-G/DevopsWiki/wiki/How-Linux-CPU-Usage-Time-and-Percentage-is-calculated
    int cpu_time_since_boot = user + nice + system + idle + iowait + irq + softirq + steal;
    int cpu_idle_since_boot = idle + iowait;
    int cpu_usage_since_boot = cpu_time_since_boot - cpu_idle_since_boot;
    float cpu_percentage = cpu_usage_since_boot / (100.f * cpu_time_since_boot);
    */



    // V2: Use the time delta to calculate difference in time since readings
    // and the usage and idle deltas, which we can then use to calculate the pct-change
    int old_user = this->user;
    int old_nice = this->nice;
    int old_system = this->system;
    int old_idle = this->idle;
    int old_iowait = this->iowait;
    int old_irq = this->irq;
    int old_softirq = this->softirq;
    int old_steal = this->steal;


    int old_cpu_time_since_boot = old_user + old_nice + old_system + old_idle + old_iowait +
            old_irq + old_softirq + old_steal;
    int old_cpu_idle_since_boot = old_idle + old_iowait;
    int old_cpu_usage_since_boot = old_cpu_time_since_boot - old_cpu_idle_since_boot;

    user = std::stoi(cpu_util[0]);
    nice = std::stoi(cpu_util[0]);
    system = std::stoi(cpu_util[0]);
    idle = std::stoi(cpu_util[0]);
    iowait = std::stoi(cpu_util[0]);
    irq = std::stoi(cpu_util[0]);
    softirq = std::stoi(cpu_util[0]);
    steal = std::stoi(cpu_util[0]);

    int cpu_time_since_boot = user + nice + system + idle + iowait + irq + softirq + steal;
    int cpu_idle_since_boot = idle + iowait;
    int cpu_usage_since_boot = cpu_time_since_boot - cpu_idle_since_boot;


    int time_delta = cpu_time_since_boot - old_cpu_time_since_boot; // for normalization
    float usage_delta = cpu_usage_since_boot - old_cpu_usage_since_boot;

    return usage_delta / (1.0 * time_delta);

};