#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <unistd.h>

using namespace std;

#define MSGBUFSIZE 256

typedef struct someArgs_tag {
    int fd;
    int delay_secs;
    const char *msg;
    struct sockaddr_in addr;
    int out;
} someArgs_t;

void *func(void *args) {
    someArgs_t *arg = (someArgs_t *) args;
    //printf("%d %s\n", arg->delay_secs, arg->msg);
    printf("thread is started\n");
    while (1) {
        int nbytes = sendto(arg->fd, arg->msg, strlen(arg->msg), 0, (struct sockaddr *) &(arg->addr),
                            sizeof(arg->addr));
        if (nbytes < 0) {
            perror("sendto");
            return NULL;
        }
        sleep(arg->delay_secs); // Unix sleep is seconds
    }

    return NULL;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Command line args should be multicast group and port\n");
        printf("(e.g. `224.0.0.0 1900`)\n");
        return 1;
    }

    char *group = argv[1];
    int port = atoi(argv[2]);

    const int delay_secs = 1;
    const char *message = "Hello, World!";

    // create what looks like an ordinary UDP socket
    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    // allow multiple sockets to use the same PORT number
    u_int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &yes, sizeof(yes)) < 0) {
        perror("Reusing ADDR failed");
        return 1;
    }

    // set up destination address
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
    addr.sin_port = htons(port);

    // bind to receive address
    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    // use setsockopt() to request that the kernel join a multicast group
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        return 1;
    }

    pthread_t pthread;
    someArgs_t args;
    args.addr = addr;
    args.fd = fd;
    args.delay_secs = delay_secs;
    args.msg = message;


    int error;

    if ((error = pthread_create(&pthread, NULL, func, (void *) &args)) != 0) {
        fprintf(stderr, "pthread_create() failed! : %s", strerror(error));
        return 0;
    }

    /*int status;
    int status_addr;
    if ((status = pthread_join(pthread, (void **) &status_addr)) != 0) {
        printf("main error: can't join thread, status = %d\n", status);
        return 0;
    }
    printf("joined with address %d\n", status_addr);
    printf("thread arg.out = %d\n", args.out);*/

    // now just enter a read-print loop
    while (1) {
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
    return 0;
}