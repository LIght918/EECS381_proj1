#include "Ordered_container.h"
#include "Utility.h"
#include <stdlib.h>
#define NDEBUG
#include <assert.h>
#include <stdio.h>

/* struct LL_Node structure declaration. This declaration is local to this file. 
This is a two-way or doubly-linked list. Each node has a pointer to the previous 
node and another pointer to the next node in the list. This means insertions or
removals of a node can be made in constant time, once the location has been
determined. */
struct LL_Node
{ 
    struct LL_Node* prev;      /* pointer to the previous node */
	struct LL_Node* next;		/* pointer to the next node */
	void* data_ptr; 			/* uncommitted pointer to the data item */
};

/* Declaration for Ordered_container. This declaration is local to this file.  
A pointer is maintained to the last node in the list as well as the first, 
meaning that additions to the end of the list can be made in constant time. 
The number of nodes in the list is kept up-to-date in the size member
variable, so that the size of the list can be accessed in constant time. */
struct Ordered_container 
{
	OC_comp_fp_t comp_func;
	struct LL_Node* first;
	struct LL_Node* last;
	int size;
};


int g_Container_count = 0 ;		/* number of Ordered_containers currently allocated */
int g_Container_items_in_use = 0 ;	/* number of Ordered_container items currently in use */
int g_Container_items_allocated = 0 ;	/* number of Ordered_container items currently allocated */

/* init member variables in OC to default values */
static void init_Order_container( struct Ordered_container* c_ptr );

/* updates the global vars keeping track of the count of items used and allocated */
static void update_g_item_count( int num );

void OC_destroy_container(struct Ordered_container* c_ptr)
{
    /* clean up items in container */
    OC_clear( c_ptr );
    
    free( c_ptr );
    
    /* keep track of the num of containers */
    g_Container_count--;
}

struct Ordered_container* OC_create_container(OC_comp_fp_t f_ptr)
{
	struct Ordered_container* new_container;

	new_container = ( struct Ordered_container* ) safe_malloc( sizeof( struct Ordered_container ) ); 

	new_container->comp_func = f_ptr;
    init_Order_container( new_container );

    /* keep track of the num of containers */
    g_Container_count++;
    
	return new_container;
}

void OC_clear(struct Ordered_container* c_ptr)
{
    struct LL_Node* cur_node;
    struct LL_Node* prev_node = NULL;
    int i = 0 ;
    int size;
    
    size = c_ptr->size;
    cur_node = c_ptr->first;
    
    /* loop over the whole container freeing memory */
    for ( i = 0 ; i < size ; ++i )
    {
        prev_node = cur_node ;
        cur_node = cur_node->next ;
        
        free( prev_node );
    }
    
    /* reset the member var */
    init_Order_container( c_ptr );
    update_g_item_count( (-1)*size );
}

static void init_Order_container( struct Ordered_container* c_ptr )
{
    /* set member vars to default values */
    c_ptr->first = NULL;
    c_ptr->last = NULL;
    c_ptr->size = 0;
}

void OC_delete_item(struct Ordered_container* c_ptr, void* item_ptr)
{
    struct LL_Node* node_to_remove = ( struct LL_Node* )item_ptr;
    
    /* connect the next and prev nodes */
    if ( node_to_remove->next )
    {
        node_to_remove->next->prev = node_to_remove->prev;
    }
    if ( node_to_remove->prev )
    {
        node_to_remove->prev->next = node_to_remove->next;
    }
    
    /* check if we have invalidated either or both of the first or last pointers
     and update them if need be */
    if ( c_ptr->first == node_to_remove )
    {
        c_ptr->first = node_to_remove->next;
    }
    if ( c_ptr->last == node_to_remove )
    {
        c_ptr->last = node_to_remove->prev;
    }
    
    /* is the data_ptr dynamic */
    free( node_to_remove );
    c_ptr->size--;
    update_g_item_count( -1 );
}

