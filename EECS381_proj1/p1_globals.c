/*
//  p1_globals.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "p1_globals.h"

int g_string_memory = 0 ;			/* number of bytes used in C-strings */

int g_Container_count = 0 ;		/* number of Ordered_containers currently allocated */
int g_Container_items_in_use = 0 ;	/* number of Ordered_container items currently in use */
int g_Container_items_allocated = 0 ;	/* number of Ordered_container items currently allocated */