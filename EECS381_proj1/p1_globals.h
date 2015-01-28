/*
//  p1_globals.h
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#ifndef P1_GLOBALS_H
#define P1_GLOBALS_H

#define MAX_LENGTH_FSTRING 5 /* space needed for "%nns" and a null terminator where n is any num 0-9*/

extern int g_string_memory;			/* number of bytes used in C-strings */

extern int g_Container_count;		/* number of Ordered_containers currently allocated */
extern int g_Container_items_in_use;	/* number of Ordered_container items currently in use */
extern int g_Container_items_allocated;	/* number of Ordered_container items currently allocated */


#endif /* defined(__EECS381_proj1__p1_globals__) */
