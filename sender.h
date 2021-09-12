#ifndef LISTENER_SENDER_H
#define LISTENER_SENDER_H

#include <netinet/in.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>

typedef struct arguments {
    int fd;
    int delay_secs;
    char *message;
    struct sockaddr_in addr;
} arguments;

void *startSender(void *args);

#endif //LISTENER_SENDER_H
