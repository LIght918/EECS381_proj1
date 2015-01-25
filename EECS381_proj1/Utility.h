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

/* size limits */
#define NAME_MAX_SIZE 15
#define MEDIUM_MAX_SIZE 7
#define FILENAME_MAX_SIZE 31
#define TITLE_MAX_BUFF_SIZE 64

/* array sizes */
#define NAME_ARRAY_SIZE      NAME_MAX_SIZE + 1
#define MEDIUM_ARRAY_SIZE    MEDIUM_MAX_SIZE + 1
#define FILENAME_ARRAY_SIZE  FILENAME_MAX_SIZE + 1
#define TITLE_ARRAY_SIZE     TITLE_MAX_BUFF_SIZE + 1

/* returns true if the char c is a space a newline or a tab
 false if otherwize */
int is_white_space( char c );

/* removes redundent white space from the given string */
void remove_white_space( char* c_str );

/* loads int a title from a file the c_str title is required to have enough space alloc
    returns true if read successfull, false if otherwize */
int get_title( FILE* infile, char* title);

/* local version of strcpy */ 
void str_cpy(char *dst, const char *src);

/* uses strcmp on the title of each record and returns the value */
int comp_Record_by_title( const void* left, const void* right );

/* uses < on the ID values of each records */
int comp_Record_by_ID( const void* left, const void* right );

/* compares a record to a title returns ture if equal */ 
int comp_Record_to_title(const void* arg_ptr, const void* data_ptr);

/* compars a record to a ID, int value returns true if equal */
int comp_Record_to_ID( const void* arg_ptr, const void* data_ptr );

/* uses strcmp on the name of each Collection and returns the value */
int comp_Collection_by_name( const void* left, const void* right );

/* allocate memory and copy the src string to it */ 
char* alloc_and_copy( const char* src );

/* clean up mememory and keep track of allocation */
void free_string( char* src );

#endif /* defined(__EECS381_proj1__Utility__) */
