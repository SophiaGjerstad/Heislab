#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/ElevatorControl.h"




int main(){

    elevio_init();
    
    ElevatorControlStruct elevatorControl_o = elevatorControlInitializer();
    ElevatorControlStruct *elevatorControl = &elevatorControl_o;
    elevatorControlClearAllOrders(elevatorControl);
    while (1){
        emergency:
        
        if (elevio_stopButton()){
            elevatorControl->currentState = EmergencyStop;
        }

        switch (elevatorControl->currentState){

        case Startup:
            printf("Starting Startup\n");
            if (elevatorControl->door.doorOpen == false){
                printf("Door not open, moving down\n");
                elevatorControlMoveElevatorDown();
                bool done = false;
                while (!done){
                    int floor = elevio_floorSensor();
                    if (floor != -1){
                        printf("%d\n",floor);
                        elevatorControlUpdateFloor(elevatorControl, floor);
                        break;
                    }
                    if (elevio_stopButton()){
                        elevatorControlStopElevator();
                        elevatorControl->currentState = EmergencyStop;
                        break;
                    }

                }
               
                
                if (elevio_stopButton()){
                        elevatorControlStopElevator();
                        elevatorControlOpenDoor(elevatorControl);
                        elevatorControl->currentState = EmergencyStop;
                        break;
                }
                printf ("Stopping\n");
                elevatorControlStopElevator();
                elevatorControl->currentState = Idle;
            } else {
                while (elevatorControl->door.doorOpen){
                    
                    if (elevio_stopButton()){
                     elevatorControl->currentState = EmergencyStop;
                    break;
                    }

                    if (!elevatorControlCheckObstruction())
                    {
                        elevatorControlCloseDoor(elevatorControl);
                        break;
                    }else{
                        while (elevatorControlCheckObstruction()){
                            elevatorControl->timer = time(NULL);
                        }
                        while(!hasBeen3Seconds(elevatorControl->timer)){

                        }
                        elevatorControlCloseDoor(elevatorControl);
                        break;
                    }
                }
            }

            break; //mulig overflødig break

        case Idle:
            printf("Entering Idle\n");
            elevatorControlUpdateInfo(elevatorControl);
            if ((elevatorControl->currentServiceingMode == UpMode) && isThereRequestAbove(elevatorControl->orderHandler,elevatorControl->currentFloor)){
                printf("I see a request above, moving up\n");
                elevatorControlMoveElevatorUp();
                elevatorControl->currentState = MovingUp;
                break;
            }

            if ((elevatorControl->currentServiceingMode == DownMode) && isThereRequestBelow(elevatorControl->orderHandler,elevatorControl->currentFloor)){
                printf("I see a request below, moving down\n");
                elevatorControlMoveElevatorDown();
                elevatorControl->currentState = MovingDown;
                break;
            }


            elevatorControl->currentServiceingMode = NoMode;
            printf("No relevant orders, broadening scope\n");
            while(1){
                elevatorControlUpdateInfo(elevatorControl);

                if (elevio_stopButton()){
                    elevatorControlStopElevator();
                    elevatorControlOpenDoor(elevatorControl);
                    elevatorControl->currentState = EmergencyStop;
                    break;
                }

                if(elevatorControlCheckIfShouldService(elevatorControl)){
                    printf("Serviceing floor\n");
                    elevatorControl->currentState = ServicingFloor;
                    break;
                }

                if(isThereRequestAbove(elevatorControl->orderHandler, elevatorControl->currentFloor)){
                    printf("I see a request above, moving up\n");
                    elevatorControl->currentServiceingMode = UpMode;
                    elevatorControl->currentState = MovingUp;
                    elevatorControlMoveElevatorUp();
                    break;
                }

                if(isThereRequestBelow(elevatorControl->orderHandler, elevatorControl->currentFloor)){
                    printf("I see a request below, moving down\n");
                    elevatorControl->currentServiceingMode = DownMode;
                    elevatorControl->currentState = MovingDown;
                    elevatorControlMoveElevatorDown();

                    break;
                }
            }
            break;

        case EmergencyStop:
            printf("EMERGENCY! STOPPING AND CLEARING ORDERS\n");
            elevio_stopLamp(1);
            elevatorControlStopElevator();
            elevatorControlClearAllOrders(elevatorControl);
            
            while (elevio_stopButton()){
                
            }
            
            elevatorControl->timer = time(NULL);
            elevio_stopLamp(0);

            if (elevatorControl->door.doorOpen){
                
                while(!hasBeen3Seconds(elevatorControl->timer)){
                    if (elevatorControlCheckObstruction()){
                        elevatorControl->timer = time(NULL);
                    }
                }
                elevatorControlCloseDoor(elevatorControl);
            }

            elevatorControl->currentState = Idle;
            break;

        case MovingUp:
            while (1){
                elevatorControlUpdateInfo(elevatorControl);
                int lastFloor = elevatorControl->currentFloor;
                while (lastFloor == elevatorControl->currentFloor){
                    elevatorControlUpdateInfo(elevatorControl);
                    if (elevio_stopButton()){
                        elevatorControlStopElevator();
                        elevatorControl->currentState = EmergencyStop;
                        goto emergency;
                    }
                    int floor = elevio_floorSensor();
                    if (floor != -1){
                        elevatorControlUpdateFloor(elevatorControl, floor);
                        break;
                    }
                }
                if (elevatorControlCheckIfShouldService(elevatorControl)){
                    elevatorControlStopElevator();
                    elevatorControlUpdateInfo(elevatorControl);
                    elevatorControl->currentState = ServicingFloor;
                    break;
                }

                if (!isThereRequestAbove(elevatorControl->orderHandler,elevatorControl->currentFloor)){
                    elevatorControlStopElevator();
                    elevatorControl->currentState = Idle;
                    break;
                }
                
            }
            break;

        case MovingDown:
            while (1){
            
                elevatorControlUpdateInfo(elevatorControl);
                int lastFloor = elevatorControl->currentFloor;
                while (lastFloor == elevatorControl->currentFloor){
                    elevatorControlUpdateInfo(elevatorControl);
                    if (elevio_stopButton()){
                        elevatorControlStopElevator();
                        elevatorControl->currentState = EmergencyStop;
                        goto emergency;
                    }

                    int floor = elevio_floorSensor();
                    if (floor != -1){
                    elevatorControlUpdateFloor(elevatorControl, floor);
                    }
                }

                if (elevatorControlCheckIfShouldService(elevatorControl)){
                    elevatorControlStopElevator();
                    elevatorControl->currentState = ServicingFloor;
                    break;
                }
                
                if (!isThereRequestBelow(elevatorControl->orderHandler,elevatorControl->currentFloor)){
                    elevatorControlStopElevator();
                    elevatorControl->currentState = Idle;
                    break;
                }
            }
               
            break;

        
        case ServicingFloor:
            
            elevatorControlUpdateInfo(elevatorControl);
            elevatorControlOpenDoor(elevatorControl);
            elevatorControl->timer = time(NULL);
            
            printf("Waiting\n");
            while (!hasBeen3Seconds(elevatorControl->timer)){
                elevatorControlUpdateInfo(elevatorControl);
                if(elevatorControlCheckObstruction()){
                    elevatorControl->timer = time(NULL);
                }

                if (elevio_stopButton()){
                    elevatorControlStopElevator();
                    elevatorControl->currentState = EmergencyStop;
                    break;
                }
            }
            elevatorControlDeleteOrdersOnFloor(elevatorControl, elevatorControl->currentFloor);
            printf("That was a loong time \n");
            
            if (elevatorControl->currentState == EmergencyStop){
                break;
            }
            while (elevatorControl->door.doorOpen){
                    
                    if (elevio_stopButton()){
                     elevatorControl->currentState = EmergencyStop;
                    break;
                    }

                    if (!elevatorControlCheckObstruction())
                    {
                        elevatorControlCloseDoor(elevatorControl);
                        break;
                    }else{
                        while (elevatorControlCheckObstruction()){
                            elevatorControl->timer = time(NULL);
                        }
                        while(!hasBeen3Seconds(elevatorControl->timer)){

                        }
                        elevatorControlCloseDoor(elevatorControl);
                        break;
                    }
            }

            elevatorControl->currentState = Idle;
        
            break;
        
        default:
            elevatorControl->currentState = Startup;
            break;
        }
    }
    return 0;
}
