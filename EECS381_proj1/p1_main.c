
#include "Collection.h"
#include "Ordered_container.h"
#include "Utility.h"
#include "Record.h"
#include "p1_globals.h"
#define NDEBUG
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_RATING 5
#define MIN_RATING 1

/*
 *
 *  Function Declarations
 *
 *  top level functions - called from main()
 *
 */

static void find_record_print( struct Ordered_container* lib_title );

static void add_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID );

static void add_coll( struct Ordered_container* catalog );

static void delete_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID,
                           struct Ordered_container* catalog);

static void delete_collection( struct Ordered_container* catalog );

static void print_collection_main( struct Ordered_container* catalog );

static void print_record( struct Ordered_container* lib_ID );

static void print_containter( struct Ordered_container* c_ptr, char* type, char* holds, OC_apply_fp_t fp );

static void print_allocation( struct Ordered_container* lib_title, struct Ordered_container* lib_ID,
                              struct Ordered_container* catalog);

static void remove_member( struct Ordered_container* lib_ID, struct Ordered_container* catalog );
static void add_member( struct Ordered_container* lib_ID, struct Ordered_container* catalog );


static void modify_rating( struct Ordered_container* lib_ID );

static void clear_container( struct Ordered_container* c_ptr , OC_apply_fp_t destructor, char* output );

static void clear_library( struct Ordered_container* lib_title, struct Ordered_container* lib_ID,
                           struct Ordered_container* catalog, char* output );

static void clear_all( struct Ordered_container* lib_title, struct Ordered_container* lib_ID,
                       struct Ordered_container* catalog, char* message );

static void quit( struct Ordered_container* lib_title, struct Ordered_container* lib_ID,
                  struct Ordered_container* catalog );

static void load_from_file( struct Ordered_container* lib_title, struct Ordered_container* lib_ID,
                            struct Ordered_container* catalog );

static void load_from_file( struct Ordered_container* lib_title, struct Ordered_container* lib_ID,
                            struct Ordered_container* catalog );

static void save_all_to_file( struct Ordered_container* lib_title, struct Ordered_container* catalog);



/*
 *  Helper Functions
 */

static char get_command_char( void );

/* reads in the medium and title from stdin returns 0 if successful nonzero if not */
static int get_medium_and_title( char* medium, char* title );

/* print the record with that is equal to the data_ptr */
static void print_rec( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, char* err_message  );

/* find the record associated with that data_ptr and remove it */
static int find_remove( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr );



/* reads the name in from stdin and finds the collection associated with that
 * if there isn't one prints and error and returns NULL */ 
static void* find_collection_by_name( struct Ordered_container* catalog );

/* 
 *   read input with checks for overflow buffers
 */

/* reads in a int and returns true if there was no error
 if an error occurs it prints a message and returns false */
static int read_int( int* num );

static void read_name( char* name );

/* if fopen fails it prints an error and returns NULL */
static FILE* read_open_file( const char* mode );

static void print_error( char* message );
static void print_error_clear( char* message );

/* on error clears the rest of the line and throws it away */
static void clear_line( void );

/* 
 *   used as function pointers in other functions
 */

/* loads the Ordered Container with a next record in the file and returns the record */
void* load_rec( FILE* in_file, struct Ordered_container* c_ptr );

/* uses strcmp on the name of each Collection and returns the value */
static int comp_Collection_by_name( const void* left, const void* right );

/* uses strcmp on name and collection */
static int comp_Collection_to_name(const void* arg_ptr, const void* data_ptr );

/* returns the inverse of empty */
static int is_Collection_not_empty( void* data_ptr );

/* compares a record to an ID, int value returns true if equal */
int comp_Record_to_ID( const void* arg_ptr, const void* data_ptr );

/* uses < on the ID values of each records */
int comp_Record_by_ID( const void* left, const void* right );


