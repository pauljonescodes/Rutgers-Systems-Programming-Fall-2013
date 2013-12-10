#define malloc( x ) my_malloc( x, __FILE__ , __LINE__ )
#define free( x ) my_free( x, __FILE__, __LINE__ )

void freeing_pointers_not_allocated() 
{
	printf("\n--------------------------------");
	printf("\nBegin **freeing pointer not allocated** test. \n\n");

	printf("You should expect to see a denied free \n\n");

	int x;
	free( x );

	printf("\nEnd **freeing porinter not allocated** test.");
	printf("\n------------------------------\n\n");

}

void freeing_pointers_not_returned_from_malloc()
{
	printf("\n--------------------------------");
	printf("\nBegin **freeing pointers not returned from malloc** test. \n\n");

	printf("You should expect to see a denied free.\n\n");
	
	int p = (char *)malloc( 200 );
	free( p + 10 );

	printf("\nEnd **freeing pointers not returned from malloc** test.");
	printf("\n------------------------------\n\n");


}

void redundant_freeing_of_the_same_pointer() 
{	
	printf("\n--------------------------------");
	printf("\nBegin **redundant freeing** test. \n\n");

	printf("You should expect to see an allocation, a free,");
	printf("and then a denied free.\n\n");
	
	char * p= (char *)malloc( 200 );
	free( p );
	free( p );

	printf("\nEnd **redundant freeing** test.");
	printf("\n------------------------------\n\n");
}

void perfectly_valid() 
{
	printf("\n--------------------------------");
	printf("\nBegin **perfectly valid** test. \n\n");

	printf("You should expect to see normal allocations.\n\n");	

	char * p = (char *)malloc( 100 ); 
	free( p );
	p = (char *) malloc( 100 ); 
	free( p );

	printf("\nEnd **perfectly valid** test.");
	printf("\n------------------------------\n\n");


}

int main(int argc, char const *argv[])
{
	freeing_pointers_not_allocated();
	freeing_pointers_not_returned_from_malloc();
	redundant_freeing_of_the_same_pointer();
	perfectly_valid();
	return 0;
}
