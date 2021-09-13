#include "sender.h"

void *startSender(void *args) {
    auto *arg = (arguments *) args;
    //printf("Thread with sender is started\n");
    while (1) {
        int nbytes = sendto(arg->fd, arg->message, strlen(arg->message), 0, (struct sockaddr *) &(arg->addr),
                            sizeof(arg->addr));
        if (nbytes < 0) {
            perror("sendto");
            return nullptr;
        }
        sleep(delayForSender);
    }
    return nullptr;
}
