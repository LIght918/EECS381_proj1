/*
//  Ordered_container_test.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/14/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Ordered_container.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* function prototypes */
void print_as_string(char* data_ptr);
int compare_string(const char* data_ptr1, const char* data_ptr2);

void find_and_remove(struct Ordered_container * container, char* probe);

int right_form( char* right );

void demo_func(void* data_ptr, void* arg);

int main( void )
{
    int i; 
    int cur_size_container = 0;
    
    char* test_string[] = { "t5", "t6", "t3", "t9", "t7", "t1", "t2" };
    int size_test = 7;
    
    char* s1 = "s1";
    /*char* s2 = "s2";
    char* s3 = "s3";
    char* s4 = "s4";
    char* s5 = "s5";
    char* s6 = "s6";
    
    char* probe1 = "s3";
    char* probe2 = "s99";
    
    char* probe3 = "t2";
    char* probe4 = "t11";*/
    
    void* node;
    
    struct Ordered_container * container;

    printf("Starting test\nCreating\n" );

    /* create the container */
    container = OC_create_container((int (*)(const void*, const void*))compare_string);

    for (  i = 0 ; i < size_test ; ++i )
    {

        printf("Inserting %s\n", test_string[ i ] );
        OC_insert( container, test_string[ i ] );
        cur_size_container = OC_get_size( container );
        printf("\tsize is %d\n", cur_size_container );
        /* make sure it has the correct size */
        assert( ( i + 1 ) == cur_size_container );
        
        if ( /*( i % 3 ) == 1 */ 1 )
        {
            OC_apply( container, (void (*)(void*))print_as_string) ;
        }
    }
    
    assert( OC_apply_if( container, (int (*)(void*))right_form ) == 0  );
    
    printf( "Inserting s1\n " );
    OC_insert( container, s1 );
    OC_apply( container, (void (*)(void*))print_as_string) ;
    
    assert( OC_apply_if( container, (int (*)(void*))right_form ) == ('s' - 't' ) );
    
    node = OC_find_item( container, s1 );
    
    OC_apply( container, (void (*)(void*))print_as_string) ;
    
    /* check the correct pointer is returned */
    printf( "\nAfter adding s1\n" );
    assert( OC_get_data_ptr( node ) == s1 );
    
    /* check to make sure delete is working */
    OC_delete_item( container, node );
    assert( OC_get_size( container ) == size_test );


    /* check clear function */
    OC_clear( container );
    assert( OC_empty( container ) != 0 );
    

    
    /* rebuild array to run more tests */
    for (  i = 0 ; i < size_test ; ++i )
    {
        OC_insert( container, test_string[ i ] );
        cur_size_container = OC_get_size( container );
        /* make sure it has the correct size */
        assert( ( i + 1 ) == cur_size_container );
    }

    /* removing one element at a time */
    for( i = 0 ; i < size_test ; ++i )
    {
        printf( "%d: removing %s\n", i , test_string[i] );
        OC_apply( container, (void (*)(void*))print_as_string) ;
        find_and_remove( container, test_string[i] );
        assert( ( size_test - i - 1 ) == OC_get_size( container ) );
    }
    
    /* clean up memory */
    /* run with valgrind to ensure no mem leaks */
    OC_destroy_container( container );

    return 0;
}


int compare_string( const char* data_ptr1, const char* data_ptr2)
{
    return strcmp(data_ptr1, data_ptr2);
}

void find_and_remove(struct Ordered_container * container, char* probe)
{
    void* found_item;
    printf("\nsearch for %s:\n", probe);
    found_item = OC_find_item(container, probe);
    if(found_item) {
        printf("found item points to %s\n", (char*)OC_get_data_ptr(found_item));
        OC_delete_item(container, found_item);
        printf("item removed\n");
        /* found_item now points to an undefined value - it is invalid */
    }
    else
    {
        printf("probed item not found\n");
    }
}

void demo_func(void* data_ptr, void* arg)
{
    int* int_ptr = (int*)arg;
    if(*int_ptr % 2) /* is the arg even or odd? */
        printf("I like this item: %s\n", (char*)data_ptr);
    else
        printf("I hate this item: %s\n", (char*)data_ptr);
}

void print_as_string(char* data_ptr)
{
    printf("\t%s\n", data_ptr);
}

int right_form( char* right )
{
    return right[0] - 't';
}

