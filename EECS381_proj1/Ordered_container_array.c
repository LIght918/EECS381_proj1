/* skeleton file for Ordered_container_array.c
The struct declaration below must be used for the Ordered_container object.
Remove this comment and complete this file with all necessary code.
*/

#include "Ordered_container.h"
#include <stdlib.h>
#include <assert.h> /* TODO #DEFINE NDEBUG */
#include <stdio.h> /* TODO remove for release */

#define DEFAULT_ALLOCATION 3

/* A complete type declaration for Ordered_container implemented as an array */
struct Ordered_container {
	OC_comp_fp_t comp_fun;	/* pointer to comparison function  */
	void** array;			/* pointer to array of pointers to void */
	int allocation;			/* current size of array */
	int size;				/* number of items currently in the array */
};

/* init member variables to default values */
static void init_Order_containter( struct Ordered_container* c_ptr );
static void OC_grow( struct Ordered_container* c_ptr );
static void copy_array( void** array_old, void** array_new, int size );

struct Ordered_container* OC_create_container(OC_comp_fp_t f_ptr)
{
    struct Ordered_container* new_container = malloc( sizeof( struct Ordered_container ) );
    
    init_Order_containter( new_container );
    new_container->comp_fun = f_ptr;
    
    /* keep track of Containers */
    g_Container_count++;
    
    return new_container;
}

void OC_destroy_container(struct Ordered_container* c_ptr)
{
    /* take care of global vars */
    g_Container_count--;
    g_Container_items_in_use -= c_ptr->size;
    g_Container_items_allocated -= c_ptr->allocation;
    
    /* clean up memory */
    free( c_ptr->array );
    free( c_ptr );
}

void OC_clear(struct Ordered_container* c_ptr)
{
    /* take care of global vars */
    g_Container_items_in_use -= c_ptr->size;
    g_Container_items_allocated -= c_ptr->allocation;
    
    /* clean up memory */
    free( c_ptr->array );
    
    init_Order_containter( c_ptr );
}

int OC_get_size(const struct Ordered_container* c_ptr)
{
    return c_ptr->size;
}

int OC_empty(const struct Ordered_container* c_ptr)
{
    return !c_ptr->size;
}

void* OC_get_data_ptr(const void* item_ptr)
{
    /* cast to a void** and derefference */
    return *((void **)item_ptr);
}

/* Helper functions */
static void init_Order_containter( struct Ordered_container* c_ptr )
{
    c_ptr->array = malloc( sizeof( void* ) * DEFAULT_ALLOCATION );
    c_ptr->size = 0;
    c_ptr->allocation = DEFAULT_ALLOCATION;
    
    /* keep track of allocation */
    g_Container_items_allocated += DEFAULT_ALLOCATION;
}

static void** OC_search( const struct Ordered_container* c_ptr, OC_comp_fp_t f_ptr, const void* data_ptr )
{
    int left = 0;
    int mid = 0;
    int right = c_ptr->size;
    
    if ( right == 0 )
    {
        return c_ptr->array; 
    }
    
    /* check if it should be at the end */
    if ( f_ptr( c_ptr->array[ right - 1 ], data_ptr ) < 0)
    {
        return c_ptr->array + right;
    }
    if (f_ptr( c_ptr->array[ 0 ], data_ptr ) > 0 )
    {
        return c_ptr->array;
    }
    
    while ( left <= right )
    {
        int com_value;
        mid = left + ( right - left ) / 2;
        com_value =  f_ptr( data_ptr, c_ptr->array[ mid ]);
        if ( com_value == 0 )
        {
            return ( c_ptr->array + mid );
        }
        else if ( com_value < 0 )
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }

    if ( mid == 0)
    {
        mid = 1; 
    }
    /* return the location that the data should be in if it is not found */
    return ( c_ptr->array + mid );
}


