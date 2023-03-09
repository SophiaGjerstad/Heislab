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
        
        if (elevio_stopButton()){
            elevatorControl.currentState = EmergencyStop;
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
                        break;
                    }
                }

                if (elevio_stopButton()){
                        elevatorControlStopElevator();
                        elevatorControlOpenDoor(&elevatorControl);
                        elevatorControl.currentState = EmergencyStop;
                        break;
                }

                elevatorControlStopElevator();
                elevatorControl.currentState = Idle;
            } else {
                while (elevatorControl.door.doorOpen){
                    
                    if (elevio_stopButton()){
                     elevatorControl.currentState = EmergencyStop;
                    break;
                    }

                    if (!elevatorControlCheckObstruction())
                    {
                        elevatorControlCloseDoor(&elevatorControl);
                        break;
                    }else{
                        while (elevatorControlCheckObstruction()){
                            elevatorControl.timer = time(NULL);
                        }
                        while(!hasBeen3Seconds(elevatorControl.timer)){

                        }
                        elevatorControlCloseDoor(&elevatorControl);
                        break;
                    }
                }
            }

            break; //mulig overflødig break

        case Idle:
            elevatorControlUpdateInfo(&elevatorControl);
            if ((elevatorControl.currentServiceingMode == UpMode) && isThereRequestAbove(&elevatorControl.orderHandler,elevatorControl.currentFloor)){
                elevatorControlMoveElevatorUp();
                elevatorControl.currentState = MovingUp;
                break;
            }

            if ((elevatorControl.currentServiceingMode == DownMode) && isThereRequestBelow(&elevatorControl.orderHandler,elevatorControl.currentFloor)){
                elevatorControlMoveElevatorDown();
                elevatorControl.currentState = MovingDown;
                break;
            }

            elevatorControl.currentServiceingMode == NoMode;
            while(1){
                elevatorControlUpdateInfo(&elevatorControl);

                if (elevio_stopButton()){
                    elevatorControlStopElevator();
                    elevatorControlOpenDoor(&elevatorControl);
                    elevatorControl.currentState = EmergencyStop;
                    break;
                }

                if(elevatorControlCheckIfShouldService(&elevatorControl)){
                    elevatorControl.currentState == ServicingFloor;
                    break;
                }

                if(isThereRequestAbove(&elevatorControl.orderHandler, elevatorControl.currentFloor)){
                    elevatorControl.currentServiceingMode = UpMode;
                    elevatorControl.currentState = MovingUp;
                    elevatorControlMoveElevatorUp();
                    break;
                }

                if(isThereRequestBelow(&elevatorControl.orderHandler, elevatorControl.currentFloor)){
                    elevatorControl.currentServiceingMode = DownMode;
                    elevatorControl.currentState = MovingDown;
                    elevatorControlMoveElevatorDown();
                    break;
                }
            }
            break;

        case EmergencyStop:
            elevio_stopLamp(1);
            elevatorControlStopElevator();
            elevatorControlClearAllOrders(&elevatorControl);
            
            while (elevio_stopButton()){
                elevatorControl.timer = time(NULL);
            }

            elevio_stopLamp(0);

            if (elevatorControl.door.doorOpen){
                
                while(!hasBeen3Seconds(elevatorControl.timer)){
                    if (elevatorControlCheckObstruction()){
                        elevatorControl.timer = time(NULL);
                    }
                }
                elevatorControlCloseDoor(&elevatorControl);
            }

            elevatorControl.currentState = Startup;
            break;

        case MovingUp:
            while (1){
                elevatorControlUpdateInfo(&elevatorControl);
                int lastFloor = elevatorControl.currentFloor;
                while (lastFloor == elevatorControl.currentFloor){
                    elevatorControlUpdateInfo(&elevatorControl);
                    if (elevio_stopButton()){
                        elevatorControlStopElevator();
                        elevatorControl.currentState = EmergencyStop;
                        break;
                    }
                    elevatorControlUpdateFloor(&elevatorControl);
                }
                if (elevatorControlCheckIfShouldService(&elevatorControl)){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = ServicingFloor;
                    break;
                }

                if (!isThereRequestBelow(&elevatorControl,elevatorControl.currentFloor)){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = Idle;
                    break;
                }
            }
            break;

        case MovingDown:
            while (1){
                elevatorControlUpdateInfo(&elevatorControl);
                int lastFloor = elevatorControl.currentFloor;
                while (lastFloor == elevatorControl.currentFloor){
                    elevatorControlUpdateInfo(&elevatorControl);
                    if (elevio_stopButton()){
                        elevatorControlStopElevator();
                        elevatorControl.currentState = EmergencyStop;
                        break;
                    }
                    elevatorControlUpdateFloor(&elevatorControl);
                }

                if (elevatorControlCheckIfShouldService(elevatorControl.currentFloor)){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = ServicingFloor;
                    break;
                }
                
                if (!isThereRequestBelow(&elevatorControl,elevatorControl.currentFloor)){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = Idle;
                    break;
                }
            }
               
            break;

        
        case ServicingFloor:
            
            elevatorControlUpdateInfo(&elevatorControl);
            elevatorControlOpenDoor(&elevatorControl);
            elevatorControl.timer = time(NULL);
            elevatorControlDeleteOrdersOnFloor(&elevatorControl);
            
            while (!hasBeen3Seconds(elevatorControl.timer)){
                elevatorControlUpdateInfo(&elevatorControl);
                if(elevatorControlCheckObstruction()){
                    elevatorControl.timer = time(NULL);
                }

                if (elevio_stopButton()){
                    elevatorControlStopElevator();
                    elevatorControl.currentState = EmergencyStop;
                    break;
                }
            }
            
            if (elevatorControl.currentState == EmergencyStop){
                break;
            }

            elevatorControlCloseDoor(&elevatorControl);
            elevatorControl.currentState = Idle;
        
            break;
        
        default:
            elevatorControl.currentState = Startup;
            break;
        }
    }
    return 0;
}
