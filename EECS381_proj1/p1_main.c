#include "Collection.h"
#include "Ordered_container.h"
#include "Utility.h"
#include "Record.h"
#include "p1_globals.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


enum error {
    COMMAND,
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
    INVAL_DATA,
    ASSERT
};


typedef void* (*Node_or_Data)( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum error err ) ;
typedef int (*Collection_fptr)(struct Collection* collection_ptr, const struct Record* record_ptr);

/* print the unrecognized command error */
static void print_error( enum error err  );

static void find_record_print( struct Ordered_container* lib_title );

static void add_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID );
static void add_coll( struct Ordered_container* catalog );

static void delete_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog);
static void delete_collection( struct Ordered_container* catalog );

static void print_collection_main( struct Ordered_container* catalog );
static void print_record( struct Ordered_container* lib_ID );
static void print_containter( struct Ordered_container* c_ptr, char* type, char* holds, OC_apply_fp_t fp );
static void print_allocation( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog);

static void apply_collection_func( struct Ordered_container* lib_ID, struct Ordered_container* catalog, Collection_fptr fp, char* action, enum error err );
static void modify_rating( struct Ordered_container* lib_ID );

static void clear_container( struct Ordered_container* c_ptr , OC_apply_fp_t destructor, char* output );
static void clear_library( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog );
static void clear_all( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog );

/* Helper Functions */

static char get_command_char( void );
/* reads in the medium and title from stdin returns 0 if sucsessful nonzero if not */
static int get_medium_and_title( char* medium, char* title );
/* on error clears the rest of the line and throws it away */
static void clear_line( void );

static void* get_data_ptr( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum error err );
static void* get_node(struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum error err );
/* print the record with that is equal to the data_prt */
static void print_rec( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum error err  );
static void find_remove( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum error err );
static int is_rec_in_catalog( void* data_ptr, void* arg_ptr);

static struct Collection* find_collection_by_name( struct Ordered_container* catalog, Node_or_Data fp );

static int read_int( int* num );
static void read_name( char* name );


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
                        find_record_print( lib_title );
                        break;
                    default:
                        print_error( COMMAND );
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
                        /*print_lib( lib_title );*/
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
                        print_error( COMMAND );
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
                        print_error( COMMAND );
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
                        apply_collection_func( lib_ID , catalog, add_Collection_member,  "added", IN_COLL );
                        break;
                    case 'L':
                        
                        break;
                    case 'C':
                        
                        break;
                    case 'A':
                        
                        break;
                    case 'a': /* allocation */
                        /* throw error */
                        print_error( COMMAND );
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
                         apply_collection_func( lib_ID , catalog, remove_Collection_member, "deleted", NOT_IN_COLL );
                        break;
                    case 'L':
                        
                        break;
                    case 'C':
                        
                        break;
                    case 'A':
                        clear_all( lib_title, lib_ID, catalog );
                        break;
                    case 'a': /* allocation */
                        /* throw error */
                        break;
                    default:
                        print_error( COMMAND );
                        break;
                }
                break;
            case 'c': /* clear */
                switch ( command[ 1 ] )
                {
                    case 'r' :
                        /* can you clear a record */
                        break;
                    case 'c':
                        
                        break;
                    case 'm':
                        
                        break;
                    case 'L':
                        clear_library( lib_title, lib_ID, catalog );
                        break;
                    case 'C':
                        clear_container( catalog, ( void(*)(void*) )destroy_Collection, "All collections deleted\n" );
                        break;
                    case 'A':
                        
                        break;
                    case 'a': /* allocation */
                        /* throw error */
                        break;
                    default:
                        print_error( COMMAND );
                        break;
                }
                break;
            case 's': /* save */
                
                break;
            case 'r': /* restore */
                
                break;
            case 'q':
                switch ( command[ 1 ] )
                {
                    case 'q':
                        /* clean up memory */
                        return 0;
                        break;
                    default:
                        print_error( COMMAND );
                        break;
                }
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
            clear_line();
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
            clear_line();
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
        case ASSERT:
            assert(0); /* should cascade on NDEBUG */ 
        default:
            fprintf( stderr, "Error Unknow\n" );
            break;
    }
}