int main( void )
{
    int i;
    
    /* holds both command characters */
    char command[ 2 ];
    
    /* set up the three libraries */
    struct Ordered_container* lib_title = OC_create_container( comp_Record_by_title );
    struct Ordered_container* lib_ID    = OC_create_container( comp_Record_by_ID );
    struct Ordered_container* catalog   = OC_create_container( comp_Collection_by_name );
    
    for ( ; ; )
    {
        printf( "\nEnter command: " );
        
        /* load the command chars */
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
                        find_record_print( lib_title );
                        break;
                    default:
                        print_error_clear( "Unrecognized command!\n");
                        break;
                }
                break;
            case 'p' : /* print */
                switch ( command[ 1 ] )
                {
                    case 'r' :
                        print_record( lib_ID );
                        break;
                    case 'L':
                        print_containter( lib_title, "Library", "records", (void (*)(void*))print_Record );
                        break;
                    case 'C':
                        /* print_catalog( catalog ); */
                        print_containter( catalog, "Catalog", "collections", (void (*)(void*))print_Collection );
                        break;
                    case 'a': /* allocation */
                        print_allocation( lib_title, lib_ID, catalog );
                        break;
                    case 'c':
                        print_collection_main( catalog );
                        break;
                    default:
                        print_error_clear( "Unrecognized command!\n");
                        break;
                }
                break;
            case 'm': /* modify (rating only) */
                switch ( command[ 1 ] )
                {
                    case 'r':
                        modify_rating( lib_ID );
                        break;
                    default:
                        print_error_clear( "Unrecognized command!\n");
                        break;
                }
                break;
            case 'a' : /* add */
                switch ( command[ 1 ] )
                {
                    case 'r' :
                        add_record( lib_title, lib_ID );
                        break;
                    case 'c':
                        add_coll( catalog );
                        break;
                    case 'm':
                        add_member( lib_ID , catalog );
                        break;
                    case 'a': /* allocation */
                        /* throw error */
                        print_error_clear( "Unrecognized command!\n");
                        break;
                    default:
                        break;
                }
                break;
            case 'd': /* delete */
                switch ( command[ 1 ] )
                {
                    case 'r' :
                        delete_record( lib_title, lib_ID, catalog );
                        break;
                    case 'c':
                        delete_collection( catalog );
                        break;
                    case 'm':
                         remove_member( lib_ID , catalog );
                        break;
                    case 'a': /* allocation */
                        /* throw error */
                        break;
                    default:
                        print_error_clear( "Unrecognized command!\n");
                        break;
                }
                break;
            case 'c': /* clear */
                switch ( command[ 1 ] )
                {
                    case 'L':
                        clear_library( lib_title, lib_ID, catalog, "All records deleted\n" );
                        break;
                    case 'C':
                        clear_container( catalog, ( void(*)(void*) )destroy_Collection, "All collections deleted\n" );
                        break;
                    case 'A':
                        clear_all(lib_title, lib_ID, catalog, "All data deleted\n" );
                        break;
                    default:
                        print_error_clear( "Unrecognized command!\n");
                        break;
                }
                break;
            case 's': /* save */
                switch ( command[ 1 ] ) {
                    case 'A':
                        save_all_to_file( lib_title, catalog );
                        break;
                    default:
                        print_error_clear( "Unrecognized command!\n");
                        break;
                }
                break;
            case 'r': /* restore */
                switch ( command[ 1 ] ) {
                    case 'A':
                        load_from_file( lib_title, lib_ID, catalog );
                        break;
                    default:
                        print_error_clear( "Unrecognized command!\n");
                        break;
                }
                break;
            case 'q':
                switch ( command[ 1 ] )
                {
                    case 'q':
                        /* clean up memory */
                        quit( lib_title, lib_ID, catalog );
                        break;
                    default:
                        print_error_clear( "Unrecognized command!\n");
                        break;
                }
            default:
                /* throw error for bad input */
                print_error_clear( "Unrecognized command!\n");
                break;
        }
    }
	return 0;
}


