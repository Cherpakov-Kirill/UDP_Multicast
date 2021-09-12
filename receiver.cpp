#include "receiver.h"

int startReceiver(struct sockaddr_in addr, int fd){
    while(1){
        char msgbuf[MSGBUFSIZE];
        int addrlen = sizeof(addr);
        int nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *) &addr,
                              reinterpret_cast<socklen_t *>(&addrlen));
        if (nbytes < 0) {
            perror("recvfrom");
            return 1;
        }
        msgbuf[nbytes] = '\0';
        puts(msgbuf);
    }
}