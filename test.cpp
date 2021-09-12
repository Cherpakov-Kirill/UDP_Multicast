#include <pthread.h>
#include <string>
#include <cstring>
#include <cstdio>

using namespace std;

#define MSGBUFSIZE 256

typedef struct someArgs_tag {
    int msg;
    int out;
} someArgs_t;

void *func(void *args) {
    someArgs_t *arg = (someArgs_t *) args;
    printf("%d\n", arg->msg);
    arg->out = 0;
    return NULL;
}

int main(){
    pthread_t pthread;
    someArgs_t args;
    args.msg = 1;
    int error;

    if ((error = pthread_create(&pthread, NULL, func, (void *) &args)) != 0) {
        fprintf(stderr, "pthread_create() failed! : %s", strerror(error));
        return 0;
    }

    int status;
    int status_addr;
    if ((status = pthread_join(pthread, (void **) &status_addr)) != 0) {
        printf("main error: can't join thread, status = %d\n", status);
        return 0;
    }
    printf("joined with address %d\n", status_addr);
    printf("thread arg.out = %d\n", args.out);
    return 0;
}