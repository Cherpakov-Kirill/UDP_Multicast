#include "sender.h"
#include <string>
#include <arpa/inet.h>

using namespace std;

void *startSender(void *args) {
    auto *arg = (arguments *) args;
    /*const int delay_secs = 1;
    string message = makeMessage(ipType);

    ///create what looks like an ordinary UDP socket
    int fd = socket(ipType==INET_ADDRSTRLEN ? AF_INET : AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0) {
        perror("socket");
        return;
    }

    ///set up destination address
    struct sockaddr_in addr{};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = ipType==INET_ADDRSTRLEN ? AF_INET : AF_INET6;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // differs from sender
    addr.sin_port = htons(port);*/

    (arg->addr).sin_addr.s_addr = inet_addr(arg->group);


    printf("Thread with sender is started\n");
    while (1) {
        int nbytes = sendto(arg->fd, arg->message, strlen(arg->message), 0, (struct sockaddr *) &(arg->addr),
                            sizeof(arg->addr));
        if (nbytes < 0) {
            perror("sendto");
            return nullptr;
        }
        sleep(arg->delay_secs); // Unix sleep is seconds
    }
    return nullptr;
}
