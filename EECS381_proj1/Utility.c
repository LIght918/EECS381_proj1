/*
//  Utility.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Utility.h"
#include "Record.h"
#include "Collection.h"
#include "p1_globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* removes redundent white space from the given string */
void remove_white_space( char* c_str )
{
    
    int i = 0;
    char* front_c_str = c_str;
    int last_char_was_white = 0;
    char str_copy[ TITLE_ARRAY_SIZE ];
    
    /* trim all the leading white space */
    while ( is_white_space( *c_str )) 
    {
        c_str++;
    }
    
    while ( *c_str != '\0' )
    {
        if ( is_white_space( *c_str ) ) {
            if ( !last_char_was_white )
            {
                str_copy[ i++ ] = *c_str;
            }
            last_char_was_white = 1;
        }
        else
        {
            last_char_was_white = 0;
            str_copy[ i++ ] = *c_str;
        }
        c_str++;
    }
    
    /* trim all the trailing white space */
    while ( is_white_space( str_copy[ --i ] )) { ; }
    
    str_copy[ ++i ] = '\0';
    
    /* copy the mem back over into the starting location */
    str_cpy( front_c_str, (char * )str_copy );
}

/* returns true if the char c is a space a newline or a tab 
   false if otherwize */
int is_white_space( char c )
{
    return ( c == ' ' ) || ( c == '\n') || (c == '\t' );
}

/* loads int a title from a file the c_str title is required to have enough space alloc
 returns true if read success full, false if otherwize */
int get_title( FILE* infile, char* title)
{
    
    int val = !fgets( title, TITLE_ARRAY_SIZE, infile ) ;
    remove_white_space( title );
    return val;
}

void str_cpy(char *dst, const char* src )
{
    int i = 0;
    assert( src );
    assert( dst );
    
    while ( src[ i ] != '\0')
    {
        *dst++ = src[ i++ ];
    }
    *dst = '\0';
}

int comp_Record_by_title( const void* left, const void* right )
{
    return strcmp( get_Record_title( (struct Record* )left),
                   get_Record_title( (struct Record* )right) );
}

int comp_Record_by_ID( const void* left, const void* right )
{
    return get_Record_ID( (struct Record* )left ) < get_Record_ID( (struct Record* )right );
}

int comp_Collection_by_name( const void* left, const void* right )
{
    return strcmp( get_Collection_name( (struct Collection* )left),
                   get_Collection_name( (struct Collection* )right ));
}

char* alloc_and_copy( const char* src )
{
    int length = (int)strlen( src );
    char* new_str = malloc( sizeof(char) * length );
    
    if ( !new_str )
    {
        /* if falure of allocation return NULL */
        return NULL;
    }
    strcpy( new_str, src );
    g_string_memory += length ;
    
    return new_str;
}

void free_string( char* src )
{
    g_string_memory -= (int)strlen( src );
    free( src );
}


