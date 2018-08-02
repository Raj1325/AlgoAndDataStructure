#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define 	LCHILD( parent ) 	( (parent*2) + 1 )
#define 	RCHILD( parent ) 	( (parent*2) + 2 )
#define     PARENT( child  )    ( child/2 )

const int MAX_HEAP_SIZE		= 100;
const int LENGTH_OF_FILENAME= 20;

int *heap 			= NULL;
int last_index		= -1;
int totalElements	= 0;

bool 	makeHeap	( );
bool	shiftUp		( int );
bool 	shiftDown 	( int );
bool 	heapSort 	( );

int main	( int argCount , char **argV )
{
	if(	!( heap = (int*)malloc(sizeof(int) * MAX_HEAP_SIZE) ) )
	{
		fprintf( stderr , "Insufficient Memory\n" );
		return EXIT_FAILURE;
	}
	
	
	FILE *source 	= NULL;
	char fileName[LENGTH_OF_FILENAME];
	printf("File Path :");
	fscanf(stdin , "%s", fileName);
	source		= fopen( fileName , "r" );
	if( !source )
	{
		fprintf( stderr , "\'%s\' : No Such File\n",fileName );
		return EXIT_FAILURE;
	}
	
	
	int numberFromFile;
	int numberCount 	= 0;
	while( fscanf( source , "%d" , &numberFromFile ) != EOF )
	{
		if( numberCount >= MAX_HEAP_SIZE ){
			fprintf( stderr , "Stack Overflow, Aborting File Read, Max Stack Size is [%d]\n" , MAX_HEAP_SIZE );
			break; 
		}
		else
		{
			heap[numberCount]	= numberFromFile;
			numberCount++;
			last_index++;
			totalElements++;
		}
	}
	fclose( source );
	printf( "Total Elements : %d\n\n" , totalElements);
	
	printf( "Elements before making heap : ");
	for( int itr = 0 ; itr < totalElements ; itr++ )
	{
		printf( "%d, " , heap[itr] );
	}
	printf("\n\n");
	
	makeHeap( );
	
	printf( "Elements after making heap : ");
	for( int itr = 0 ; itr < totalElements ; itr++ )
	{
		printf( "%d, " , heap[itr] );
	}
	printf("\n\n");
	
	heapSort( );
	
	printf( "Elements after heapsort : ");
	for( int itr = 0 ; itr < totalElements ; itr++ )
	{
		printf( "%d, " , heap[itr] );
	}
	printf("\n");
	
	return EXIT_SUCCESS;
	
}


bool 	makeHeap	( )
{
	
	bool 	isheapBuild 	= false;
	
	for( int parentToItr= last_index/2 ; parentToItr > -1 ; parentToItr-- ) 
	{
		shiftDown( parentToItr );
	}
	
	return isheapBuild;

}


bool 	shiftDown 	( int parentIndex )
{
	bool isShiftedDown 		= false;
	bool isLeftChildPresent	= false;
	bool isRightChildPresent= false;
	
	
	while( (LCHILD(parentIndex) <= last_index) || (RCHILD(parentIndex) <= last_index) ) 
	{
		isShiftedDown 	= true;
		int biggerChild = parentIndex;
		
		if( (LCHILD(parentIndex) <= last_index) )
		{
			isLeftChildPresent = true;
		}
		if( (RCHILD(parentIndex) <= last_index) )
		{
			isRightChildPresent = true;
		}
		
		
		if( isLeftChildPresent && isRightChildPresent)
		{
			biggerChild = (heap[LCHILD(parentIndex)] > heap[RCHILD(parentIndex)]) ? \
							LCHILD(parentIndex) : RCHILD(parentIndex);
		}
		else if( isLeftChildPresent ){
			biggerChild = LCHILD(parentIndex);
		}
		else if( isRightChildPresent )
		{
			biggerChild = RCHILD(parentIndex);
		}
		
		
		if( heap[parentIndex] < heap[biggerChild] )
		{
			int swapTemp 		= heap[parentIndex];
			heap[parentIndex] 	= heap[biggerChild];
			heap[biggerChild] 	= swapTemp;
			parentIndex 		= biggerChild;
		}
		else{
			break;
		}
		
		isRightChildPresent = false;
		isLeftChildPresent  = false;
	}
	
	return isShiftedDown;
}


bool	shiftUp		( int node)
{
	bool isShiftedUp 	= false;
	
	while( PARENT(node) > 0 )
	{
		if( heap[PARENT(node)] < heap[node] )
		{
			isShiftedUp 		= true;
			int swapTemp 		= heap[PARENT(node)];
			heap[PARENT(node)]	= heap[node];
			heap[node] 			= heap[PARENT(node)];
			node 				= PARENT(node);
		}
		else{
			break;
		}
		
	}
	
	return isShiftedUp;
}


bool 	heapSort 	( )
{
	bool isHeapSorted 	= false;
		
	int swapTemp;
	for( int itr = last_index ; itr > 0 ; itr-- )
	{
		isHeapSorted= true;
		swapTemp 	= heap[0];
		heap[0] 	= heap[itr];
		heap[itr] 	= swapTemp;
		last_index--;
		shiftDown( 0 );
	}
	
	return isHeapSorted;
}

