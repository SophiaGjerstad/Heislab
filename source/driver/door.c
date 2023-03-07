#include <stdlib.h>
#include <stdio.h>
#include "door.h"
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <assert.h>
//#include <pthread.h>

/*Copied all includes from elevio.c file. 
Don't know if all are needed.
Therefore some commented out, but have them ready in this file in case.
*/

Door* initializeDoor(void) {
    Door ElevatorDoor;
    ElevatorDoor.doorOpen = 0;
    ElevatorDoor.doorLampOn = 0;
    return &ElevatorDoor;
}


