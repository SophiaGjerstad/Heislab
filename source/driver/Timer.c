#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <assert.h>
//#include <pthread.h>
#include "Timer.h"

void AlertAfter3Seconds(void) {
    double start = time(NULL);
    double diff = 0;
    while(diff<3) {
        double end = time(NULL);
        diff = difftime(end,start);
    }
    return 1;
}
