#pragma once

//This file will contain functions that will be implemented in a state-machine based switch in main?

typedef enum {Idle, EmergencyStop, MovingUp, MovingDown, Startup, ServicingFloor} State;

typedef enum {Up, Down, none} ServiceMode;

typedef struct ElevatorControl{
    int currentFloor;
    State currentState;
    ServiceMode currentServiceingMode;
}ElevatorControl;
