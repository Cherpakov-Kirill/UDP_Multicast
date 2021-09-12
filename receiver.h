#ifndef LISTENER_RECEIVER_H
#define LISTENER_RECEIVER_H

#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>

#define MSGBUFSIZE 50

int startReceiver(struct sockaddr_in addr, int fd);

#endif //LISTENER_RECEIVER_H
