#pragma once
#include "Door.h"

//This file will contain functions that will be implemented in a state-machine based switch in main?

typedef enum {Idle, EmergencyStop, MovingUp, MovingDown, Startup, ServicingFloor} State;

typedef enum {Up, Down, none} ServiceMode;

typedef struct ElevatorControl{
    int currentFloor;
    State currentState;
    ServiceMode currentServiceingMode;
}ElevatorControl;

ElevatorControl* elevatorControlInitializer(void);

void elevatorControlSetElevatorMode(ElevatorControl *inst, ServiceMode typeOfServiceMode);
void elevatorControlSetElevatorState(ElevatorControl *inst, State typeOfState);
void elevatorControlStartElevator(void);
void elevatorControlStopElevator(void);
bool elevatorControlCheckObstruction(void);
void elevatorControlOpenDoor(Door *inst);
void elevatorControlCloseDoor(Door *inst);
void elevatorControlUpdateFloor(ElevatorControl *inst);
bool elevatorControlCheckIfStopped(ElevatorControl *inst);
