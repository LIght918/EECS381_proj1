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
#include "Ordered_container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NDEBUG
#include <assert.h>
#include <ctype.h>

extern int g_string_memory; 

/* removes redundent white space from the given string */
static void remove_white_space( char* c_str );

void* safe_malloc( size_t size )
{
    void* new_mem = malloc( size );
    if ( new_mem == NULL )
    {
        /* if memory allocation failed terminate
         the program and return 1 */
        exit(1);
    }
    return new_mem;
}


/* removes redundent white space from the given string */
static void remove_white_space( char* c_str )
{
    int i = 0;
    char* front_c_str = c_str;
    int last_char_was_white = 0;
    char str_copy[ TITLE_ARRAY_SIZE ];
    
    /* trim all the leading white space */
    while ( isspace( *c_str ))
    {
        c_str++;
    }
    
    while ( *c_str != '\0' )
    {
        if ( isspace( *c_str ) ) {
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
    while ( isspace( str_copy[ --i ] )) { ; }
    
    str_copy[ ++i ] = '\0';
    
    /* copy the mem back over into the starting location */
    strcpy( front_c_str, (char * )str_copy );
}

/* loads int a title from a file the c_str title is required to have enough space alloc
 returns 0 if read success full, nonzero if otherwize */
int get_title( FILE* infile, char* title)
{
    char* val = fgets( title, TITLE_ARRAY_SIZE, infile ) ;
    remove_white_space( title );

    return ( strlen( title ) <= 0 ) || !val ;
}

int comp_Record_by_title( const void* left, const void* right )
{
                  
    return strcmp( get_Record_title( (struct Record* )left),
                   get_Record_title( (struct Record* )right) );
}

int comp_Record_to_title(const void* arg_ptr, const void* data_ptr)
{
    /*printf( "title: %s\n", (char*) arg_ptr );
    printf( "record: %s\n", get_Record_title( (struct Record* )data_ptr) );*/

	return strcmp( (char*) arg_ptr, get_Record_title( (struct Record* )data_ptr) ); 
}

/* allocates memory and copyes the src string into that
 * and keeps track of the global allocations
 * returns NULL if there is a allocation error
 */
char* alloc_and_copy( const char* src )
{
    int length = (int)strlen( src ) + 1 ;
    char* new_str = safe_malloc( sizeof(char) * length );
    
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
    /* add one to deal with the null termiator */ 
    g_string_memory -= (int)strlen( src ) + 1;
    free( src );
}

/* clears the input buffer on an error */
void clear_line( void )
{
    char buffp[ TITLE_ARRAY_SIZE ];
    fgets( buffp , TITLE_MAX_BUFF_SIZE, stdin );
}

void* get_data_ptr( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr )
{
    void* cur_node = OC_find_item_arg(c_ptr, data_ptr, fafp );

    if ( cur_node == NULL )
    {
        return NULL;
    }
    
    return OC_get_data_ptr( cur_node );
}