static struct Record* find_record_by_title( struct Ordered_container* lib_title )
{
    char title[ TITLE_ARRAY_SIZE ];
    struct Record* rec;
    
    if( get_title( stdin, title ) )
    {
        print_error( "Could not read an integer value!\n" );
        return NULL;
    }
    
    rec = get_data_ptr( lib_title, comp_Record_to_title, title );
    
    if ( rec == NULL )
    {
        print_error( "No record with that title!\n" );
    }
    return rec;
}

static void find_record_print( struct Ordered_container* lib_title )
{
    struct Record* rec = find_record_by_title( lib_title );
    
    /* if we find the record print it out */
    if ( rec != NULL )
    {
        print_Record( rec );
    }
}


static void print_record( struct Ordered_container* lib_ID )
{
    int ID;
    
    if ( !read_int( &ID ) )
    {
        return;
    }
    
    print_rec( lib_ID, comp_Record_to_ID, &ID, "No record with that ID!\n" );
}


static void print_rec( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, char* err_message  )
{
    struct Record* rec = get_data_ptr( c_ptr, fafp, data_ptr );
    
    
    if ( rec != NULL )
    {
        print_Record( rec );
    }
    else
    {
        print_error( err_message );
    }
}

static void print_allocation( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog)
{
    printf( "Memory allocations:\n" );
    printf( "Records: %d\n", OC_get_size( lib_ID ) );
    printf( "Collections: %d\n", OC_get_size( catalog ) );
    printf( "Containers: %d\n", g_Container_count );
    printf( "Container items in use: %d\n", g_Container_items_in_use );
    printf( "Container items allocated: %d\n", g_Container_items_allocated );
    printf( "C-strings: %d bytes total\n", g_string_memory );
}

static void add_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID )
{
    char title [ TITLE_ARRAY_SIZE  ];
    char medium[ MEDIUM_ARRAY_SIZE ];
    struct Record* new_rec;
    
    /* read the record in from the command line */
    if ( !get_medium_and_title( medium, title ) )
    {
        print_error( "Could not read a title!\n" );
        return;
    }
    
    /* check if the Record already exists */
    if ( OC_find_item_arg( lib_title, title , comp_Record_to_title ) != NULL )
    {
        print_error( "Library already has a record with this title!\n" );
        return;
    }
    else
    {
        new_rec = create_Record(medium, title );
        OC_insert( lib_title, new_rec );
        OC_insert( lib_ID, new_rec );
        printf( "Record %d added\n", get_Record_ID( new_rec ) );
    }
}


static int get_medium_and_title( char* medium, char* title )
{
    if ( scanf( "%" STRINGIFY( MEDIUM_MAX_SIZE ) "s" , medium) != 1 || !get_title( stdin, title ) ) {
        return 1;
    }
    return 0;
}


static void delete_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog )
{
    struct Record* rec_to_remove = find_record_by_title( lib_title );
    
    if ( rec_to_remove )
    {
        const char* title;
        int ID ;
        
        /* check to see if Record is in catalog 
           if so throw and error and return */
        if ( OC_apply_if_arg( catalog, (OC_apply_if_arg_fp_t) is_Collection_member_present, rec_to_remove ) )
        {
            print_error( "Cannot delete a record that is a member of a collection!\n" ) ;
            return ;
        }
        
        ID = get_Record_ID( rec_to_remove );
        title = get_Record_title( rec_to_remove );
        
        /* remove from both libs */
        if ( find_remove( lib_title, comp_Record_to_title, (void*)title ) )
        {
            find_remove( lib_ID, comp_Record_to_ID, &ID );
        }
        else
        {
            print_error( "No record with that title!\n" );
        }
        
        printf("Record %d %s deleted\n", ID, title );
        
        destroy_Record( rec_to_remove );
    }
}

/* remove the node associated with the data_ptr
 * if there is not record found print the error err
 * requires a OC_find_item_arg_fp_t fafp that returns true for the data_ptr given
 * return 1 on success NULL if otherwise
 */