static void clear_line( void )
{
    char buffp[ TITLE_ARRAY_SIZE ];
    fgets( buffp , TITLE_MAX_BUFF_SIZE, stdin );
}

static void print_rec( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum error err  )
{
    struct Record* rec = get_data_ptr( c_ptr, fafp, data_ptr, err );
    
    if ( rec != NULL )
    {
        print_Record( rec );
    }
}

static struct Record* find_record_by_title( struct Ordered_container* lib_title )
{
    char title[ TITLE_ARRAY_SIZE ];
    struct Record* rec;
    
    if( get_title( stdin, title ) )
    {
        print_error( READ_TITLE );
        return NULL;
    }
    
    rec = get_data_ptr( lib_title, comp_Record_to_title, title, NOT_FOUND_TITLE );
    return rec;
}



static void find_record_print( struct Ordered_container* lib_title )
{
    struct Record* rec = find_record_by_title( lib_title );
    
    /* if we find the record print it out */
    if ( rec )
    {
        print_Record( rec );
    }
    
}

/* reads in a int and returns true if there was no error
    if an error occures it prints a message and returns false */
static int read_int( int* num )
{
    if ( scanf("%d", num ) != 1 )
    {
        print_error( READ_INT );
        return false;
    }
    return true;
}


static void print_record( struct Ordered_container* lib_ID )
{
    int ID;
    
    if ( !read_int( &ID ) )
    {
        return;
    }
    
    print_rec( lib_ID, comp_Record_to_ID, &ID, NOT_FOUND_ID );
}

static void* get_data_ptr( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum error err )
{
    void* cur_node = get_node(c_ptr, fafp, data_ptr, err );

    if ( cur_node == NULL )
    {
        return NULL;
    }
    
    return OC_get_data_ptr( cur_node );
}

