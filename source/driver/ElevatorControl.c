#include <stdlib.h>
#include <stdio.h>
#include "ElevatorControl.h"
#include "time.h"
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
    elevatorControl.timer = time(NULL);
    return &elevatorControl;
}

void elevatorControlSetElevatorMode(ElevatorControlStruct *elevatorControlPointer, ServiceMode typeOfServiceMode){
    elevatorControlPointer->currentServiceingMode = typeOfServiceMode;
}

void elevatorControlSetElevatorState(ElevatorControlStruct *elevatorControlPointer, State typeOfState){
    elevatorControlPointer->currentState = typeOfState;
}

bool elevatorControlCheckObstruction(void){
    return elevio_obstruction();
}


void elevatorControlOpenDoor(ElevatorControlStruct *elevatorControlPointer){
    elevatorControlPointer->door.doorOpen = true;
    elevatorControlPointer->door.doorLampOn = true;
    elevio_doorOpenLamp(1);
}


void elevatorControlCloseDoor(ElevatorControlStruct *elevatorControlPointer){
    elevatorControlPointer->door.doorOpen = false;
    elevatorControlPointer->door.doorLampOn = false;
    elevio_doorOpenLamp(0);
}


void elevatorControlUpdateFloor(ElevatorControlStruct *elevatorControlPointer){
    if (elevio_floorSensor != -1){
        elevatorControlPointer->currentFloor = elevio_floorSensor;
        elevio_floorIndicator(elevio_floorSensor());
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
        return false;
        break;

    case DownMode:
        //Stopper dersom det er en relevant bestilling på etasjen
        if (isThereRequestAtFloor(&elevatorControlPointer->orderHandler, elevatorControlPointer->currentFloor, DownRequest)){ 
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

void elevatorControlUpdateInfo(ElevatorControlStruct *elevatorControlPointer){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(f, b);
            if (btnPressed){
                elevio_buttonLamp(f, b, btnPressed);
                addToOrderHandlerMatrix(&elevatorControlPointer->orderHandler, f, b);
            }
        }
}

void elevatorControlDeleteOrdersOnFloor(ElevatorControlStruct *elevatorControlPointer){
    for (int button = 0; button < N_BUTTONS; button++){
        elevio_buttonLamp(elevatorControlPointer->currentFloor - 1, button, 0);
        deleteFromOrderHandlerMatrix(&elevatorControlPointer->orderHandler,elevatorControlPointer->currentFloor - 1, button);
    }
}

void elevatorControlClearAllOrders(ElevatorControlStruct *elevatorControlPointer){
    for (int floor = 0; floor < N_FLOORS; floor++){
        elevatorControlDeleteOrdersOnFloor(elevatorControlPointer);
    }

}

bool hasBeen3Seconds(double start_time) {
    double end = time(NULL);
    if(difftime(end,start_time)<=3) {
        return 1;
    }
    else {
        return 0;
    }
}



//Manger: ServiceFloor funksjonalitet som sletter bestillinger og skrur av lamper. 