/* look at this func again */
void OC_insert(struct Ordered_container* c_ptr, const void* data_ptr)
{
    struct LL_Node* cur_node = c_ptr->first;
    struct LL_Node* new_node = safe_malloc( sizeof( struct LL_Node ) );
    
    new_node->data_ptr = (void*)data_ptr;
    c_ptr->size++ ;
    update_g_item_count( 1 );
    
    /* if the container is empty add it as first and last */
    if ( c_ptr->first == NULL )
    {
        c_ptr->first = new_node;
        c_ptr->last  = new_node;
        new_node->prev = NULL;
        new_node->next = NULL;
    }
    else if ( c_ptr->comp_func( data_ptr, cur_node->data_ptr ) < 0 )
    {
        /* check to see if it should be the first node */
        
        c_ptr->first = new_node;
        
        cur_node->prev = new_node;
        new_node->next = cur_node;
        new_node->prev = NULL;
    }
    else
    {
        /* its ok to increment it first time since we know it
         isn't the first element */
        while ( ( cur_node = cur_node->next ) )
        {
            /* if the new data should be on the left of cur_node
             insert it on the left */
            if ( c_ptr->comp_func( data_ptr, cur_node->data_ptr ) < 0  )
            {
                cur_node->prev->next = new_node;
                
                new_node->prev = cur_node->prev;
                cur_node->prev = new_node;
                new_node->next = cur_node;
                return ;
            }
        }
        
        /* if we make it out of the loop we know it must go at the end */
        cur_node = c_ptr->last ;
        c_ptr->last = new_node;
        
        cur_node->next = new_node;
        new_node->prev = cur_node;
        new_node->next = NULL;
    }
}

static void update_g_item_count( int num )
{
    /* make sure this works */
    g_Container_items_allocated += num;
    g_Container_items_in_use += num;
}

/* using a linear search due to this being a linked list O(n) */
void* OC_find_item(const struct Ordered_container* c_ptr, const void* data_ptr)
{
    struct LL_Node* cur_node;
    int comp_value;
    
    if ( c_ptr == NULL )
    {
        /* throw error */
        return NULL;
    }
    
    cur_node = c_ptr->first;
    
    
    while ( cur_node != NULL )
    {
        
        comp_value = c_ptr->comp_func( data_ptr, cur_node->data_ptr );
        
        if ( comp_value == 0  )
        {
            /* if we have found it just return */
            return cur_node;
        }
        else if ( comp_value < 0 )
        {
            return NULL;
        }
        
        /* increment the pointer */
        cur_node = cur_node->next;
    }
    
    /* if we make it out of the loop the date_ptr was not in the container */
    return NULL;
}

void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp)
{
    struct LL_Node* cur_node = c_ptr->first;
    
    while ( cur_node != NULL )
    {
        afp( cur_node->data_ptr );
        cur_node = cur_node->next;
    }
}

void* OC_find_item_arg(const struct Ordered_container* c_ptr, const void* arg_ptr, OC_find_item_arg_fp_t fafp)
{
	struct LL_Node* cur_node = c_ptr->first; 
	while ( cur_node != NULL )
    {
    	/* check to see if we have a matching data */ 
    	if ( fafp( arg_ptr, cur_node->data_ptr ) == 0 )
    	{
    		return cur_node;
    	}

    	cur_node = cur_node->next; 
    }

    return NULL; 
}

int OC_apply_if(const struct Ordered_container* c_ptr, OC_apply_if_fp_t afp)
{
	struct LL_Node* cur_node = c_ptr->first;
	int fp_t_value; /* holds the return value of the OC_apply_if_fp_t */

	while ( cur_node != NULL )
	{
		fp_t_value = afp( cur_node->data_ptr );
        
		if ( fp_t_value != 0 )
		{
			return fp_t_value;
		}

		cur_node = cur_node->next; 
	}
	return 0; 
}

void OC_apply_arg(const struct Ordered_container* c_ptr, OC_apply_arg_fp_t afp, void* arg_ptr)
{
	struct LL_Node* cur_node = c_ptr->first;
    
	while ( cur_node != NULL )
	{
		afp( cur_node->data_ptr, arg_ptr ); 
		cur_node = cur_node->next; 
	}
}

int OC_apply_if_arg(const struct Ordered_container* c_ptr, OC_apply_if_arg_fp_t afp, void* arg_ptr)
{
	struct LL_Node* cur_node = c_ptr->first;
	int fp_t_value; /* holds the return value of the OC_apply_if_fp_t */
    
    
    
	while ( cur_node != NULL )
	{
		fp_t_value = afp( cur_node->data_ptr, arg_ptr );
		if ( fp_t_value != 0 )
		{
			return fp_t_value; 
		}
        cur_node = cur_node->next;
	}

	return 0; 
}
int OC_get_size(const struct Ordered_container* c_ptr)
{
    return c_ptr->size;
}

int OC_empty(const struct Ordered_container* c_ptr)
{
    return !OC_get_size( c_ptr );
}

void* OC_get_data_ptr(const void* item_ptr)
{
    /* cast the ptr and return the data_ptr */
    return ( (struct LL_Node*)item_ptr )->data_ptr;
}

