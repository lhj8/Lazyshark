#ifndef LAZYSHARK_PACKET_H
#define LAZYSHARK_PACKET_H

#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <cstring>
#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <unordered_map>
#include <functional>
#include <tuple>
#include "common.h"


using namespace std;

class CustomPacket {
private:
    unsigned int no_;
    string time_;
    unsigned int len_;
    string protocol_;
    string source_;
    string dest_;
    uint16_t sport_;
    uint16_t dport_;
    string data_;
    string warning_;
    int priority_;

    template <typename T>
    void allocateAndCopy(T** dest, const T* src, size_t size) {
        if (*dest) {
            delete *dest;
        }
        *dest = new T;
        memcpy(*dest, src, size);
    }
public:
    void setNo(unsigned int packetCount);
    void setTime(const string &time);
    void setLen(unsigned int plen);
    void setWarning(const string &logTime, const unordered_map<string, tuple<int, string>> &logMap);
    void setData(const string &data);
    void savePayload(const u_char *payload, size_t len);
    void print_hex_ascii_line (const u_char *payload, int len, int offset, ostringstream& out);

    void processIP(const void* hdr, const string &type);
    void processTCP(const struct tcphdr* hdr);
    void processUDP(const struct udphdr* hdr);
    void processICMP(const struct icmphdr *hdr);
    void processARP(const struct ether_header *hdr);
    void processICMPV6();


    unsigned int getNo() const;
    unsigned int getLen() const;
    string getTime() const;
    string getProtocol() const;
    string getSIP() const;
    string getDIP() const;
    uint16_t getSPort() const;
    uint16_t getDPort() const;
    string getWarning() const;
    string getData() const;
    int getPriority() const;




    ~CustomPacket();
};

#endif //LAZYSHARK_PACKET_H
