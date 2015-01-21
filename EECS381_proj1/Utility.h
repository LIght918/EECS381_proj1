/*
//  Utility.h
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#ifndef EECS381_proj1__Utility__
#define EECS381_proj1__Utility__

#include <stdio.h>

/* returns true if the char c is a space a newline or a tab
 false if otherwize */
int is_white_space( char c );

/* removes redundent white space from the given string */
void remove_white_space( char* c_str );

/* loads int a title from a file the c_str title is required to have enough space alloc
    returns true if read success full, false if otherwize */
int get_title( FILE* infile, char* title);

/* local version of strcpy */ 
void str_cpy(char *dst, const char *src);

#endif /* defined(__EECS381_proj1__Utility__) */
