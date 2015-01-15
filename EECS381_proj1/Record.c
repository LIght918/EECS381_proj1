/*
//  Record.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Record.h"

/* skeleton file for Record.c
 The struct declaration below must be used for Record objects.
 Remove this comment and complete this file with all necessary code.
 */

/* a Record contains an int ID, rating, and pointers to C-strings for the title and medium */
struct Record {
    char* title;
    int ID;
    char* medium;
    int rating;
};

