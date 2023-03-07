#pragma once
#include <stdbool.h>

typedef enum {
    Upp,
    Down,
    CabinRequest
}RequestType;

typedef struct OrderHandlerStruct
{
    bool orderMatrix [4][3];
    void (*addToOrderHandlerMatrix)(OrderHandlerStruct *, int floor, RequestType typeOfRequest);
    void (*deleteFromOrderHandlerMatrix)(OrderHandlerStruct *,int floor, RequestType typeOfRequest);
    bool (*isThereRequestAbove)(OrderHandlerStruct *inst,int floor);
    bool (*isThereRequestBelow)(OrderHandlerStruct *inst,int floor);
    int (*isThereRequestAtFloor)(OrderHandlerStruct *inst,int floor);
}OrderHandlerStruct;

void addToOrderHandlerMatrix(OrderHandlerStruct *inst, int floor, RequestType typeOfRequest);//Adds an order of the specified type to the floor
void deleteFromOrderHandlerMatrix(OrderHandlerStruct *inst,int floor, RequestType typeOfRequest);//Deletes an order of the specified type to the floor
bool isThereRequestAbove(OrderHandlerStruct *inst,int floor);//Checks if there are any requests above the floor. 
int isThereRequestAtFloor(OrderHandlerStruct *inst,int floor);//Checks if there are any requests above the floor. 
bool isThereRequestBelow(OrderHandlerStruct *inst,int floor);//Checks if there are any requests above the floor. 

void orderHandlerInitializeHandler(void);
//This function initializes a 4x3 matrix to hold requests for elevator floors and cabin buttons. 
//The function should dynamically allocate memory and set all values in the matrix to zero.
//[][] - how does this indexing work?
//4 rows indicating each floor. Index[0] = first floor?
//3 columns indicating order up, order down, order to specific floor number from cabin. 
//Think about MINNELEKKASJE bruh

