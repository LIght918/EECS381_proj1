
#include <iostream>
#include <fstream> 
#include <stdio.h> 

using namespace std; 

const int num_rec = 26; 
const int num_col = 0; 
int main( void )
{
	FILE* out_file = fopen( "big_data_test.txt", "w" );

	if ( !out_file)
	{
		exit( 1 ); 
	}

	fprintf( out_file, "%d\n", num_rec );
	char c = 'a';

	for (int i = 0; i < num_rec ; ++i)
	{
		fprintf( out_file, "%d DVD %d %c\n", 35 - i , (rand() % 5) + 1,  rand() % 26 + 'a' );	
	}

	fprintf( out_file, "%d\n", num_col );

	for (int i = 0; i < num_col ; ++i)
	{
		/* code */
	}
	return 0; 
}


