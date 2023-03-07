#pragma once
#include "Door.h"

//This file will contain functions that will be implemented in a state-machine based switch in main?

typedef enum {Idle, EmergencyStop, MovingUp, MovingDown, Startup, ServicingFloor} State;

typedef enum {Up, Down, none} ServiceMode;

typedef struct ElevatorControlStruct{
    int currentFloor;
    State currentState;
    ServiceMode currentServiceingMode;
}ElevatorControlStruct;

ElevatorControlStruct* elevatorControlInitializer(void);

void elevatorControlSetElevatorMode(ElevatorControlStruct *inst, ServiceMode typeOfServiceMode);
void elevatorControlSetElevatorState(ElevatorControlStruct *inst, State typeOfState);
void elevatorControlStartElevator(void);
void elevatorControlStopElevator(void);
bool elevatorControlCheckObstruction(void);
void elevatorControlOpenDoor(Door *inst);
void elevatorControlCloseDoor(Door *inst);
void elevatorControlUpdateFloor(ElevatorControlStruct *inst);
bool elevatorControlCheckIfStopped(ElevatorControlStruct *inst);
