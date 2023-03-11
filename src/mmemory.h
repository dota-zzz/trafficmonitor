#pragma once

#include <pdh.h>
#include <thread>
#include <vector>
#include <windows.h>


static float get_percent_memory() {
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  GlobalMemoryStatusEx(&statex);
  DWORDLONG physical_memory = statex.ullTotalPhys / (1024 * 1024 * 1024);
  DWORDLONG usePhys =
      physical_memory - (statex.ullAvailPhys / (1024 * 1024 * 1024));
  return (float)usePhys / (float)physical_memory;
}
static double get_physical_memory() {
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  GlobalMemoryStatusEx(&statex);
  double physical_memory = (double)statex.ullTotalPhys / (1024 * 1024 * 1024);
  return physical_memory;
}
static double get_virtual_memory() {
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  GlobalMemoryStatusEx(&statex);
  double virtual_memory = (double)statex.ullAvailVirtual / (1024 * 1024 * 1024);
  return virtual_memory;
}

static double get_memory_used() {
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  GlobalMemoryStatusEx(&statex);
  double physical_memory = (double)statex.ullTotalPhys / (1024 * 1024 * 1024);
  double usePhys =
      physical_memory - ((double)statex.ullAvailPhys / (1024 * 1024 * 1024));
  return usePhys;
}

static std::vector<double> get_memory_message() {
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  GlobalMemoryStatusEx(&statex);
  double physical_memory = (double)statex.ullTotalPhys / (1024 * 1024 * 1024);
  double virtual_memory = (double)statex.ullAvailVirtual / (1024 * 1024 * 1024);
  double usePhys =
      physical_memory - ((double)statex.ullAvailPhys / (1024 * 1024 * 1024));
  return std::vector<double>{physical_memory, virtual_memory, usePhys};
}

static double get_CPU_used() {
  PDH_HQUERY cpuQuery;
  PDH_HCOUNTER cpuTotal;
  PdhOpenQuery(NULL, NULL, &cpuQuery);
  PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL,
                &cpuTotal);
  PdhCollectQueryData(cpuQuery);
  Sleep(1000); // wait 1 second to get accurate reading
  PDH_FMT_COUNTERVALUE counterVal;
  PdhCollectQueryData(cpuQuery);
  PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
  PdhCloseQuery(cpuQuery);
  return counterVal.doubleValue;
}

class CPUMemoryTask {
public:
  void set_getting_CPU_memory() {
    is_getting_CPU_memory_ = true;
    std::thread CPU_memory_task([&]() {
      while (is_getting_CPU_memory_) {
        auto memory_message = get_memory_message();
        memory_used = memory_message[2];
        memory_not_used = memory_message[0] - memory_used;
        memory_percentage = memory_used / memory_message[0];
        CPU_used = get_CPU_used();
      }
    });
    CPU_memory_task.detach();
  }
  void stop_getting_CPU_memory() { is_getting_CPU_memory_ = false; }
  inline double tget_CPU_used() { return CPU_used; }
  inline double tget_memory_used() { return memory_used; }
  inline bool is_getting_CPU_memory() { return is_getting_CPU_memory_; }
  inline double tget_memory_not_used() { return memory_not_used; }
  inline double tget_memory_percentage() { return memory_percentage; }

private:
  bool is_getting_CPU_memory_ = false;
  double CPU_used = 0.0;
  double memory_used = 0.0;
  double memory_not_used = 0.0;
  double memory_percentage = 0;
};