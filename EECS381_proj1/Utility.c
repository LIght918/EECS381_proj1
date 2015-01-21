/*
//  Utility.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Utility.h"
#include "p1_globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_STRING_LENGTH 100

/* removes redundent white space from the given string */
void remove_white_space( char* c_str )
{
    
    int i = 0;
    char* front_c_str = c_str;
    int last_char_was_white = 0;
    char str_copy[ MAX_STRING_LENGTH ];
    
    while ( is_white_space( *c_str )) 
    {
        c_str++;
    }
    
    printf( "%s\n", c_str );
    
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
    
    printf("i: %i\n", i );
    while ( is_white_space( str_copy[ --i ] )) {
        ;
    }
    
    str_copy[ ++i ] = '\0';
    printf( "%s\n", str_copy );
    
    /* why whould this give a seg fault */
    str_cpy( front_c_str, (char * )str_copy );
    
    printf( "%s\n", str_copy );
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
    int temp;
    int i = 0;
    do {
        temp = fgetc( infile );
        *title++ = temp;
        if ( i++ >= TITLE_MAX_BUFF_SIZE )
        {
            /* buffer overflow so throw an error */
            return 0;
        }
        /* read in will temp is not a end line or EOF  */
        /* should an error be throw in EOF reached? */ 
    } while ( temp != '\n' && temp > 0 ) ;
    
    /* clean up the garbage char that was put in on the 
     last iteration through the loop and give it the null terminator */
    *(--title) = '\0';
    
    return 1;
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