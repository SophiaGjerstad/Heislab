#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <assert.h>
//#include <pthread.h>
#include "Timer.h"
double startTimer(void) {
    double start = time(NULL);
    return start;
}//virker unødvendig

bool hasBeen3Seconds(double start_time) {
    double end = time(NULL);
    if(difftime(end,start_time)<=3) {
        return 1;
    }
    else {
        return 0;
    }
}

//burde kanskje lage en funksjon - handleObstruction som blir kalt når heis stopper på etasje.
//Da skal den starte Timer og loope for å sjekke etter obstruction med hasBeen3Seconds som argument. 

/*double start = time(NULL);
    double diff = 0;
    while(diff<3) {
        double end = time(NULL);
        diff = difftime(end,start);
    }
    return 1;
    */