#include "Ordered_container.h"
/* skeleton file for Ordered_container_list.c
The struct declarations below must be used for the linked-list node and Ordered_container objects.
Remove this comment and complete this file with all necessary code.
*/


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

struct Ordered_container* OC_create_container(OC_comp_fp_t f_ptr)
{
	struct Ordered_container* new_container;

	if ( f_ptr == NULL )
	{
		/* throw and error */

		return NULL; 
	}

	new_container = ( struct Ordered_container* ) malloc( sizeof( struct Ordered_container ) ); 

	new_container->comp_func = f_ptr;
	new_container->first = NULL;
	new_container->last = NULL;
	new_container->size = 0; 

	return new_container;
}

void OC_destroy_container(struct Ordered_container* c_ptr)
{
	struct LL_Node* node;


	if ( c_ptr == NULL )
		return; 
	
	node = c_ptr->first;

	/* am I missing the last pointer */
	while ( node != c_ptr->last )
	{
		free ( node->data_ptr ); 
		node = node->next;
		free ( node->prev );
	}

	free( node->data_ptr );
	free( node ); 

}

void OC_clear(struct Ordered_container* c_ptr)
{
	OC_destroy_container( c_ptr ); 

}


int OC_get_size(const struct Ordered_container* c_ptr)
{
	if ( c_ptr == NULL )
	{
		/* throw error */
		return 0 ; 
	}
	return c_ptr->size; 
}

