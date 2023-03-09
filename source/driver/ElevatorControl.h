#pragma once
#include "door.h"
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
    double timer;
}ElevatorControlStruct;

ElevatorControlStruct elevatorControlInitializer(void);
void elevatorControlSetElevatorMode(ElevatorControlStruct *elevatorControlPointer, ServiceMode typeOfServiceMode);
void elevatorControlSetElevatorState(ElevatorControlStruct *elevatorControlPointer, State typeOfState);
bool elevatorControlCheckObstruction(void); // returnerer true dersom det er en obstruksjon
void elevatorControlOpenDoor(ElevatorControlStruct *elevatorControlPointer);
void elevatorControlCloseDoor(ElevatorControlStruct *elevatorControlPointer);
void elevatorControlUpdateFloor(ElevatorControlStruct *elevatorControlPointer, int floor);
bool elevatorControlCheckIfShouldService(ElevatorControlStruct *elevatorControlPointer);
void elevatorControlMoveElevatorUp();
void elevatorControlMoveElevatorDown();
void elevatorControlStopElevator();
void elevatorControlUpdateInfo(ElevatorControlStruct *elevatorControlPointer);
void elevatorControlDeleteOrdersOnFloor(ElevatorControlStruct *elevatorControlPointer, int floor);
void elevatorControlClearAllOrders(ElevatorControlStruct *elevatorControlPointer);

bool hasBeen3Seconds(double start_time);