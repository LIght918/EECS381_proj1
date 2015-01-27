/*
//  Collection.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Collection.h"
#include "Ordered_container.h"
#include "Utility.h"
#include "Record.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/* #define NDEBUG */ /* include that on release */ 

/* a Collection contains a pointer to a C-string name and a container
 that holds pointers to Records - the members. */
struct Collection {
    char* name;
    struct Ordered_container* members;
};

struct Collection* create_Collection(const char* name)
{
    struct Collection* new_collection = malloc( sizeof( struct Collection ) );
    
    /* alloc mem for name and copy it over */
    new_collection->name = alloc_and_copy( name ); 
    

    
    /* use strcpy as the comp func */
    new_collection->members = OC_create_container( comp_Record_by_title );
    
    return new_collection;
}


void destroy_Collection(struct Collection* collection_ptr)
{
    free_string( collection_ptr->name );
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
    
    /* can I pass a NULL to OC_delete */
    if ( record_to_rm )
    {
        OC_delete_item( collection_ptr->members, record_to_rm );
        return 0;
    }
    
    return 1;
}

void print_Collection(const struct Collection* collection_ptr)
{
    printf( "Collection %s contains:", collection_ptr->name );
    
    if ( OC_empty( collection_ptr->members ) )
    {
        /* if empty print none */ 
        printf(" None" );
    }
    
    printf("\n" );
    
    OC_apply( collection_ptr->members, ( void(*)( void * ))print_Record );
}

void save_Collection(const struct Collection* collection_ptr, FILE* outfile)
{
    /* output the name and number of records in the Collection */
    fprintf( outfile, "%s%d", collection_ptr->name, OC_get_size( collection_ptr->members ) );
    /* loop through the whole collection outputing the title to the file */
    OC_apply_arg( collection_ptr->members, ( void(*)( void * ,void*))save_Record, outfile );
}

struct Collection* load_Collection(FILE* input_file, const struct Ordered_container* records)
{
    /* what do I do with records */
    char name[ NAME_MAX_SIZE ];
    char title[ TITLE_MAX_BUFF_SIZE ];
    int  i, num_records;
    struct Collection* new_collection;
    void* cur_record = NULL;
    
    /* read in the name of the collection and the number of records*/
    if ( fscanf( input_file, "%s %d\n", name, &num_records ) != 2 )
    {
        assert(0);
        return NULL;
    }
    
    printf("%s %d\n", name, num_records );
    
    /* read int the new line */ 
    /*fget( input_file ); */

    new_collection = create_Collection( name );
    
    for ( i = 0; i < num_records; ++i)
    {
        int val = get_title( input_file, title ); 
        cur_record = OC_find_item_arg( records, (void*)title, comp_Record_to_title );

        /* read in the title and then check if it is in records */
        if ( ( cur_record == NULL ) || val )
        {
            printf("title: %s\n", title );
            
            /* if given bad input clean up mem and return NULL */
            destroy_Collection( new_collection );
            return NULL;
        }
        
        printf("its is: %s\n", title );
        print_Record( cur_record );

        OC_insert( new_collection->members, cur_record );
    }
    
    return new_collection;
}