static void OC_grow( struct Ordered_container* c_ptr )
{
    int new_allocation;
    void** new_array;
    
    new_allocation = 2 * ( c_ptr->allocation + 1 );
    new_array = malloc( sizeof( void* ) * new_allocation );
    
    /* copy over the memory */
    copy_array( c_ptr->array, new_array, c_ptr->size );
    
    /* take care of global */
    g_Container_items_allocated += new_allocation - c_ptr->allocation;
    
    c_ptr->array = new_array;
    c_ptr->allocation = new_allocation;
}

/* copies the old array to a new array
        requires that the array_new has enough space to hold array_old
        requires that size of old array be correct */
static void copy_array( void** array_old, void** array_new, int size )
{
    int i;
    for (i = 0; i < size; ++i )
    {
        *array_new = *array_old;
        array_new++;
        array_old++;
    }
}

void OC_delete_item(struct Ordered_container* c_ptr, void* item_ptr)
{
    void** node = (void**)item_ptr;
    /* find the new size and decrement the size member variable 
       subarray_size = total_size - size of the front array */
    int size_subarray = ( --c_ptr->size ) - (int)(  node - c_ptr->array );
    
    /* need to take care of globals */
    g_Container_items_in_use--;
    
    copy_array( node + 1, node, size_subarray );
}

void* OC_find_item(const struct Ordered_container* c_ptr, const void* data_ptr)
{
    void** node = OC_search( c_ptr, c_ptr->comp_fun, data_ptr );
    
    return ( c_ptr->comp_fun( data_ptr, *node ) == 0 ) ? node : NULL;
}

void OC_insert(struct Ordered_container* c_ptr, const void* data_ptr)
{
    int i, size_subarray;
    void** node;
    
    /* check if we have enough size for and insert */
    if ( c_ptr->allocation == c_ptr->size )
    {
        OC_grow( c_ptr );
    }
    
    /* find the location to insert */
    node = OC_search( c_ptr, c_ptr->comp_fun, data_ptr );
    
    size_subarray = c_ptr->size - (int)( node - c_ptr->array ) ;

    /* move the right part of the subarray on place to the right */
    /* copy_array( node, node + 1, size_subarray ); */
    for ( i = 0; i < size_subarray; ++i )
    {
        /*printf( "c_ptr->size - i == %d \n", c_ptr->size - i );*/
        c_ptr->array[ c_ptr->size - i ] = c_ptr->array[ c_ptr->size - 1 - i ];
    }
    
    c_ptr->size++;
    
    /* copy the data_ptr into the space we just made */ 
    *node = ( void* )data_ptr;
}


void* OC_find_item_arg(const struct Ordered_container* c_ptr, const void* arg_ptr, OC_find_item_arg_fp_t fafp)
{
    void** node = OC_search( c_ptr, fafp, arg_ptr );
    
    return ( fafp( arg_ptr, *node ) == 0 ) ? node : NULL;
}


void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp)
{
    int i;
    void** cur_node = c_ptr->array;
    
    for ( i = 0 ; i < c_ptr->size; ++i)
    {
        afp( *cur_node++ );
    }
}

int OC_apply_if(const struct Ordered_container* c_ptr, OC_apply_if_fp_t afp)
{
    int i;
    void** cur_node = c_ptr->array;
    
    for ( i = 0 ; i < c_ptr->size; ++i)
    {
        int value = afp( *cur_node++ );
        if ( value != 0 )
        {
            return value;
        }
    }
    
    return 0;
}

void OC_apply_arg(const struct Ordered_container* c_ptr, OC_apply_arg_fp_t afp, void* arg_ptr)
{
    int i;
    void** cur_node = c_ptr->array;
    
    for ( i = 0 ; i < c_ptr->size; ++i)
    {
        afp( *cur_node++, arg_ptr );
    }
}

int OC_apply_if_arg(const struct Ordered_container* c_ptr, OC_apply_if_arg_fp_t afp, void* arg_ptr)
{
    int i;
    void** cur_node = c_ptr->array;
    
    
    
    for ( i = 0 ; i < c_ptr->size; ++i)
    {
        int value = afp( *cur_node++, arg_ptr );
        
        printf( "%d\n", value );
        
        if ( value != 0 )
        {
            return value;
        }
    }
    
    return 0;
}




