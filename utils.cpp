#include "utils.h"

string getIpAddr(int ipType) {
    struct ifaddrs *ifAddrStruct = nullptr;
    struct ifaddrs *ifa = nullptr;
    void *tmpAddrPtr = nullptr;
    getifaddrs(&ifAddrStruct);
    string ipString;
    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        switch (ipType) {
            case INET_ADDRSTRLEN: {
                if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
                    /// is a valid IP4 Address
                    tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
                    char addressBuffer[INET_ADDRSTRLEN];
                    for (int i = 0; i < INET_ADDRSTRLEN; i++) {
                        addressBuffer[i] = '0';
                    }
                    inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                    if (ifa->ifa_name[0] != 'l' && ifa->ifa_name[1] != 'o') {
                        int i = 0;
                        while (addressBuffer[i] != '\0') {
                            ipString += addressBuffer[i];
                            i++;
                        }
                        //printf("Ip_addr IPv4: %s   pid: %d\n", addressBuffer, getpid());
                    }
                }
                break;
            }
            case INET6_ADDRSTRLEN: {
                if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
                    /// is a valid IP6 Address
                    tmpAddrPtr = &((struct sockaddr_in6 *) ifa->ifa_addr)->sin6_addr;
                    char addressBuffer[INET6_ADDRSTRLEN];
                    inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
                    printf("'%s': %s\n", ifa->ifa_name, addressBuffer);
                    if (ifa->ifa_name[0] != 'l' && ifa->ifa_name[1] != 'o') {
                        int i = 0;
                        while (addressBuffer[i] != '\0') {
                            ipString += addressBuffer[i];
                            i++;
                        }
                        printf("ip_addr IPv6: %s\n", addressBuffer);
                    }
                }
                break;
            }
            default: {
                if (ifAddrStruct != nullptr) freeifaddrs(ifAddrStruct);//remember to free ifAddrStruct
                return "Error with detecting ip";
            }

        }
    }
    if (ifAddrStruct != nullptr) freeifaddrs(ifAddrStruct);//remember to free ifAddrStruct
    return ipString;
}

int getIpType(char *ip) {
    for (int i = 0; i <= 3; i++) {
        if (ip[i] == '.') return INET_ADDRSTRLEN;
    }
    return INET6_ADDRSTRLEN;
}

string makeMessage(int ipType) {
    string message = "ip: ";
    message += getIpAddr(ipType);
    message += "   pid: ";
    message += std::to_string(getpid());
    return message;
}