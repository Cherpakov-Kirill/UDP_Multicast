#ifndef LISTENER_UTILS_H
#define LISTENER_UTILS_H
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
int getIpType(char* ip);
string makeMessage(int ipType);
#endif //LISTENER_UTILS_H
