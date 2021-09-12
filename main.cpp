#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <string>

#include "receiver.h"
#include "sender.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Command line args should be multicast group and port\n");
        printf("(e.g. `224.0.0.0 1900`)\n");
        return 1;
    }

    char *group = argv[1];
    int port = atoi(argv[2]);
    int ipType = getIpType(group);

    const int delay_secs = 1;
    string message = makeMessage(ipType);

    ///create what looks like an ordinary UDP socket
    int fd = socket(ipType==INET_ADDRSTRLEN ? AF_INET : AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    ///allow multiple sockets to use the same PORT number
    u_int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &yes, sizeof(yes)) < 0) {
        perror("Reusing ADDR failed");
        return 1;
    }

    ///set up destination address
    struct sockaddr_in addr{};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = ipType==INET_ADDRSTRLEN ? AF_INET : AF_INET6;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
    addr.sin_port = htons(port);

    ///bind to receive address
    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    ///use setsockopt() to request that the kernel join a multicast group
    struct ip_mreq mreq{};
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        return 1;
    }

    ///make struct arguments for thread with sender
    arguments args;
    args.addr = addr;
    args.fd = fd;
    args.delay_secs = delay_secs;
    args.message = (char*) malloc(sizeof(char)*message.length());
    strcpy(args.message, message.c_str());

    ///start thread with sender
    pthread_t pthread;
    int error;
    if ((error = pthread_create(&pthread, nullptr, startSender, (void *) &args)) != 0) {
        fprintf(stderr, "pthread_create() failed! : %s", strerror(error));
        return 0;
    }

    ///start receiver
    startReceiver(addr, fd);
    return 0;
}