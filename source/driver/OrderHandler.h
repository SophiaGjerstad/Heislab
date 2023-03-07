#pragma once

void orderHandlerInitializeHandler(void);
//This function initializes a 4x3 matrix to hold requests for elevator floors and cabin buttons. 
//The function should dynamically allocate memory and set all values in the matrix to zero.
//[][] - how does this indexing work?
//4 rows indicating each floor. Index[0] = first floor?
//3 columns indicating order up, order down, order to specific floor number from cabin. 
//Think about MINNELEKKASJE bruh
