
//BST
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <iomanip>
#include <iostream>

#define 	LCHILD( parent ) 	( (parent*2) + 1 )
#define 	RCHILD( parent ) 	( (parent*2) + 2 )
#define     PARENT( child  )    ( child/2 )

#define ARRAY_SIZE 10000
#define LENGTH_OF_FILENAME 20

typedef struct Node{
	
	int data;
	bool hasLeftChild;
	bool hasRightChild;
	
}TREE_NODE;

TREE_NODE TREE[ARRAY_SIZE] ;
int TOTAL_ELEMENTS = 0;


bool insertData	( int );
bool inOrderTraverse ( int );

int main ( int argc , char **argv )
{
	int errorCode = EXIT_SUCCESS;
	
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
	while( fscanf( source , "%d" , &numberFromFile ) != EOF )
	{
		if( TOTAL_ELEMENTS >= ARRAY_SIZE ){
			fprintf( stderr , "Stack Overflow, Aborting File Read, Max Stack Size is [%d]\n" , ARRAY_SIZE );
			break; 
		}
		else
		{
			if( !insertData( numberFromFile ) ){
				errorCode = EXIT_FAILURE;
				printf( "Error Inserting %d Data...\n", TOTAL_ELEMENTS );
				break;
			}
			TOTAL_ELEMENTS++;
		}
	}
	fclose( source );
	
	if( !inOrderTraverse( 0 ) )
	{
		errorCode = EXIT_FAILURE;
		printf( "Error Traversing Tree...\n" );
	}
	std::cout<<std::endl;
	return errorCode;	

}

bool insertData	( int data )
{
	
	bool insertionSucceded = false;
	
	
	if( TOTAL_ELEMENTS == 0 )
	{
		//root is empty , insert data into root
		TREE[TOTAL_ELEMENTS].data = data;
		TREE[TOTAL_ELEMENTS].hasLeftChild	= false;
		TREE[TOTAL_ELEMENTS].hasRightChild	= false;
		insertionSucceded = true;
	}
	else{
		
		int NodeIndex = 0;
		bool isInsertLeft = false;
		bool hasChild = false;
		while( !insertionSucceded )
		{
			if( data <= TREE[NodeIndex].data  )
			{
				isInsertLeft = true;
				hasChild 	 = TREE[NodeIndex].hasLeftChild;
			}
			else{
				isInsertLeft = false;
				hasChild 	 = TREE[NodeIndex].hasRightChild;	
			}
			
			if(hasChild){
				if(	isInsertLeft )
				{
					NodeIndex = LCHILD(NodeIndex);
				}
				else{
					NodeIndex = RCHILD(NodeIndex);
				}
			}
			else{
			
				if(	isInsertLeft )
				{	
					if( LCHILD(NodeIndex) < ARRAY_SIZE )
					{
						TREE[LCHILD(NodeIndex)].data  = data;
						TREE[LCHILD(NodeIndex)].hasLeftChild = false;
						TREE[LCHILD(NodeIndex)].hasRightChild = false;
						TREE[NodeIndex].hasLeftChild  = true;
						insertionSucceded = true;
						//printf("%d\n" , LCHILD(NodeIndex) );
					}
					else{
						printf(" Cannot insert left data, index out of bound \n");
						break;
					}
					
				}
				else{
					if( RCHILD(NodeIndex) < ARRAY_SIZE )
					{
						TREE[RCHILD(NodeIndex)].data  = data;
						TREE[RCHILD(NodeIndex)].hasLeftChild = false;
						TREE[RCHILD(NodeIndex)].hasRightChild = false;
						TREE[NodeIndex].hasRightChild  = true;
						insertionSucceded = true;
						//printf("%d\n" , RCHILD(NodeIndex) );
					}
					else{
						printf(" Cannot insert right data, index out of bound \n");
						break;
					}	
				}
			}
			
			
		}
		
	}
	
	return insertionSucceded;

}


bool inOrderTraverse ( int index )
{
	static int rowCnt = 0;
	bool doneTraversal = true;
	
	if( TREE[index].hasLeftChild )
	{
		inOrderTraverse( LCHILD(index) );
	}
	
	if(rowCnt == 10 ){
		std::cout << std::endl;
		rowCnt = 0;
	}
	std::cout << std::setw(5) << TREE[index].data << " ";
	rowCnt++;
	//printf( "%d\n" , TREE[index].data );
	
	if( TREE[index].hasRightChild )
	{
		inOrderTraverse( RCHILD(index) );
	}
	
	return doneTraversal;

}
