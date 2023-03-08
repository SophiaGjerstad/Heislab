#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/ElevatorControl.h"
#include "driver/Timer.h"



int main(){

    elevio_init();

    ElevatorControlStruct elevatorControl = *elevatorControlInitializer();
    while (1){
        if (elevio_stopButton()){
            elevatorControl.currentState = EmergencyStop;
             continue;
        }

        switch (elevatorControl.currentState){

        case Startup:
            if (elevatorControl.door.doorOpen == false){
                elevatorControlMoveElevatorDown();
                while (elevio_floorSensor == -1){
                    elevatorControlUpdateInfo(&elevatorControl);
                    if (elevio_stopButton()){
                        elevatorControlStopElevator();
                        elevatorControl.currentState = EmergencyStop;
                        continue;
                    }
                }
                elevatorControlStopElevator();
                elevatorControl.currentState = Idle;
            } else {
                elevatorControlCloseDoor(&elevatorControl);
            }
                continue;

        case Idle:
            elevatorControlUpdateInfo(&elevatorControl);
            if ((elevatorControl.currentServiceingMode == UpMode) && isThereRequestAbove(&elevatorControl.orderHandler,elevatorControl.currentFloor)){
                elevatorControlMoveElevatorUp();
                elevatorControl.currentState = MovingUp;
                continue;
            }

            if ((elevatorControl.currentServiceingMode == DownMode) && isThereRequestBelow(&elevatorControl.orderHandler,elevatorControl.currentFloor)){
                elevatorControlMoveElevatorDown();
                elevatorControl.currentState = MovingDown;
                continue;
            }

            elevatorControl.currentServiceingMode == NoMode;
            while(1){
                elevatorControlUpdateInfo(&elevatorControl);
                if (elevio_stopButton()){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = EmergencyStop;
                    continue;
                }

                if(elevatorControlCheckIfShouldService(&elevatorControl)){
                    elevatorControl.currentState == ServicingFloor;
                    continue;
                }

                if(isThereRequestAbove(&elevatorControl.orderHandler, elevatorControl.currentFloor)){
                    elevatorControl.currentServiceingMode = UpMode;
                    elevatorControl.currentState = MovingUp;
                    elevatorControlMoveElevatorUp();
                    continue;
                }

                if(isThereRequestBelow(&elevatorControl.orderHandler, elevatorControl.currentFloor)){
                    elevatorControl.currentServiceingMode = DownMode;
                    elevatorControl.currentState = MovingDown;
                    elevatorControlMoveElevatorDown();
                    continue;
                }
            }
            continue;

        case EmergencyStop:
            elevatorControlStopElevator();
            elevatorControlClearAllOrders(&elevatorControl);
            while (elevio_stopButton()){
            
            }
            elevatorControl.currentState = Startup;
            continue;

        case MovingUp:
            while (1){
                elevatorControlUpdateInfo(&elevatorControl);
                int lastFloor = elevatorControl.currentFloor;
                while (lastFloor == elevatorControl.currentFloor){
                    elevatorControlUpdateInfo(&elevatorControl);
                    if (elevio_stopButton()){
                        elevatorControlStopElevator();
                        elevatorControl.currentState = EmergencyStop;
                        continue;
                    }
                    elevatorControlUpdateFloor(&elevatorControl);
                }
                if (elevatorControlCheckIfShouldService(&elevatorControl)){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = ServicingFloor;
                    continue;
                }

                if (!isThereRequestBelow(&elevatorControl,elevatorControl.currentFloor)){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = Idle;
                    continue;
                }
            }
            continue;

        case MovingDown:
            while (1){
                elevatorControlUpdateInfo(&elevatorControl);
                int lastFloor = elevatorControl.currentFloor;
                while (lastFloor == elevatorControl.currentFloor){
                    elevatorControlUpdateInfo(&elevatorControl);
                    if (elevio_stopButton()){
                        elevatorControlStopElevator();
                        elevatorControl.currentState = EmergencyStop;
                        continue;
                    }
                    elevatorControlUpdateFloor(&elevatorControl);
                }

                if (elevatorControlCheckIfShouldService(elevatorControl.currentFloor)){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = ServicingFloor;
                    continue;
                }
                
                if (!isThereRequestBelow(&elevatorControl,elevatorControl.currentFloor)){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = Idle;
                    continue;
                }
               
            continue;

        
        case ServicingFloor:
            elevatorControlUpdateInfo(&elevatorControl);
            elevatorControlOpenDoor(&elevatorControl);
            elevatorControlDeleteOrdersOnFloor(&elevatorControl);
            if (elevio_stopButton()){
                elevatorControlStopElevator();
                elevatorControl.currentState = EmergencyStop;
                continue;
            }
            TimerAlertAfter3Seconds();
            elevatorControlUpdateInfo(&elevatorControl);
            if (elevio_stopButton()){
                elevatorControlStopElevator();
                elevatorControl.currentState = EmergencyStop;
                continue;
            }
            while(1){
                
                elevatorControlUpdateInfo(&elevatorControl);
                
                if (elevio_stopButton()()){
                elevatorControlStopElevator();
                elevatorControl.currentState = EmergencyStop;
                continue;
                }
                
                if(!elevatorControlCheckObstruction()){
                    elevatorControlCloseDoor(&elevatorControl);
                    elevatorControl.currentState = Idle;
                    continue;
                }
            }   
            continue;
        
        default:
            elevatorControl.currentState = Startup;
            continue;
        }
    }


    
    // printf("=== Example Program ===\n");
    // printf("Press the stop button on the elevator panel to exit\n");

    // elevio_motorDirection(DIRN_UP);

    // while(1){
    //     int floor = elevio_floorSensor();
    //     printf("floor: %d \n",floor);

    //     if(floor == 0){
    //         elevio_motorDirection(DIRN_UP);
    //     }

    //     if(floor == N_FLOORS-1){
    //         elevio_motorDirection(DIRN_DOWN);
    //     }


    //     for(int f = 0; f < N_FLOORS; f++){
    //         for(int b = 0; b < N_BUTTONS; b++){
    //             int btnPressed = elevio_callButton(f, b);
    //             elevio_buttonLamp(f, b, btnPressed);
    //         }
    //     }

    //     if(elevio_obstruction()){
    //         elevio_stopLamp(1);
    //     } else {
    //         elevio_stopLamp(0);
    //     }
        
    //     if(elevio_stopButton()()){
    //         elevio_motorDirection(DIRN_STOP);
    //         continue;
    //     }
        
    //     nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    // }

    return 0;
}
