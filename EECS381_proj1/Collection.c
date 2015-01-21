/*
//  Collection.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Collection.h"
#include "Ordered_container.h"
#include "Record.h"
#include <stdlib.h>
#include <string.h>

/* a Collection contains a pointer to a C-string name and a container
 that holds pointers to Records - the members. */
struct Collection {
    char* name;
    struct Ordered_container* members;
};

/* uses strcmp on the title of each record and returns the value */
static int comp_Record( const void* left, const void* right );

struct Collection* create_Collection(const char* name)
{
    struct Collection* new_collection = malloc( sizeof( struct Collection ) );
    
    /* alloc mem for name and copy it over */
    new_collection->name = malloc( sizeof(char) * strlen( name ) );
    strcpy( new_collection->name, name);
    
    /* use strcpy as the comp func */
    new_collection->members = OC_create_container( comp_Record );
    
    return new_collection;
}


void destroy_Collection(struct Collection* collection_ptr)
{
    free( collection_ptr->name );
    OC_destroy_container( collection_ptr->members );
    free( collection_ptr );
}

const char* get_Collection_name(const struct Collection* collection_ptr)
{
    return collection_ptr->name;
}


int Collection_empty(const struct Collection* collection_ptr)
{
    return OC_empty( collection_ptr->members );
}

int add_Collection_member(struct Collection* collection_ptr, const struct Record* record_ptr)
{
    if ( !OC_find_item( collection_ptr->members, record_ptr ))
    {
        /* if the record is not found insert it and return 0 */
        OC_insert( collection_ptr->members, record_ptr );
        return 0;
    }
    
    return 1;
}

int is_Collection_member_present(const struct Collection* collection_ptr, const struct Record* record_ptr)
{
    return (int)OC_find_item( collection_ptr->members, record_ptr );
}

int remove_Collection_member(struct Collection* collection_ptr, const struct Record* record_ptr)
{
    void* record_to_rm = OC_find_item( collection_ptr->members, record_ptr );
    OC_delete_item( collection_ptr->members, record_to_rm );
    return (int)OC_find_item( collection_ptr->members, record_ptr );
}

void print_Collection(const struct Collection* collection_ptr)
{
    OC_apply( collection_ptr->members, ( void(*)( void * ))print_Record );
}

void save_Collection(const struct Collection* collection_ptr, FILE* outfile)
{
    OC_apply_arg(collection_ptr->members, ( void(*)( void * ,void*))save_Record, outfile );
}

static int comp_Record( const void* left, const void* right )
{
    return strcmp( get_Record_title((struct Record* )left),
                  get_Record_title((struct Record* )right) );
}
