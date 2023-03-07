#pragma once
#include "Door.h"
#include "elevio.h"
#include "OrderHandler.h"

//This file will contain functions that will be implemented in a state-machine based switch in main?

typedef enum {Startup, Idle, EmergencyStop, MovingUp, MovingDown, ServicingFloor} State;

typedef enum {UpMode, DownMode, NoMode} ServiceMode;

typedef struct ElevatorControlStruct{
    int currentFloor;
    State currentState;
    ServiceMode currentServiceingMode;
    OrderHandlerStruct orderHandler;
    Door door;
}ElevatorControlStruct;

ElevatorControlStruct* elevatorControlInitializer(void);
void elevatorControlSetElevatorMode(ElevatorControlStruct *elevatorControlPointer, ServiceMode typeOfServiceMode);
void elevatorControlSetElevatorState(ElevatorControlStruct *elevatorControlPointer, State typeOfState);
bool elevatorControlCheckObstruction(void);
void elevatorControlOpenDoor(ElevatorControlStruct *elevatorControlPointer);
void elevatorControlCloseDoor(ElevatorControlStruct *elevatorControlPointer);
void elevatorControlUpdateFloor(ElevatorControlStruct *elevatorControlPointer);
bool elevatorControlCheckIfShouldStop(ElevatorControlStruct *elevatorControlPointer);
void elevatorControlMoveUp();
void elevatorControlMoveDown();
void elevatorControlStopElevator();

