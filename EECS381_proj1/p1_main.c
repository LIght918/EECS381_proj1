#include <stdlib.h>
#include <stdio.h>
#include "Ordered_container.h"
#include "Utility.h"
#include "Record.h"
#include "p1_globals.h"
#include <assert.h>



enum error { COMMAND,
    DUPLICATE_REC,
    DUPLICATE_COLL,
    IN_COLL,
    NOT_IN_COLL,
    CANT_DELETE,
    CLEAR_COLL,
    NOT_FOUND_TITLE,
    NOT_FOUND_ID,
    NOT_FOUND_COLL,
    READ_TITLE,
    READ_INT,
    RATING_RANGE,
    FILE_OPEN,
    INVAL_DATA };

/* print the unrecognized command error */
static void print_error( enum error err  );

static void find_record( struct Ordered_container* lib_title );
static void print_allocation( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog);
static void add_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID );
static void delete_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID );


/* Helper Functions */

/* check if there is already something in the OC with that name and adds it if not
   returns 0 if add nonzero if not */
static int  check_dup_add( struct Ordered_container* c_ptr, void* data_ptr );
/* returns the Record assosiated with the title or ID  */
static void* get_record( struct Ordered_container* c_ptr , void* data_ptr );
static char get_command_char( void );
/* reads in the medium and title from stdin returns 0 if sucsessful nonzero if not */
static int get_medium_and_title( char* medium, char* title );
/* on error clears the rest of the line and throws it away */
static void clear_line( void );



    
    


