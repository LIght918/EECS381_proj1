
#include <iostream>
#include <fstream> 
#include <stdio.h> 

using namespace std; 

const int num_rec = 699; 
const int num_col = 0; 
const int size = 50;

int main( void )
{
	FILE* out_file = fopen( "big_data_test.txt", "w" );

	if ( !out_file)
	{
		exit( 1 ); 
	}

	fprintf( out_file, "%d\n", num_rec );
	char c = 'a';
	char string[ size ]; 

	for( int i = 0 ; i < size ; ++i )
	{
		string[ i ] = '\0' ;
	}

	string[ 0 ] = 'a';
	int x = 0; 
	for (int i = 1; i < num_rec ; ++i)
	{
		fprintf( out_file, "%d DVD %d %s\n", i , (rand() % 5) + 1, string );	
		
		string[x]++;

		if ( i % 26 == 0  )
		{
			string[ x ] = 'a';
			string[ ++x ] = 'a';
		}
	}

	fprintf( out_file, "%d\n", num_col );

	for (int i = 0; i < num_col ; ++i)
	{
		/* code */
	}
	return 0; 
}