static int find_remove( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr )
{
    void* node = OC_find_item_arg( c_ptr, data_ptr, fafp );
    
    if ( node == NULL )
    {
        return 0 ;
    }
    
    OC_delete_item( c_ptr, node );
    return 1;
}


/*  prints the container with the supplied function pointer
 *  type  is the name of the container
 *  holds is what the container's data_ptrs are
 *  all ptr must be valid
 */
static void print_containter( struct Ordered_container* c_ptr, char* type, char* holds, OC_apply_fp_t fp )
{
    if ( OC_empty( c_ptr ) )
    {
        printf( "%s is empty\n", type );
    }
    else
    {
        printf( "%s contains %d %s:\n", type, OC_get_size( c_ptr ), holds );
        OC_apply( c_ptr, fp );
    }
}

static char get_command_char( void )
{
    int c;
    
    /* load chars until we get one that it not white space 
       and throw an error if we get a EOF */
    while ( ( ( c = getchar() ) > 0 ) && isspace( c ) )
        ;
    
    return c;
}

static void add_coll( struct Ordered_container* catalog )
{
    struct Collection* new_coll;
    char name[ NAME_ARRAY_SIZE ];
    
    read_name( name );
    
    if ( OC_find_item_arg( catalog, name, comp_Collection_to_name ) != NULL )
    {
        print_error( "Catalog already has a collection with this name!\n" );
        return;
    }
    
    new_coll = create_Collection( name );
    
    OC_insert( catalog, new_coll );
    printf("Collection %s added\n", name );
}


/* I had these two functions implemented with function pointers but I broke them up according to spec */

static void remove_member( struct Ordered_container* lib_ID, struct Ordered_container* catalog )
{
    int ID;
    
    /* load the collection */
    void* data_ptr = find_collection_by_name( catalog ) ;
    
    /* check for read errors  */
    if ( data_ptr && read_int( &ID ) )
    {
        struct Record* rec;
        struct Collection* coll = OC_get_data_ptr( data_ptr );
        rec = get_data_ptr( lib_ID, comp_Record_to_ID, &ID );
        /* make sure there is a record with that ID */
        if ( rec )
        {
            if ( !remove_Collection_member( coll, rec ) )
            {
                /* if it was successful print out the action and what it was applied to */
                printf("Member %d %s deleted\n", ID, get_Record_title( rec ) );
            }
            else
            {
                print_error( "Record is not a member in the collection!\n" );
            }
            
        }
        else
        {
            print_error( "No record with that ID!\n" );
        }
    }
    else
    {
        clear_line();
    }
}


static void add_member( struct Ordered_container* lib_ID, struct Ordered_container* catalog )
{
    int ID;
    /* load the collection */
    void* data_ptr = find_collection_by_name( catalog ) ;
    
    /* check for read errors  */
    if ( data_ptr && read_int( &ID ) )
    {
        struct Record* rec;
        struct Collection* coll = OC_get_data_ptr( data_ptr );
        
        rec = get_data_ptr( lib_ID, comp_Record_to_ID, &ID);
        
        /* make sure there is a record with that ID */
        if ( rec )
        {
            if ( !add_Collection_member( coll, rec ) )
            {
                /* if it was successful print out the action and what it was applied to */
                printf("Member %d %s added\n", ID, get_Record_title( rec ) );
            }
            else
            {
                print_error( "Record is already a member in the collection!\n" );
            }
        }
        else
        {
            print_error( "No record with that ID!\n" );
        }
    }
    else
    {
        clear_line();
    }
}

static void print_collection_main( struct Ordered_container* catalog )
{
    void* coll =  find_collection_by_name( catalog );
    
    if ( coll != NULL  )
    {
        print_Collection( OC_get_data_ptr( coll ) );
    }
}

