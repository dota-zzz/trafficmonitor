#pragma once
#include <functional>
#include <iostream>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <winsock2.h>


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
class NetworkPort {
public:
  NetworkPort();
  ~NetworkPort() { is_getting_speed = false; }
  // 返回速度
  inline double get_sent_speed(int port_num) {
    return sent_speed_vec[port_num];
  }
  inline double get_received_speed(int port_num) {
    return received_speed_vec[port_num];
  }
  inline double get_sent(int port_num) { return sent[port_num]; }
  inline double get_received(int port_num) { return received[port_num]; }
  inline int get_auto_port_num() {
    auto max_it = std::max_element(received.begin(), received.end());
    int max_index = std::distance(received.begin(), max_it);
    return max_index;
  }
  // 返回网口名
  inline std::vector<std::string> get_port_name() { return port_name; }
  inline int get_port_name_size() { return port_name.size(); }
  // 开始和停止检测速度
  void set_getting_speed();
  void close_getting_speed();
  // 设置速度变化回调函数
  void setMessageChanged(std::function<void()> callback);
  struct PortDetails {
    std::string adapter_name;
    std::string adapter_description;
    std::string adapter_type;
    DWORD adapter_speed;
    std::string mac_addresse;
    std::string ip_address;
    std::string subnet_mask;
    std::string default_gateway;
  };

  void get_adapterInfo(PortDetails *port_details,
                       const std::string &_port_name);

  PortDetails *get_port_details(int port_in_table_index);

private:
  // 速度
  std::vector<double> sent_speed_vec;
  std::vector<double> received_speed_vec;
  // 总流量
  std::vector<double> sent;
  std::vector<double> received;
  // 网口名在GetIfTable中的索引
  std::vector<DWORD> portnum_in_table;
  // 是否在检测
  volatile bool is_getting_speed;
  // 网口名
  std::vector<std::string> port_name;
  // 获取最大子字符串
  std::string get_common(const std::vector<std::string> &strs);
  // 获取速度
  void get_speed();
  // 填充网口数组
  void fill_portvec();
  bool simplify_str(std::string &);

  std::function<void()> MessageChanged;
};