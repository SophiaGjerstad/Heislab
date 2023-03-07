#pragma once
#include <stdbool.h>

typedef struct Door{
    bool doorOpen; 
    bool doorLampOn; 
    void (*doorChangeDoorStatus) (Door * pointerToDoor); //function pointers
    void (*doorChangeLightStatus) (Door * pointerToDoor);
}Door;


Door* initializeDoor(void);
/*this function initializes the struct Door by creating an instance of Door and
inking the function pointers in the struct to the "set functions" doorChangeDoorStatus and
doorChangeLightStatus. 
It returns a pointer to the instance. Only use this one instance throughout the program!!*/

//would get functions also be useful/necessary?

void doorChangeDoorStatus(Door * pointerToDoor);
//this function changes the door status to the opposite of what it was before. No other logic

void doorChangeLightStatus(Door * pointerToDoor);
//this function changes the light status to the opposite of what it was before. No other logic