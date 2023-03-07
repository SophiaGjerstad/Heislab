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
    ElevatorDoor.doorChangeDoorStatus = doorChangeDoorStatus;
    ElevatorDoor.doorChangeLightStatus = doorChangeLightStatus;
    ElevatorDoor.doorOpen = 0;
    ElevatorDoor.doorLampOn = 0;
    Door * pointerElevatorDoor = &ElevatorDoor;
    return pointerElevatorDoor;
}
//er ikke dette overflødig?


void doorChangeDoorStatus(Door * pointerToDoor) {
    //this should only change the door status. No logic
    if (pointerToDoor->doorOpen) {
        pointerToDoor->doorOpen = 0;
    } else {
        pointerToDoor->doorOpen = 1;
    }
}

void doorChangeLightStatus(Door * pointerToDoor) {
    if (pointerToDoor->doorLampOn) {
        pointerToDoor->doorLampOn = 0;
    } else {
        pointerToDoor->doorLampOn = 1;
    }
}


