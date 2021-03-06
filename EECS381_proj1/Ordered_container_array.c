
#include "Ordered_container.h"
#include "Utility.h"
#include <stdlib.h>
#define NDEBUG
#include <assert.h>


#define DEFAULT_ALLOCATION 3

/* A complete type declaration for Ordered_container implemented as an array */
struct Ordered_container {
    OC_comp_fp_t comp_fun;	/* pointer to comparison function  */
    void** array;			/* pointer to array of pointers to void */
    int allocation;			/* current size of array */
    int size;				/* number of items currently in the array */
};


int g_Container_count = 0 ;		/* number of Ordered_containers currently allocated */
int g_Container_items_in_use = 0 ;	/* number of Ordered_container items currently in use */
int g_Container_items_allocated = 0 ;	/* number of Ordered_container items currently allocated */


/* init member variables to default values */
static void init_Order_containter( struct Ordered_container* c_ptr );
static void OC_grow( struct Ordered_container* c_ptr );
static void copy_array( void** array_old, void** array_new, int size );
static void** OC_search_for_insert( const struct Ordered_container* c_ptr, OC_comp_fp_t f_ptr, const void* arg_ptr );
static void* OC_bsearch( const struct Ordered_container* c_ptr, OC_comp_fp_t f_ptr, const void* arg_ptr );

struct Ordered_container* OC_create_container(OC_comp_fp_t f_ptr)
{
    struct Ordered_container* new_container = safe_malloc( sizeof( struct Ordered_container ) );
    
    init_Order_containter( new_container );
    new_container->comp_fun = f_ptr;
    
    /* keep track of Containers */
    g_Container_count++;
    
    return new_container;
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

static void init_Order_containter( struct Ordered_container* c_ptr )
{
    /* set the values to their default values */
    c_ptr->size = 0;
    c_ptr->allocation = DEFAULT_ALLOCATION;
    
    c_ptr->array = safe_malloc( sizeof( void* ) * DEFAULT_ALLOCATION );
    
    /* keep track of allocation */
    g_Container_items_allocated += DEFAULT_ALLOCATION;
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

void OC_insert(struct Ordered_container* c_ptr, const void* data_ptr)
{
    int i, size_subarray;
    void** node;
    
    /* check if we have enough size for and insert */
    if ( c_ptr->allocation == c_ptr->size )
        OC_grow( c_ptr );
    
    /* find the location to insert */
    node = OC_search_for_insert( c_ptr, c_ptr->comp_fun, data_ptr );
    
    size_subarray = c_ptr->size - (int)( node - c_ptr->array ) ;
    
    /* move the right part of the subarray on place to the right */
    for ( i = 0; i < size_subarray; ++i )
    {
        c_ptr->array[ c_ptr->size - i ] = c_ptr->array[ c_ptr->size - 1 - i ];
    }
    
    c_ptr->size++;
    g_Container_items_in_use++;
    
    /* copy the data_ptr into the space we just made */
    *node = ( void* )data_ptr;
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

static void OC_grow( struct Ordered_container* c_ptr )
{
    int new_allocation;
    void** new_array;
    
    /* create a new array with double the size we currently need */
    new_allocation = 2 * ( c_ptr->allocation + 1 );
    new_array = safe_malloc( sizeof( void* ) * new_allocation );
    
    /* copy over the memory */
    copy_array( c_ptr->array, new_array, c_ptr->size );
    
    /* take care of global */
    g_Container_items_allocated += new_allocation - c_ptr->allocation;
    
    /* we don't need the old array any more */
    free( c_ptr->array );
    
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
        ++array_new;
        ++array_old;
    }
}

static void** OC_search_for_insert( const struct Ordered_container* c_ptr, OC_comp_fp_t f_ptr, const void* arg_ptr )
{
    int left = 0 ;
    int right;
    int mid = 0;
    
    if ( OC_empty( c_ptr ) )
    {
        /* if its empty just return the first element in the array */
        return c_ptr->array;
    }
    
    right = c_ptr->size - 1;
    
    while( left <= right )
    {
        int com_value;
        
        mid = ( right + left ) / 2 ;
        
        com_value = f_ptr( arg_ptr, c_ptr->array[ mid ] );
        
        if ( com_value > 0 ) {
            
            left = mid + 1 ;
            
            if ( left > right ) {
                return c_ptr->array + mid + 1;
            }
        }
        else
        {
            if ( left == mid )
            {
                return c_ptr->array + mid ;
            }
            right = mid - 1;
        }
        
    }
    
    return c_ptr->array + mid ;
}

void* OC_find_item_arg(const struct Ordered_container* c_ptr, const void* arg_ptr, OC_find_item_arg_fp_t fafp)
{
    return OC_bsearch(c_ptr, fafp, arg_ptr );
}


void* OC_find_item(const struct Ordered_container* c_ptr, const void* data_ptr)
{
    return OC_bsearch( c_ptr, c_ptr->comp_fun, data_ptr );
}

static void* OC_bsearch( const struct Ordered_container* c_ptr, OC_comp_fp_t f_ptr, const void* arg_ptr )
{
    int left = 0 ;
    int right;
    int mid = 0;
    
    if ( OC_empty( c_ptr ) )
        return NULL;
    
    right = c_ptr->size - 1;
    
    while( left <= right )
    {
        int com_value;
        
        mid = ( right + left ) / 2 ;
        
        com_value = f_ptr( arg_ptr, c_ptr->array[ mid ] );
        
        if ( com_value == 0 )
        {
            return c_ptr->array + mid ;
        }
        if ( com_value < 0 )
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
        
    }
    
    return NULL;
}

void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp)
{
    int i;
    for ( i = 0 ; i < c_ptr->size; ++i)
    {
        afp( c_ptr->array[ i ] );
    }
}

int OC_apply_if(const struct Ordered_container* c_ptr, OC_apply_if_fp_t afp)
{
    int i;
    
    for ( i = 0 ; i < c_ptr->size; ++i)
    {
        int value = afp( c_ptr->array[ i ] );
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
    
    for ( i = 0 ; i < c_ptr->size; ++i)
    {
        afp( c_ptr->array[ i ], arg_ptr );
    }
}

int OC_apply_if_arg(const struct Ordered_container* c_ptr, OC_apply_if_arg_fp_t afp, void* arg_ptr)
{
    int i;
    
    for ( i = 0 ; i < c_ptr->size; ++i)
    {
        int value = afp( c_ptr->array[ i ], arg_ptr );
        if ( value != 0 )
        {
            return value;
        }
    }
    
    return 0;
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
    /* cast to a void** and dereference */
    return *((void **)item_ptr);
}
