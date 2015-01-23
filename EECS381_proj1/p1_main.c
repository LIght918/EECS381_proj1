#include <stdlib.h>
#include <stdio.h>
#include "Ordered_container.h"
#include "Utility.h"
#include "Record.h"
#include "p1_globals.h"

/* print the unrecognized command error */
static void error_unrecognized_command( void );
static struct Record* get_record( struct Ordered_container* c_ptr , void* data_ptr );
static void find_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID );
static void print_allocation( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog);

int main(void)
{
    struct Ordered_container* lib_title = OC_create_container( comp_Record_by_title );
    struct Ordered_container* lib_ID    = OC_create_container( comp_Record_by_ID );
    struct Ordered_container* catalog   = OC_create_container( comp_Collection_by_name );
    
    char first_c, second_c = '\0';
    
    for ( ; ; )
    {
        printf( "\nEnter command: " );
        if ( ( ( first_c = getchar() ) <= 0 ) || ( ( second_c = getchar() ) <= 0 ) )
        {
            /* what error checking TODO */
            /* what else to clean up */
            continue;
        }
        printf( "%c %c", first_c, second_c );
        switch ( first_c )
        {
            case 'f' :/* find (records only)*/
                
                switch ( second_c )
                {
                    case 'r' :
                        find_record( lib_title, lib_ID );
                        break;
                    default:
                        error_unrecognized_command();
                        break;
                }
                break;
            case 'p' : /* print */
                switch ( second_c ) {
                    case 'r' :
                        
                        break;
                    case 'c':
                        
                        break;
                    case 'm':
                        
                        break;
                    case 'L':
                        
                        break;
                    case 'C':
                        
                        break;
                    case 'A':
                        
                        break;
                    case 'a': /* allocation */
                        print_allocation( lib_title, lib_ID, catalog );
                        break;
                    default:
                        break;
                }
                
                break;
            case 'm': /* modify (rating only) */
                
                
                break;
            case 'a' : /* add */
                
                
                break;
            case 'd': /* delete */
                
                
                break;
            case 'c': /* clear */
                
                break;
            case 's': /* save */
                
                break;
            case 'r': /* restore */
                
                break;
            default:
                /* throw error for bad input */
                error_unrecognized_command();
                break;
        }
  
    }
    
	return 0;
}


static void error_unrecognized_command( void )
{
    printf( "Unrecognized command!\n" );
}

static struct Record* get_record( struct Ordered_container* c_ptr , void* data_ptr )
{
    void* node_ptr = OC_find_item( c_ptr, data_ptr );
    return ( struct Record* ) OC_get_data_ptr( node_ptr );
}


static void find_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID )
{
    int ID;
    char title[ TITLE_ARRAY_SIZE ];
    struct Record* cur_node = NULL;
    
    /* check to see if the surch is of type int */
    if ( scanf( "%d", &ID ) == 1 )
    {
        cur_node = get_record( lib_ID, &ID );
        if ( cur_node == NULL )
        {
            /* if it wasn't found throw an error and exit */
            printf( "No record with that title!\n" );
            return;
        }
    }
    else if ( get_title( stdin, title ) )
    {
        cur_node = get_record( lib_ID, title );
        if ( cur_node == NULL ) {
            /* if it wasn't found throw an error and exit */
            printf( "No record with that ID!\n" );
            return;
        }
    }
    else
    {
        /* throw error */
        /* TODO find what error message to throw */
        
        return;
    }
    
    print_Record( cur_node );
}

static void print_allocation( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog)
{
    printf( "Memory allocations:\n" );
    printf( "Records: %d\n", OC_get_size( lib_title) + OC_get_size( lib_ID ) );
    printf( "Collections: %d\n", OC_get_size( catalog ) );
    printf( "Containers: %d\n", g_Container_count );
    printf( "Container items in use: %d\n", g_Container_items_in_use );
    printf( "Container items allocated: %d\n", g_Container_items_allocated );
    printf( "C-strings: %d bytes total\n", g_Container_count );
}
