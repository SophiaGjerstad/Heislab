#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/ElevatorControl.h"



int main(){

    elevio_init();

    ElevatorControlStruct elevatorControl = *elevatorControlInitializer();
    while (1){
     if (elevio_stopButton){
            elevatorControl.currentState = EmergencyStop;
            break;
        }

        switch (elevatorControl.currentState)
        {

        case Startup:
            if (elevatorControl.door.doorOpen == false){
                elevatorControlMoveElevatorDown();
                while (elevio_floorSensor == -1)
                {
                    if (elevio_stopButton){
                        elevatorControlStopElevator();
                        elevatorControl.currentState = EmergencyStop;
                        break;
                    }
                }
                elevatorControlStopElevator();
                elevatorControl.currentState = Idle;
            }
            else
            {
                elevatorControlCloseDoor(&elevatorControl);
            }
                break;

        case Idle:
            if (elevio_stopButton){
                elevatorControlStopElevator();
                elevatorControl.currentState = EmergencyStop;
                break;
            }
            
            break;

        case EmergencyStop:
            elevatorControlStopElevator();
            clearOrderHandlerMatrix(&elevatorControl.orderHandler);
            while (elevio_stopButton){
            
            }
            elevatorControl.currentState = Startup;
            break;

        case MovingUp:
            while (1){
                int lastFloor = elevatorControl.currentFloor;
                while (lastFloor == elevatorControl.currentFloor){
                    if (elevio_stopButton){
                        elevatorControlStopElevator();
                        elevatorControl.currentState = EmergencyStop;
                        break;
                    }
                    elevatorControlUpdateFloor(&elevatorControl);
                }
                if (elevatorControlCheckIfShouldStop(elevatorControl.currentFloor)){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = ServicingFloor;
                    break;
                }
            }
            break;

        case MovingDown:
            while (1){
                int lastFloor = elevatorControl.currentFloor;
                while (lastFloor == elevatorControl.currentFloor){
                    if (elevio_stopButton){
                        elevatorControlStopElevator();
                        elevatorControl.currentState = EmergencyStop;
                        break;
                    }
                    elevatorControlUpdateFloor(&elevatorControl);
                }
                if (elevatorControlCheckIfShouldStop(elevatorControl.currentFloor)){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = ServicingFloor;
                    break;
                }
            }
            break;

        
        case ServicingFloor:
            if (elevio_stopButton){
                elevatorControlStopElevator();
                elevatorControl.currentState = EmergencyStop;
                break;
            }
            break;
        
        default:
            elevatorControl.currentState = Startup;
            break;
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
        
    //     if(elevio_stopButton()){
    //         elevio_motorDirection(DIRN_STOP);
    //         break;
    //     }
        
    //     nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    // }

    return 0;
}
