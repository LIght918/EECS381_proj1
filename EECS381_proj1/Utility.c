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
#include "Ordered_container.h" /* TODO remove */
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

int comp_Record_to_title(const void* arg_ptr, const void* data_ptr)
{
    printf( "Test: %s\n", (char*) arg_ptr );
    print_Record( (struct Record* )data_ptr );
	return strcmp( get_Record_title( (struct Record* )data_ptr), (char*) arg_ptr ); 
}

int comp_Record_by_ID( const void* left, const void* right )
{
    return get_Record_ID( (struct Record* )left ) < get_Record_ID( (struct Record* )right );
}

int comp_Record_to_ID( const void* arg_ptr, const void* data_ptr )
{
    return ( get_Record_ID( (struct Record* )data_ptr ) -  *( (int*) arg_ptr ) );
}

int comp_Collection_by_name( const void* left, const void* right )
{
    return strcmp( get_Collection_name( (struct Collection* )left),
                   get_Collection_name( (struct Collection* )right ));
}

int comp_Collection_to_name(const void* arg_ptr, const void* data_ptr )
{
    return strcmp( arg_ptr, get_Collection_name( (struct Collection* ) data_ptr ) );
}

int is_Collection_not_empty( void* data_ptr )
{
    return !Collection_empty( ( struct Collection* ) data_ptr );
}


char* alloc_and_copy( const char* src )
{
    int length = (int)strlen( src ) + 1 ;
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
    /* add one to deal with the null termiator */ 
    g_string_memory -= (int)strlen( src ) + 1;
    free( src );
}

/* print the corisponding error message to *
 * the error passed in                     */
void print_error( enum error err  )
{
    switch ( err ) {
        case COMMAND:
            clear_line();
            fprintf( stderr, "Unrecognized command!\n");
            break;
        case DUPLICATE_REC:
            fprintf( stderr, "Library already has a record with this title!\n" );
            break;
        case DUPLICATE_COLL:
            fprintf( stderr,"Catalog already has a collection with this name!\n");
            break;
        case IN_COLL:
            fprintf( stderr,"Record is already a member in the collection!\n");
            break;
        case NOT_IN_COLL:
            fprintf( stderr,"Record is not a member in the collection!\n");
            break;
        case CANT_DELETE:
            fprintf( stderr,"Cannot delete a record that is a member of a collection!\n");
            break;
        case CLEAR_COLL:
            fprintf( stderr,"Cannot clear all records unless all collections are empty!\n");
            break;
        case NOT_FOUND_TITLE:
            fprintf( stderr,"No record with that title!\n");
            break;
        case NOT_FOUND_ID:
            fprintf( stderr,"No record with that ID!\n");
            break;
        case NOT_FOUND_COLL:
            fprintf( stderr,"No collection with that name!\n");
            break;
        case READ_TITLE:
            fprintf( stderr,"Could not read a title!\n");
            break;
        case READ_INT:
            clear_line();
            fprintf( stderr,"Could not read an integer value!\n");
            break;
        case RATING_RANGE:
            fprintf( stderr, "Rating is out of range!\n");
            break;
        case FILE_OPEN:
            fprintf( stderr,"Could not open file!\n");
            break;
        case INVAL_DATA:
            fprintf( stderr,"Invalid data found in file!\n");
            break;
        case NONE:
            break; 
        case ASSERT:
            assert(0); /* should cascade on NDEBUG */ 
        default:
            fprintf( stderr, "Error Unknow\n" );
            break;
    }
}

void* get_data_ptr( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum error err )
{
    void* cur_node = get_node(c_ptr, fafp, data_ptr, err );

    if ( cur_node == NULL )
    {
        return NULL;
    }
    
    return OC_get_data_ptr( cur_node );
}

