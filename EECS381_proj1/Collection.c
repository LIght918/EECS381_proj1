/*
//  Collection.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Collection.h"



/* a Collection contains a pointer to a C-string name and a container
 that holds pointers to Records - the members. */
struct Collection {
    char* name;
    struct Ordered_container* members;
};
