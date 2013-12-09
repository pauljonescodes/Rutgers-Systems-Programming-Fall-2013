#define malloc( x ) my_malloc( x, __FILE__ , __LINE__ )
#define free( x ) my_free( x, __FILE__, __LINE__ )

void freeing_pointers_not_allocated() 
{
	int x;
	free( x );
}

void freeing_pointers_not_returned_from_malloc()
{
	int p = (char *)malloc( 200 );
	free( p + 10 );
}

void redundant_freeing_of_the_same_pointer() 
{
	char * p= (char *)malloc( 200 );
	free( p );
	free( p );
}

void perfectly_valid() 
{
	char * p = (char *)malloc( 100 ); 
	//free( p );
	p = (char *) malloc( 100 ); 
	free( p );
}

int main(int argc, char const *argv[])
{
	//freeing_pointers_not_allocated();
	//freeing_pointers_not_returned_from_malloc();
	//redundant_freeing_of_the_same_pointer();
	perfectly_valid();
	return 0;
}