static void* get_node(struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum error err )
{
    void* cur_node = OC_find_item_arg( c_ptr, data_ptr, fafp );
    
    if ( cur_node == NULL )
    {
        print_error( err );
    }
    
    return cur_node;
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
    char title[ TITLE_ARRAY_SIZE ];
    char medium[ MEDIUM_ARRAY_SIZE ];
    struct Record* new_rec;
    

    /* read the record in from the command line */
    if ( !get_medium_and_title( medium, title ) )
    {
        print_error( READ_TITLE );
        return;
    }
    
    if ( OC_find_item_arg( lib_title, title , comp_Record_to_title ) != NULL )
    {
        print_error( DUPLICATE_REC );
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

static void find_remove( struct Ordered_container* c_ptr, OC_find_item_arg_fp_t fafp, void* data_ptr, enum error err )
{
    void* node = OC_find_item_arg( c_ptr, data_ptr, fafp );
    
    if ( node == NULL )
    {
        print_error( err );
    }
    
    OC_delete_item( c_ptr, node );
}

int is_rec_in_catalog( void* data_ptr, void* arg_ptr)
{
    return is_Collection_member_present( ( struct Collection* )data_ptr, arg_ptr );
}



static void delete_record( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog )
{
    struct Record* rec_to_remove = find_record_by_title( lib_title );
    
    if ( rec_to_remove )
    {
        const char* title;
        int ID ;
        
        /* TODO test this line */
        if( OC_apply_if_arg( catalog, is_rec_in_catalog, rec_to_remove ) )
        {
            print_error( IN_COLL ) ;
            return ;
        }
        
        ID = get_Record_ID( rec_to_remove );
        title = get_Record_title( rec_to_remove );
        
        /* remove from both libs */
        find_remove(lib_title, comp_Record_to_title, (void*)title, NOT_FOUND_TITLE );
        find_remove(lib_ID, comp_Record_to_ID, &ID, ASSERT );
        
        printf("Record %d %s deleted\n", ID, title );
        
        destroy_Record( rec_to_remove );
    }
}




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


static void read_name( char* name )
{
    if (scanf( "%s", name ) != 1 )
    {
        /* there shouldn't be an error but
         just to be safe */
        assert( 0 );
    }
}

static void add_coll( struct Ordered_container* catalog )
{
    
    struct Collection* new_coll;
    char name[ NAME_ARRAY_SIZE ];
    
    read_name( name );
    
    if ( OC_find_item_arg( catalog, name, comp_Collection_to_name ) != NULL )
    {
        print_error( IN_COLL );
        return;
    }
    
    new_coll = create_Collection( name );
    
    OC_insert( catalog, new_coll );
    printf("Collection %s added\n", name );
}

static struct Collection* find_collection_by_name( struct Ordered_container* catalog, Node_or_Data fp )
{
    char name[ NAME_ARRAY_SIZE ];
    
    read_name( name );
    
    return fp( catalog, comp_Collection_to_name, name, NOT_FOUND_COLL );
}


static void print_collection_main( struct Ordered_container* catalog )
{
    struct Collection* coll = find_collection_by_name( catalog, get_data_ptr );
    
    if ( coll != NULL  )
    {
        print_Collection( coll );
    }
}

static void delete_collection( struct Ordered_container* catalog )
{
    void* node = find_collection_by_name( catalog, get_node );
    
    
    if ( node != NULL  )
    {
        struct Collection* coll = OC_get_data_ptr( node );
        
        printf("Collection %s deleted\n", get_Collection_name( coll ) );
        
        OC_delete_item( catalog, node );
        
        destroy_Collection( coll );
    }
}

static void modify_rating( struct Ordered_container* lib_ID )
{
    struct Record* rec;
    int ID, rating;
    
    if ( !read_int( &ID ) || !read_int( &rating ) )
    {
        return;
    }
    
    if ( rating < 0 || rating > MAX_RATING )
    {
        print_error( RATING_RANGE );
        return;
    }
    
    rec = get_data_ptr( lib_ID, comp_Record_to_ID, &ID, NOT_FOUND_ID );
    
    if ( rec )
    {
        printf( "Rating for record %d changed to %d\n", get_Record_ID(rec), rating );
        set_Record_rating( rec, rating );
        
    }
}


static void apply_collection_func( struct Ordered_container* lib_ID, struct Ordered_container* catalog, Collection_fptr fp, char* action, enum error err )
{
    int ID;
    struct Record* rec;
    struct Collection* coll = find_collection_by_name( catalog, get_data_ptr );
    
    
    if ( read_int( &ID ) && coll )
    {
        rec = get_data_ptr( lib_ID, comp_Record_to_ID, &ID, NOT_FOUND_ID );
        if ( rec )
        {
            if ( !fp( coll, rec ) )
            {
                printf("Member %d %s %s\n", ID, get_Record_title( rec ), action );
            }
            else
            {
                print_error( err );
            }
            
        }
    }
}

static void clear_container( struct Ordered_container* c_ptr , OC_apply_fp_t destructor, char* output )
{
    OC_apply( c_ptr, destructor );
    OC_clear( c_ptr );
    printf( "%s", output );
}



static void clear_library( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog )
{
    if ( OC_apply_if( catalog, is_Collection_not_empty ) )
    {
        print_error( CLEAR_COLL );
    }
    else
    {
        clear_container(lib_ID, ( void(*)(void*) )destroy_Record, "") ;
        clear_container(lib_title, ( void(*)(void*) )destroy_Record, "All records deleted\n");
    }
}


static void clear_all( struct Ordered_container* lib_title, struct Ordered_container* lib_ID, struct Ordered_container* catalog )
{
    clear_container( catalog, ( void(*)(void*) )destroy_Collection, "" );
    clear_container(lib_ID, ( void(*)(void*) )destroy_Record, "") ;
    clear_container(lib_title, ( void(*)(void*) )destroy_Record, "All data deleted\n" );
}
