

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>


const int MAX_STACK_SIZE 	= 50;
const int MAX_WORD_SIZE		= 30;
const int LENGTH_OF_FILENAME= 20;
int stackTopIndex 			= -1;

char **stack = NULL;

bool 	push	( const char *word );
char* 	pop 	( );
char*	top		( );
bool	isEmpty	( );


bool 	initializeStack 	( );
bool 	releaseStack 		( );


int main (int argc , char** argv )
{

	if( !initializeStack() )
	{
		fprintf( stderr , "Stack Not Initialized\n" );
		return EXIT_FAILURE;
	}
	
	
	FILE *source 	= NULL;
	char fileName[LENGTH_OF_FILENAME];
	printf("File Path :");
	fscanf(stdin , "%s",fileName);
	source		= fopen( fileName , "r" );
	if( !source )
	{
		fprintf( stderr , "\'%s\' : No Such File\n",fileName );
		return EXIT_FAILURE;
	}
	
	
	char wordFromFile[MAX_WORD_SIZE];
	while( fscanf( source , "%s" , wordFromFile ) != EOF )
	{	
		if( !push( wordFromFile ) ){
			fprintf( stderr , "Stack Overflow, Aborting File Read, Max Stack Size is [%d]\n" , MAX_STACK_SIZE );
			break; 
		}
	}
	fclose( source );
	
	
	while( !isEmpty() ){
		printf( "%s\n" , pop() );
	}
	
	releaseStack( );
	
	return EXIT_SUCCESS;
	
} 


bool 	push	( const char *word )
{

	bool isPushed 	= false;
	
	stackTopIndex++;
	if( stackTopIndex < MAX_STACK_SIZE )
	{
		strcpy( stack[stackTopIndex] , word );
		isPushed 	= true;
	}
	else{
		stackTopIndex--;
	}
	
	return isPushed;
	
}


char* 	pop 	( )
{
	 
	 char *word	= top ( );
	 
	 if( word )
	 {
	 	stackTopIndex--;
	 }
	 
	 return word;
	 
}



char*	top		( )
{
	char *word	= NULL;
	 
	 if( !isEmpty() )
	 {
	 	word 	= (char*) malloc ( sizeof( char ) * MAX_WORD_SIZE );
	 	strcpy ( word , stack[stackTopIndex] );
	 }
	 
	 return word;
	 
}


bool	isEmpty	( )
{

	bool isStackEmpty 	= false;
	
	if( stackTopIndex < 0 )
	{
		isStackEmpty = true;
	}
	
	return isStackEmpty;

}


bool 	initializeStack 	( )
{

	bool isInitialized	= false;
	
	stack = (char**) malloc ( MAX_STACK_SIZE * sizeof( void* ) );
	for(int firstDimensionItr = 0 ; firstDimensionItr < MAX_STACK_SIZE ; firstDimensionItr++)
	{
		stack[firstDimensionItr] = (char*) malloc ( MAX_WORD_SIZE * sizeof(char) );
		for( int secDimensionItr = 0 ; secDimensionItr < MAX_WORD_SIZE ; secDimensionItr++)
		{
			stack[firstDimensionItr][secDimensionItr] = '\0';
		}
	}	
	isInitialized	= true;
	
	return isInitialized;
	
}


bool 	releaseStack	( )
{

	bool isReleased = false;
	
	for( int itr = 0 ; itr < MAX_STACK_SIZE ; itr++ )
	{
		free( stack[itr] );
	}
	free( stack );
	isReleased = true;
	
	return isReleased;
	
}
