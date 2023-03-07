#pragma once
#include <stdbool.h>

typedef struct Door{
    bool doorOpen; 
    bool doorLampOn;
}Door;


Door* initializeDoor(void);
/*this function initializes the struct Door by creating an instance of Door 
It returns a pointer to the instance. Only use this one instance throughout the program!!*/

