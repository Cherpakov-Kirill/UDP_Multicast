#include "receiver.h"

time_t makeList(map<string, time_t> mp, time_t timeOfLastMakingList, string messageFromMe) {
    time_t currentTime = time(nullptr);
    if (currentTime - timeOfLastMakingList >= delayForMakingList) {
        tm* timeInfo = localtime(&currentTime);
        cout <<"_____________________________________\n";
        cout << "Current time: " << asctime(timeInfo);
        cout << "Alive nodes list:\n";
        int counter = 1;
        auto iter = mp.begin();
        while (iter != mp.end()) {
            auto nextIter = iter;
            nextIter++;
            if (currentTime - iter->second > timeOfLife) {
                mp.erase(mp.find(iter->first));
            } else {
                cout<<counter<<".  ";
                counter++;
                if(iter->first.c_str() == messageFromMe){
                    printf("%s  -- It is this process.\n", iter->first.c_str());
                }
                else printf("%s\n", iter->first.c_str());
            }
            iter = nextIter;
        }
        cout <<"_____________________________________\n\n";
        return time(nullptr);
    }
    return timeOfLastMakingList;
}

int startReceiver(struct sockaddr_in addr, int fd) {
    map<string, time_t> mp;
    time_t timeOfLastMakingList = time(nullptr);
    string messageFromMe = makeMessage(addr.sin_family == AF_INET ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN); ///get message from this process
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
        string message = string(msgbuf);
        mp[message] = time(nullptr);
        timeOfLastMakingList = makeList(mp, timeOfLastMakingList, messageFromMe);
    }
}