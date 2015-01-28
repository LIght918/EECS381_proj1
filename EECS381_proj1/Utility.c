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
#include <assert.h>
#include <ctype.h>

extern int g_string_memory; 

static void str_cpy(char *dst, const char* src );

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
    str_cpy( front_c_str, (char * )str_copy );
}

/* loads int a title from a file the c_str title is required to have enough space alloc
 returns 0 if read success full, nonzero if otherwize */
int get_title( FILE* infile, char* title)
{
    fgets( title, TITLE_ARRAY_SIZE, infile ) ;
    remove_white_space( title );

    return  strlen( title ) <= 0 ;
}

/* copies string from src to dst */
/* standard version wasn't working with an array */
/* TODO see if can be removed */ 
static void str_cpy(char *dst, const char* src )
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
    /*printf( "title: %s\n", (char*) arg_ptr );
    printf( "record: %s\n", get_Record_title( (struct Record* )data_ptr) );*/

	return strcmp( (char*) arg_ptr, get_Record_title( (struct Record* )data_ptr) ); 
}

int comp_Record_by_ID( const void* left, const void* right )
{
    return get_Record_ID( (struct Record* )left ) < get_Record_ID( (struct Record* )right );
}

int comp_Record_to_ID( const void* arg_ptr, const void* data_ptr )
{
    return ( get_Record_ID( (struct Record* )data_ptr ) -  *( (int*) arg_ptr ) );
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


/* print the corisponding error message to *
 * the error passed in                     */
void print_error( enum Error_e err  )
{
    switch ( err ) {
        case COMMAND:
            fprintf( stdout, "Unrecognized command!\n");
            clear_line();
            break;
        case DUPLICATE_REC:
            fprintf( stdout, "Library already has a record with this title!\n" );
            break;
        case DUPLICATE_COLL:
            fprintf( stdout,"Catalog already has a collection with this name!\n");
            break;
        case IN_COLL:
            fprintf( stdout,"Record is already a member in the collection!\n");
            break;
        case NOT_IN_COLL:
            fprintf( stdout,"Record is not a member in the collection!\n");
            break;
        case CANT_DELETE:
            fprintf( stdout,"Cannot delete a record that is a member of a collection!\n");
            break;
        case CLEAR_COLL:
            fprintf( stdout,"Cannot clear all records unless all collections are empty!\n");
            break;
        case NOT_FOUND_TITLE:
            fprintf( stdout,"No record with that title!\n");
            break;
        case NOT_FOUND_ID:
            fprintf( stdout,"No record with that ID!\n");
            break;
        case NOT_FOUND_COLL:
            fprintf( stdout,"No collection with that name!\n");
            break;
        case READ_TITLE:
            fprintf( stdout,"Could not read a title!\n");
            break;
        case READ_INT:
            fprintf( stdout,"Could not read an integer value!\n");
            clear_line();
            break;
        case RATING_RANGE:
            fprintf( stdout, "Rating is out of range!\n");
            break;
        case FILE_OPEN:
            fprintf( stdout,"Could not open file!\n");
            clear_line();
            break;
        case INVAL_DATA:
            fprintf( stdout,"Invalid data found in file!\n");
            break;
        case NONE: /* no error needs to be printed */
            break; 
        case ASSERT:
            assert(0); /* should cascade on NDEBUG */ 
        default:
            fprintf( stdout, "Error Unknow\n" );
            break;
    }
}

void* get_data_ptr( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum Error_e err )
{
    void* cur_node = get_node(c_ptr, fafp, data_ptr, err );

    if ( cur_node == NULL )
    {
        return NULL;
    }
    
    return OC_get_data_ptr( cur_node );
}

void* get_node(struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum Error_e err )
{
    void* cur_node = OC_find_item_arg( c_ptr, data_ptr, fafp );
    
    if ( cur_node == NULL )
    {
        print_error( err );
    }
    
    return cur_node;
}

/* init the static formated strings used for input to protect againts buffer overflow */ 
char* init_global_fstring( char* input, int buffer_size )
{
	sprintf( input, "%%%ds", buffer_size );	
	return input; 
}


