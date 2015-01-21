/*
//  test_Utility.c
//  EECS381_proj1
//
//  Created by Charlie OConor on 1/21/15.
//  Copyright (c) 2015 Charlie OConor. All rights reserved.
*/

#include "Record.h"
#include "Utility.h"
#include "p1_globals.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>


int main( void )
{
    int i;
    char* white_space_test = "    much ado   about nothing  ";
    FILE* infile;
    struct Record* rec;
    char title[ TITLE_MAX_BUFF_SIZE ];
    
    remove_white_space( white_space_test );
    printf( "New string is %s", white_space_test );
    
    infile = fopen( "Utility_read.txt", "r" );
    
    for ( i = 0; i < 3; ++i)
    {
        get_title( infile, title);
        printf("title is: %s\n", title );
    }
    
    fclose( infile );

    infile = fopen("load_Record_test.txt", "r" );
    
    for ( i = 0; i < 5 ; ++i )
    {
        rec = load_Record( infile );
        destroy_Record( rec );
    }
    
    return 0;
}