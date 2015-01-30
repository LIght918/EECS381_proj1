/*
//  Record.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Record.h"
#include "Utility.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define NDEBUG
#include <assert.h>

/* skeleton file for Record.c
 The struct declaration below must be used for Record objects.
 Remove this comment and complete this file with all necessary code.
 */

/* a Record contains an int ID, rating, and pointers to C-strings for the title and medium */
struct Record {
    char* title;
    int ID;
    char* medium;
    int rating;
};

/* local global */
static int Record_ID_counter = 1;

struct Record* create_Record(const char* medium, const char* title)
{
    struct Record* new_Record = safe_malloc( sizeof( struct Record ) );
    
    new_Record->ID = Record_ID_counter++;
    new_Record->rating = 0;
    new_Record->title = alloc_and_copy( title );
    new_Record->medium = alloc_and_copy( medium );
    
    return new_Record;
}

void destroy_Record(struct Record* record_ptr)
{
    free_string( record_ptr->title );
    free_string( record_ptr->medium);
    free( record_ptr );
}

int get_Record_ID(const struct Record* record_ptr)
{
    return record_ptr->ID;
}

const char* get_Record_title(const struct Record* record_ptr)
{
    return record_ptr->title;
}

void set_Record_rating(struct Record* record_ptr, int new_rating)
{
    record_ptr->rating = new_rating;
}

void print_Record(const struct Record* record_ptr)
{
    if ( record_ptr->rating == 0 )
    {
        printf( "%d: %s u %s\n", record_ptr->ID,
                    record_ptr->medium, record_ptr->title );
    }
    else
    {
        printf( "%d: %s %d %s\n",  record_ptr->ID, record_ptr->medium,
                    record_ptr->rating, record_ptr->title );
    }
}

void save_Record(const struct Record* record_ptr, FILE* outfile)
{

    fprintf( outfile, "%d %s %d %s\n",  record_ptr->ID, record_ptr->medium,
                    record_ptr->rating, record_ptr->title );
}

void reset_Record_ID_counter(void)
{
    Record_ID_counter = 1;
}

struct Record* load_Record(FILE* infile)
{
    struct Record* new_record;
    int ID, rating;
    char medium[ MEDIUM_MAX_SIZE ];
    char title [ TITLE_MAX_BUFF_SIZE ];
    
    /* read in from the file making sure not to overflow the buffer */
    if ( fscanf( infile, "%d %" STRINGIFY( MEDIUM_MAX_SIZE )"s %d\n", &ID, medium, &rating ) != 3 )
        return NULL;
    
    if ( get_title( infile, title ) )
        return NULL;
    
    new_record = create_Record( medium, title );
    
    /* update it's member vars from the defaults to the input values */
    new_record->rating = rating;
    new_record->ID = ID;
    
    /* set Record ID to the max value we have seen loading from the file
       and decrement it back to what it was before create_Record() was called */
    
    if( ID > Record_ID_counter )
        Record_ID_counter = ID + 1;
    else
       Record_ID_counter--;
    
    return new_record;
}


