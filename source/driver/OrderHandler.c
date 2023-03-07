#include "OrderHandler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <assert.h>
//#include <pthread.h>

void orderHandlerInitializeHandler(void){
    OrderHandlerStruct orderHandler = {{{0,0,0},{0,0,0},{0,0,0},{0,0,0}}};
    orderHandler.addToOrderHandlerMatrix = addToOrderHandlerMatrix;
    orderHandler.deleteFromOrderHandlerMatrix = deleteFromOrderHandlerMatrix;
    orderHandler.isThereRequestAbove = isThereRequestAbove;
    orderHandler.isThereRequestBelow = isThereRequestBelow;
    orderHandler.isThereRequestAtFloor = isThereRequestAtFloor;
    //dynamically allocate memory to 4x3 matrix. 
    //set values to 0. 
    //return a pointer to matrix somehow? We need to keep track of the initialized matrix and
    //be able to modify it later in the program. 
}

void addToOrderHandlerMatrix(OrderHandlerStruct *inst, int floor, RequestType typeOfRequest){
    inst->orderMatrix[floor-1][typeOfRequest] = 1;
}

void deleteFromOrderHandlerMatrix(OrderHandlerStruct *inst, int floor, RequestType typeOfRequest){
    inst->orderMatrix[floor-1][typeOfRequest] = 0;
}

bool isThereRequestAbove(OrderHandlerStruct *inst,int currentFloor){
    for (int i = currentFloor - 1; i < 4; i++){
        for (int j = 0; j < 3; j++){
            if (inst->orderMatrix[i][j] == true){
                return true;
            }
        }
    }
    return false;
}

bool isThereRequestBelow(OrderHandlerStruct *inst,int currentFloor){
    for (int i = currentFloor - 1; i >= 0; i--){
        for (int j = 0; j < 3; j++){
            if (inst->orderMatrix[i][j] == true){
                return true;
            }
        }
    }
    return false;
}

bool isThereRequestAtFloor(OrderHandlerStruct *inst,int currentFloor, RequestType typeOfRequest){
    if (inst->orderMatrix[currentFloor - 1][2] == true){
        return true;
    }

    if(inst->orderMatrix[currentFloor - 1][typeOfRequest] == true){
        return true;
     }
    
    return false;
}



