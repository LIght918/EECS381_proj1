

#include <stdlib.h>
#include <stdio.h>

void init_global_fstring( char* input, int buffer_size )
{
	sprintf( input, "%%%ds", buffer_size );	
}



int main( void )
{
	char test[ 100 ];
	init_global_fstring( test, 5 );
	
	printf( "%s\n", test ); 
	return 0; 
}
