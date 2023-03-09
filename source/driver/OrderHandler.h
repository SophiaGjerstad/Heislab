#pragma once
#include <stdbool.h>
typedef enum {
    UpRequest,
    DownRequest,
    CabinRequest
}RequestType;

typedef struct OrderHandlerStruct
{
    bool orderMatrix[4][3];
}OrderHandlerStruct;

void addToOrderHandlerMatrix(OrderHandlerStruct *inst, int floor, RequestType typeOfRequest);//Adds an order of the specified type to the floor
void deleteFromOrderHandlerMatrix(OrderHandlerStruct *inst,int floor, RequestType typeOfRequest);//Deletes an order of the specified type to the floor
bool isThereRequestAbove(OrderHandlerStruct inst,int currentFloor);//Checks if there are any requests above the floor. 
bool isThereRequestAtFloor(OrderHandlerStruct inst,int currentFloor, RequestType typeOfRequest);//Checks if there are any requests above the floor. 
bool isThereRequestBelow(OrderHandlerStruct inst, int currentFloor);//Checks if there are any requests above the floor. 

OrderHandlerStruct* orderHandlerInitializer(void);
//This function initializes a 4x3 matrix to hold requests for elevator floors and cabin buttons. 
//The function should dynamically allocate memory and set all values in the matrix to zero.
//4 rows indicating each floor. Index[0] = first floor?
//3 columns indicating order up, order down, order to specific floor number from cabin. 
//Think about MINNELEKKASJE bruh

