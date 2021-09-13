#ifndef LISTENER_SENDER_H
#define LISTENER_SENDER_H

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int delayForSender = 1;

typedef struct arguments {
    int fd;
    char *message;
    struct sockaddr_in addr;
} arguments;

void *startSender(void *args);

#endif //LISTENER_SENDER_H
