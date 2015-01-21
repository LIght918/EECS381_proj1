/*
//  test_record1.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/20/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Record.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main( void )
{
    char* title1 = "The Hangover";
    char* medium1 = "DVD";
    int new_rating = 10;
    int i;
    FILE * outfile;
    
    struct Record* temp;
    struct Record* r1 = create_Record( medium1, title1 );
    
    
    assert( get_Record_ID(r1) == 0 );
    assert( strcmp( get_Record_title(r1), title1 ) == 0 );
    
    
    print_Record( r1 );
    printf( "setting r1's rating to %d\n", new_rating );
    set_Record_rating( r1, new_rating );
    print_Record( r1 );
    
    reset_Record_ID_counter();
    outfile = fopen( "Test_record1_save.out", "w" );
    assert( outfile );
    
    for( i = 0; i < new_rating; ++i )
    {
        temp = create_Record( medium1, title1 );
        assert( get_Record_ID(temp) == i );
        set_Record_rating( temp, 10 - i );
        save_Record(temp , outfile );
        destroy_Record( temp );
    }
    
    fclose( outfile );
    
    return 0;
}