static void delete_collection( struct Ordered_container* catalog )
{
    void* node = find_collection_by_name( catalog );
    
    if ( node != NULL  )
    {
        struct Collection* coll = OC_get_data_ptr( node );
        
        printf("Collection %s deleted\n", get_Collection_name( coll ) );
        
        OC_delete_item( catalog, node );
        
        destroy_Collection( coll );
    }
}

/* reads in a name from stdin and returns the collection associated with it
   returns NULL and throws an error if there wasn't any collection by that name */
static void* find_collection_by_name( struct Ordered_container* catalog )
{
    char name[ NAME_ARRAY_SIZE ];
    void* node;
    
    read_name( name );
    
    node = OC_find_item_arg( catalog, name, comp_Collection_to_name );
    
    if ( node == NULL )
    {
        print_error( "No collection with that name!\n" );
    }
    
    return node;
}

/* reads in a name of NAME_ARRAY_SIZE
 requires name have at least that much memory alloc to it
 as well as is a valid pointer */
static void read_name( char* name )
{
    /* puts the limit in the string literal */
    if ( scanf( "%" STRINGIFY( NAME_MAX_SIZE ) "s" , name ) != 1 )
    {
        /* there shouldn't be an error but
         just to be safe */
        assert( 0 );
    }
}

static void modify_rating( struct Ordered_container* lib_ID )
{
    struct Record* rec;
    int ID, rating;
    
    /* read input and check for errors */
    if ( !read_int( &ID ) || !read_int( &rating ) )
        return;

    rec = get_data_ptr( lib_ID, comp_Record_to_ID, &ID );
    
    if ( rec )
    {
        /* make sure rating is in range */
        if ( rating < MIN_RATING || rating > MAX_RATING )
        {
            print_error( "Rating is out of range!\n" );
            return;
        }
        
        printf( "Rating for record %d changed to %d\n", get_Record_ID(rec), rating );
        set_Record_rating( rec, rating );
    }
    else
    {
        print_error( "No record with that ID!\n" );
    }
}

/*
 *
 * Restore and load functions
 * with their helper functions
 *
 */

static FILE* read_open_file( const char* mode )
{
    char name[ FILENAME_ARRAY_SIZE ];
    FILE* new_file;
    
    if ( scanf( "%" STRINGIFY( FILENAME_MAX_SIZE ) "s", name ) != 1 )
        return NULL;
    
    new_file = fopen( name, mode );
    
    if ( new_file == NULL )
    {
        print_error_clear( "Could not open file!\n" );
    }
    
    return new_file;
}

static void save_all_to_file( struct Ordered_container* lib_title, struct Ordered_container* catalog)
{
    FILE* out_file = read_open_file( "w" );
    
    if ( out_file )
    {
        fprintf( out_file, "%d\n", OC_get_size( lib_title ) );
        OC_apply_arg( lib_title, ( OC_apply_arg_fp_t )save_Record,  out_file );
        
        fprintf( out_file, "%d\n", OC_get_size( catalog ) );
        OC_apply_arg( catalog, ( OC_apply_arg_fp_t )save_Collection, out_file );
        
        printf( "Data saved\n" );
        fclose( out_file );
    }
}

static void load_from_file( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog )
{
    FILE* in_file = read_open_file( "r" );
    
    if ( in_file )
    {
        int i, num;
        clear_all( lib_title, lib_ID, catalog, "" );
        
        /* read in the number of Records to load */
        if( fscanf( in_file, "%d", &num ) != 1 )
        {
            clear_all( lib_title, lib_ID, catalog, "" );
            print_error( "Invalid data found in file!\n" );
            return ;
        }
        
        /* load the data */
        for ( i = 0; i < num; ++i )
        {
            struct Record* rec = load_Record( in_file );
            
            if ( rec )
            {
                /* if record was loaded correctly insert it */
                OC_insert( lib_ID, rec );
                OC_insert( lib_title, rec);
            }
            else
            {
                clear_all( lib_title, lib_ID, catalog, "" );
                print_error( "Invalid data found in file!\n" );
                return;
            }
        }
        
        /* read in the number of things to load */
        if( fscanf( in_file, "%d", &num ) != 1 )
        {
            clear_all( lib_title, lib_ID, catalog, "" );
            print_error( "Invalid data found in file!\n" );
            return ;
        }
        
        /* load the data in from the file */
        for ( i = 0 ; i < num ; ++i )
        {
            struct Collection* coll = load_Collection( in_file, lib_title );
            
            if ( coll )
                OC_insert( catalog, coll );
            else
            {
                clear_all( lib_title, lib_ID, catalog, "" );
                print_error( "Invalid data found in file!\n" );
                return;
            }
        }
        
        printf( "Data loaded\n" );
        fclose( in_file );
    }
}


