#include <stdlib.h>
#include <stdio.h>
#include "ElevatorControl.h"
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <assert.h>
//#include <pthread.h>

ElevatorControlStruct* elevatorControlInitializer(void){
    ElevatorControlStruct elevatorControl;
    elevatorControl.currentFloor = 0;
    elevatorControl.currentState = Startup;
    elevatorControl.currentServiceingMode = NoMode;
    elevatorControl.orderHandler = *orderHandlerInitializer();
    elevatorControl.door = *initializeDoor();
    return &elevatorControl;
}

void elevatorControlSetElevatorMode(ElevatorControlStruct *elevatorControlPointer, ServiceMode typeOfServiceMode){
    elevatorControlPointer->currentServiceingMode = typeOfServiceMode;
}

void elevatorControlSetElevatorState(ElevatorControlStruct *elevatorControlPointer, State typeOfState){
    elevatorControlPointer->currentState = typeOfState;
}

bool elevatorControlCheckObstruction(void){

}


void elevatorControlOpenDoor(ElevatorControlStruct *elevatorControlPointer){
    elevatorControlPointer->door.doorOpen = true;
    elevatorControlPointer->door.doorLampOn = true;
}


void elevatorControlCloseDoor(ElevatorControlStruct *elevatorControlPointer){
    elevatorControlPointer->door.doorOpen = false;
    elevatorControlPointer->door.doorLampOn = false;
}


void elevatorControlUpdateFloor(ElevatorControlStruct *elevatorControlPointer){
    if (elevio_floorSensor != -1){
        elevatorControlPointer->currentFloor = elevio_floorSensor;
    }
}

bool elevatorControlCheckIfShouldService(ElevatorControlStruct *elevatorControlPointer){
    switch (elevatorControlPointer->currentServiceingMode)
    {
    case UpMode:
        //Stopper dersom det er en relevant bestilling på etasjen
        if (isThereRequestAtFloor(&elevatorControlPointer->orderHandler, elevatorControlPointer->currentFloor, UpRequest)){ 
            return true;
        }
        //Stopper også dersom det ikke finnes bestillinger i etasjer over
        if (isThereRequestAbove(&elevatorControlPointer->orderHandler,elevatorControlPointer->currentFloor == false)){
            return true;
        }
        return false;
        break;

    case DownMode:
        //Stopper dersom det er en relevant bestilling på etasjen
        if (isThereRequestAtFloor(&elevatorControlPointer->orderHandler, elevatorControlPointer->currentFloor, DownRequest)){ 
            return true;
        }
        //Stopper også dersom det ikke finnes bestillinger i etasjer under
        if (isThereRequestBelow(&elevatorControlPointer->orderHandler,elevatorControlPointer->currentFloor == false)){
            return true;
        }

        return false;
        break;

    case NoMode:
        //Stopper dersom det er en relevant bestilling på etasjen. Sjekker begge request typer.
        if (isThereRequestAtFloor(&elevatorControlPointer->orderHandler, elevatorControlPointer->currentFloor, UpRequest)){ 
            return true;
        }
        if (isThereRequestAtFloor(&elevatorControlPointer->orderHandler,elevatorControlPointer->currentFloor, DownRequest)){
            return true;
        }
        
        return false;
        break;
    
    default:
        return false;
        break;
    }
}

void elevatorControlMoveElevatorUp(){
    elevio_motorDirection(DIRN_UP);
}

void elevatorControlMoveElevatorDown(){
    elevio_motorDirection(DIRN_DOWN);
}

void elevatorControlStopElevator(){
    elevio_motorDirection(DIRN_STOP);
}

