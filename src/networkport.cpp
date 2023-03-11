#include "networkport.h"
#include "debug.h"
#include <QDebug>
#include <chrono>
#include <iostream>
#include <map>
#include <thread>

NetworkPort::NetworkPort() : is_getting_speed(false) { fill_portvec(); }

std::string NetworkPort::get_common(const std::vector<std::string> &strs) {
  // 返回数组内最长公共子字符串
  std::string common = "";
  if (!strs.empty()) {
    common = strs[0];
  }

  for (int i = 1; i < strs.size(); i++) {
    int j = 0;
    for (; j < common.size() && j < strs[i].size(); j++) {
      if (common[j] != strs[i][j]) {
        break;
      }
    }
    common = common.substr(0, j);
  }
  // printz(common);
  return common;
}

void NetworkPort::fill_portvec() {
  std::unordered_map<int, std::vector<std::string>> net_map;
  DWORD size = 0;
  GetIfTable(nullptr, &size, 0);
  MIB_IFTABLE *pTable = (MIB_IFTABLE *)malloc(size);
  GetIfTable(pTable, &size, 0);

  // 根据接收量，相同的归到map下同一映射
  for (DWORD i = 0; i < pTable->dwNumEntries; i++) {
    int received = pTable->table[i].dwInOctets;
    int sent = pTable->table[i].dwOutOctets;
    std::string name(reinterpret_cast<char *>(pTable->table[i].bDescr),
                     pTable->table[i].dwDescrLen);
    if (name[0] < 0) {
      continue;
    }
    if (received != 0) {
      if (net_map.find(received) == net_map.end()) {
        net_map.emplace(received, std::vector<std::string>{std::move(name)});
        portnum_in_table.push_back(i);
      } else {
        net_map[received].push_back(std::move(name));
      }
    }
  }
  free(pTable);
  // 取map中每个项的公共子字符串为网口名
  for (auto iter = net_map.begin(); iter != net_map.end(); ++iter) {
    port_name.push_back(std::move(get_common(iter->second)));
    sent.push_back(0);
    received.push_back(0);
    sent_speed_vec.push_back(0);
    received_speed_vec.push_back(0);
  }
  sent_speed_vec.push_back(0);
  received_speed_vec.push_back(0);
}

