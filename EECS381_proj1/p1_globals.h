/*
//  p1_globals.h
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#ifndef EECS381_proj1__p1_globals__
#define EECS381_proj1__p1_globals__

/* size limits */
#define NAME_MAX_SIZE 15
#define MEDIUM_MAX_SIZE 7
#define FILENAME_MAX_SIZE 31
#define TITLE_MAX_BUFF_SIZE 64



/* Do I need this one? */ 
int g_string_memory;			/* number of bytes used in C-strings */

int g_Container_count;		/* number of Ordered_containers currently allocated */
int g_Container_items_in_use;	/* number of Ordered_container items currently in use */
int g_Container_items_allocated;	/* number of Ordered_container items currently allocated */

#endif /* defined(__EECS381_proj1__p1_globals__) */
