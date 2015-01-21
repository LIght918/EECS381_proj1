/*
//  Record.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Record.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
int Record_ID_counter = 0;


struct Record* create_Record(const char* medium, const char* title)
{
    struct Record* new_Record = malloc( sizeof( struct Record ) );
    
    new_Record->ID = Record_ID_counter++;
    
    new_Record->rating = 0;
    
    new_Record->title = malloc( sizeof( char ) * strlen( title ) );
    strcpy( new_Record->title, title );
    
    new_Record->medium = malloc( sizeof( char ) * strlen( medium ) );
    strcpy( new_Record->medium, medium );
    
    return new_Record;
}

void destroy_Record(struct Record* record_ptr)
{
    free( record_ptr->title );
    free( record_ptr->medium);
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
    save_Record( record_ptr, stdout );
}

void save_Record(const struct Record* record_ptr, FILE* outfile)
{
    
    if ( record_ptr->rating == 0  )
    {
        fprintf( outfile, "%d: %s u %s\n", record_ptr->ID,
               record_ptr->medium, record_ptr->title );
    }
    else
    {
        fprintf( outfile, "%d: %s %d %s\n",  record_ptr->ID, record_ptr->medium,
               record_ptr->rating, record_ptr->title );
    }
}

void reset_Record_ID_counter(void)
{
    Record_ID_counter = 0;
}

struct Record* load_Record(FILE* infile)
{
    /*scanf( "%d", )*/
    
    return NULL;
}