void NetworkPort::get_speed() {
  PMIB_IFTABLE pIfTable;
  DWORD dwSize = 0;
  DWORD dwRetVal = 0;
  // 记录上一次读取的网络接口信息
  std::map<DWORD, ULONGLONG> lastBytesSent;
  std::map<DWORD, ULONGLONG> lastBytesReceived;
  pIfTable = (PMIB_IFTABLE)malloc(dwSize);
  // 每秒读取网络接口信息
  while (is_getting_speed) {
    dwRetVal = GetIfTable(pIfTable, &dwSize, 0);
    if (dwRetVal == ERROR_INSUFFICIENT_BUFFER) {
      pIfTable = (PMIB_IFTABLE)realloc(pIfTable, dwSize);
      dwRetVal = GetIfTable(pIfTable, &dwSize, 0);
    }
    if (dwRetVal == NO_ERROR) {
      // std::cout << port_name[1] << std::endl;
      // printz(port_name);
      std::vector<std::string> names;
      for (int i = 0; i < (int)pIfTable->dwNumEntries; i++) {
        MIB_IFROW ifRow = pIfTable->table[i];
        std::string name(reinterpret_cast<char *>(ifRow.bDescr),
                         ifRow.dwDescrLen);
        // printz(name);
        if (!simplify_str(name)) {
          continue;
        }
        // printz(name);
        if (std::find(names.begin(), names.end(), name) == names.end()) {
          names.push_back(name);
          double x = ifRow.dwOutOctets;
          double x1 = lastBytesSent[ifRow.dwIndex];
          double sent_1s =
              double(ifRow.dwOutOctets - lastBytesSent[ifRow.dwIndex]) / 1024;
          double received_1s =
              double(ifRow.dwInOctets - lastBytesReceived[ifRow.dwIndex]) /
              1024;
          int index = std::find(port_name.begin(), port_name.end(), name) -
                      port_name.begin();
          sent_speed_vec[index] = sent_1s;
          received_speed_vec[index] = received_1s;
          if (sent_1s < 10000) {
            sent[index] += sent_1s;
          }
          if (received_1s < 10000) {
            received[index] += received_1s;
          }

          // 更新记录的网络接口信息
          lastBytesSent[ifRow.dwIndex] = ifRow.dwOutOctets;
          lastBytesReceived[ifRow.dwIndex] = ifRow.dwInOctets;
        }
      }
    } else {
      std::cout << "GetIfTable failed with error code: " << dwRetVal
                << std::endl;
    }
    MessageChanged();
    // 等待一秒
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  free(pIfTable);
  std::cout << "结束监测速度" << std::endl;
}

void NetworkPort::set_getting_speed() {
  is_getting_speed = true;
  std::thread t(&NetworkPort::get_speed, this);
  t.detach();
}
void NetworkPort::close_getting_speed() { is_getting_speed = false; }

bool NetworkPort::simplify_str(std::string &str) {
  for (const auto &name : port_name) {
    if (str.find(name) != std::string::npos) {
      str = name;
      return true;
    }
  }
  return false;
}

void NetworkPort::setMessageChanged(std::function<void()> callback) {
  MessageChanged = callback;
}
void NetworkPort::get_adapterInfo(PortDetails *port_details,
                                  const std::string &_port_name) {
  PIP_ADAPTER_INFO pAdapterInfo;
  PIP_ADAPTER_INFO pAdapter = NULL;
  DWORD dwRetVal = 0;
  UINT i;

  ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
  pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
  if (pAdapterInfo == NULL) {
    printf("Error allocating memory needed to call GetAdaptersinfo\n");
    return;
  }
  if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
    FREE(pAdapterInfo);
    pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
    if (pAdapterInfo == NULL) {
      printf("Error allocating memory needed to call GetAdaptersinfo\n");
      return;
    }
  }
  if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
    pAdapter = pAdapterInfo;
    while (pAdapter) {
      if (_port_name != std::string(pAdapter->Description)) {
        pAdapter = pAdapter->Next;
        continue;
      }
      port_details->adapter_name = pAdapter->AdapterName;
      port_details->adapter_description = pAdapter->Description;
      switch (pAdapter->Type) {
      case MIB_IF_TYPE_OTHER:
        port_details->adapter_type = "Other";
        break;
      case MIB_IF_TYPE_ETHERNET:
        port_details->adapter_type = "Ethernet";
        break;
      case MIB_IF_TYPE_TOKENRING:
        port_details->adapter_type = "Token Ring";
        break;
      case MIB_IF_TYPE_FDDI:
        port_details->adapter_type = "FDDI";
        break;
      case MIB_IF_TYPE_PPP:
        port_details->adapter_type = "PPP";
        break;
      case MIB_IF_TYPE_LOOPBACK:
        port_details->adapter_type = "Lookback";
        break;
      case MIB_IF_TYPE_SLIP:
        port_details->adapter_type = "Slip";
        break;
      default:
        port_details->adapter_type = "Unknown type";
        break;
      }
      port_details->ip_address = pAdapter->IpAddressList.IpAddress.String;
      port_details->subnet_mask = pAdapter->IpAddressList.IpMask.String;
      port_details->default_gateway = pAdapter->GatewayList.IpAddress.String;
      // pAdapter = pAdapter->Next;
      break;
    }
  } else {
    printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
  }
  if (pAdapterInfo)
    FREE(pAdapterInfo);
}
NetworkPort::PortDetails *
NetworkPort::get_port_details(int port_in_table_index) {
  PortDetails *port_details = new PortDetails;
  PMIB_IFTABLE pIfTable;
  DWORD dwSize = 0;
  DWORD dwRetVal = 0;
  pIfTable = (PMIB_IFTABLE)malloc(dwSize);
  // 每秒读取网络接口信息
  dwRetVal = GetIfTable(pIfTable, &dwSize, 0);
  if (dwRetVal == ERROR_INSUFFICIENT_BUFFER) {
    pIfTable = (PMIB_IFTABLE)realloc(pIfTable, dwSize);
    dwRetVal = GetIfTable(pIfTable, &dwSize, 0);
  }
  if (dwRetVal == NO_ERROR) {
    for (int i = 0; i < (int)pIfTable->dwNumEntries; i++) {
      if (i != port_in_table_index) {
        continue;
      }
      MIB_IFROW &ifRow = pIfTable->table[i];
      // port_details->adapter_name = reinterpret_cast<const char
      // *>(ifRow.wszName); port_details->adapter_desc = reinterpret_cast<const
      // char *>(ifRow.bDescr);
      port_details->adapter_speed = ifRow.dwSpeed;
      char macAddress[18];
      snprintf(macAddress, sizeof(macAddress), "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
               ifRow.bPhysAddr[0], ifRow.bPhysAddr[1], ifRow.bPhysAddr[2],
               ifRow.bPhysAddr[3], ifRow.bPhysAddr[4], ifRow.bPhysAddr[5]);
      port_details->mac_addresse = macAddress;

      break;
    }
  }
  get_adapterInfo(port_details, port_name[port_in_table_index]);
  free(pIfTable);
  return port_details;
}