int main(void)
{
    struct Ordered_container* lib_title = OC_create_container( comp_Record_by_title );
    struct Ordered_container* lib_ID    = OC_create_container( comp_Record_by_ID );
    struct Ordered_container* catalog   = OC_create_container( comp_Collection_by_name );
    int i;
    
    /*char first_c, second_c = '\0';
    char buffer[ TITLE_ARRAY_SIZE ];*/
    char command[ 3 ];
    
    /* init the command string */
    command[ 2 ] = '\0';
    
    for ( ; ; )
    {
        printf( "\nEnter command: " );
        
        for ( i = 0 ; i < 2; ++i)
        {
            command[ i ] = get_command_char();
        }
        
        switch ( command[ 0 ] )
        {
            case 'f' :/* find (records only)*/
                
                switch ( command[ 1 ] )
                {
                    case 'r' :
                        find_record( lib_title );
                        break;
                    default:
                        print_error( COMMAND );
                        break;
                }
                break;
            case 'p' : /* print */
                switch ( command[ 1 ] ) {
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
                switch ( command[ 1 ] ) {
                    case 'r' :
                        add_record( lib_title, lib_ID );
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
                        /* throw error */
                        break;
                    default:
                        break;
                }
                break;
            case 'd': /* delete */
                switch ( command[ 1 ] ) {
                    case 'r' :
                        delete_record( lib_title, lib_ID );
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
                        /* throw error */
                        break;
                    default:
                        break;
                }
                break;
            case 'c': /* clear */
                switch ( command[ 1 ] ) {
                    case 'r' :
                        /* can you clear a record */
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
                        /* throw error */
                        break;
                    default:
                        break;
                }
                break;
            case 's': /* save */
                
                break;
            case 'r': /* restore */
                
                break;
            default:
                /* throw error for bad input */
                print_error( COMMAND );
                break;
        }
  
    }
    
	return 0;
}

static void print_error( enum error err  )
{
    clear_line();
    printf( "ERROR" ); 
    
    /*"Unrecognized command!\n"
    "Library already has a record with this title!\n"
    "Catalog already has a collection with this name!\n"
    "Record is already a member in the collection!\n"
    "Record is not a member in the collection!\n"
    "Cannot delete a record that is a member of a collection!\n"
    "Cannot clear all records unless all collections are empty!\n"
    "No record with that title!\n"
    "No record with that ID!\n"
    "No collection with that name!\n"
    "Could not read a title!\n"
    "Could not read an integer value!\n"
    "Rating is out of range!\n"
    "Could not open file!\n"
    "Invalid data found in file!\n"*/
    
    switch ( err ) {
        case COMMAND:
            fprintf( stderr, "Unrecognized command!\n");
            break;
        case DUPLICATE_REC:
            fprintf( stderr, "Library already has a record with this title!\n" );
            break;
        case DUPLICATE_COLL:
            fprintf( stderr,"Catalog already has a collection with this name!\n");
            break;
        case IN_COLL:
            fprintf( stderr,"Record is already a member in the collection!\n");
            break;
        case NOT_IN_COLL:
            fprintf( stderr,"Record is not a member in the collection!\n");
            break;
        case CANT_DELETE:
            fprintf( stderr,"Cannot delete a record that is a member of a collection!\n");
            break;
        case CLEAR_COLL:
            fprintf( stderr,"Cannot clear all records unless all collections are empty!\n");
            break;
        case NOT_FOUND_TITLE:
            fprintf( stderr,"No record with that title!\n");
            break;
        case NOT_FOUND_ID:
            fprintf( stderr,"No record with that ID!\n");
            break;
        case NOT_FOUND_COLL:
            fprintf( stderr,"No collection with that name!\n");
            break;
        case READ_TITLE:
            fprintf( stderr,"Could not read a title!\n");
            break;
        case READ_INT:
            fprintf( stderr,"Could not read an integer value!\n");
            break;
        case RATING_RANGE:
            fprintf( stderr, "Rating is out of range!\n");
            break;
        case FILE_OPEN:
            fprintf( stderr,"Could not open file!\n");
            break;
        case INVAL_DATA:
            fprintf( stderr,"Invalid data found in file!\n");
            break;
        default:
            fprintf( stderr, "Error Unknow\n" );
            break;
    }
}

static void clear_line( void )
{
    char buffp[ TITLE_ARRAY_SIZE ];
    fgets( buffp , TITLE_MAX_BUFF_SIZE, stdin );
    printf( "%s\n", buffp );
}


/* TODO change this name */
static void* get_record( struct Ordered_container* c_ptr , void* data_ptr )
{
    void* node_ptr = OC_find_item( c_ptr, data_ptr );
    if ( node_ptr == NULL )
    {
        return NULL;
    }
    return OC_get_data_ptr( node_ptr );
}


static void find_record( struct Ordered_container* lib_title )
{
    struct Record* rec;
    char title[ TITLE_ARRAY_SIZE ];
    
    if( !get_title( stdin, title ) )
    {
        print_error( READ_TITLE );
        return;
    }
    
    rec = get_record( lib_title, title );
    if ( rec == NULL )
    {
        print_error( NOT_FOUND_TITLE );
        return;
    }
    
    print_Record( rec ); 
    
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

static void add_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID )
{
    char title[ TITLE_ARRAY_SIZE ];
    char medium[ MEDIUM_ARRAY_SIZE ];
    struct Record* new_rec;
    
    /* read the record in from the command line */
    if ( !get_medium_and_title( medium, title ) )
    {
        printf( "Medium: %s\nTitle: %s\n", medium, title );
        print_error( READ_TITLE );
        return;
    }
    
    if ( OC_find_item( lib_title, title ) != NULL )
    {
        print_error( DUPLICATE_REC );
        return;
    }
    else
    {
        new_rec = create_Record(medium, title );
        print_Record( new_rec );
        OC_insert( lib_title, new_rec );
        OC_insert( lib_ID, new_rec );
    }
}

static int  check_dup_add( struct Ordered_container* c_ptr, void* data_ptr )
{
    /* need to imp TODO */
    /* assert until finished */
    assert( 0 );
    
    return 0;
}

static void delete_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID )
{
    void* node_to_remove; /* = find_record( lib_title, lib_ID );*/
    struct Record* rec_to_remove;
    
    rec_to_remove = OC_get_data_ptr( node_to_remove );
    
    OC_delete_item( lib_ID, node_to_remove );
    OC_delete_item( lib_title, node_to_remove);
    
    /* clean up memory */
    destroy_Record( rec_to_remove );
}

static char get_command_char( void )
{
    int c;
    
    /* load chars until we get one that it not white space 
       and throw an error if we get a EOF */
    while ( ( ( c = getchar() ) > 0 ) && is_white_space( c ) )
        ;
    
    return c;
}

static int get_medium_and_title( char* medium, char* title )
{
    if ( scanf("%s ", medium) != 1 || !get_title( stdin, title ) ) {
        return 1;
    }
    return 0;
}


