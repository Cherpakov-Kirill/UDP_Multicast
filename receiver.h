#ifndef LISTENER_RECEIVER_H
#define LISTENER_RECEIVER_H

#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>
#include <map>
#include <iostream>

#include "utils.h"

using namespace std;

#define MSGBUFSIZE 50

const int delayForMakingList = 2;
const int timeOfLife = 2;

int startReceiver(struct sockaddr_in addr, int fd);

#endif //LISTENER_RECEIVER_H