/*
 *
 * Clear functions and helpers as well as quit()
 *
 */

static void clear_library( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog, char* output )
{
    if ( strlen( output ) > 0 && OC_apply_if( catalog, is_Collection_not_empty ) )
    {
        print_error( "Cannot clear all records unless all collections are empty!\n" );
    }
    else
    {
        reset_Record_ID_counter();
        
        /* clear lib_ID instead of calling clear_container b/c
         it points to the same data as lib_title */
        OC_clear( lib_ID );
        
        /* clear the container and destroy the records */
        clear_container(lib_title, ( void(*)(void*) )destroy_Record, output );
        assert( OC_get_size( lib_ID ) == 0 );
        assert( OC_get_size( lib_title ) == 0 ); 
    }
}

static void clear_all( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog, char* message )
{
    clear_container( catalog, ( void(*)(void*) )destroy_Collection, "" );
    /*clear_container(lib_ID, ( void(*)(void*) )destroy_Record, "") ;*/
    clear_library(lib_title, lib_ID, catalog, message );
}


static void quit( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog )
{
    clear_all( lib_title, lib_ID, catalog, "All data deleted\n" );
    printf( "Done\n" );
    exit(0);
}


/* clears a Container as well as applies the destructor to all the data it pointed to */
static void clear_container( struct Ordered_container* c_ptr , OC_apply_fp_t destructor, char* output )
{
    OC_apply( c_ptr, destructor );
    OC_clear( c_ptr );
    printf( "%s", output );
}

/*
 *
 *  Function to read in
 *
 */

/* reads in a int and returns nonzero if there was no error
 if an error occurs it prints a message and returns 0  */
static int read_int( int* num )
{
    if ( scanf("%d", num ) != 1 )
    {
        print_error_clear( "Could not read an integer value!\n" );
        return 0;
    }
    return 1;
}

/* print the error message and clears the stream */
static void print_error_clear( char* message )
{
    fprintf( stdout, "%s", message );
    clear_line();
}

/* clears the input buffer on an error */
void clear_line( void )
{
    while ( getchar() != '\n' ) ;
}


static void print_error( char* message )
{
    fprintf( stdout, "%s", message );
}


/*
 *  Comparator functions to be passed to OC containers
 *
 */

static int comp_Collection_by_name( const void* left, const void* right )
{
    return strcmp( get_Collection_name( (struct Collection* )left  ),
                  get_Collection_name( (struct Collection* )right ));
}

static int comp_Collection_to_name(const void* arg_ptr, const void* data_ptr )
{
    return strcmp( arg_ptr, get_Collection_name( (struct Collection* ) data_ptr ) );
}

static int is_Collection_not_empty( void* data_ptr )
{
    return !Collection_empty( ( struct Collection* ) data_ptr );
}

int comp_Record_by_ID( const void* left, const void* right )
{
    return get_Record_ID( (struct Record* )left ) < get_Record_ID( (struct Record* )right );
}

int comp_Record_to_ID( const void* arg_ptr, const void* data_ptr )
{
    return ( get_Record_ID( (struct Record* )data_ptr ) -  *( (int*) arg_ptr ) );
}
