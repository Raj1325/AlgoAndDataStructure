
#include	<stdio.h>
#include	<stdlib.h>

#define ARRAY_LENGTH 100
const int LENGTH_OF_FILENAME = 20;

int data		[ ARRAY_LENGTH ] ;
int forward 	[ ARRAY_LENGTH ] ;
int backward	[ ARRAY_LENGTH ] ;
int validCount 	= -1;

int main ( int argc , char **argv )
{
	
	FILE *source 	= NULL ;
	char fileName [ LENGTH_OF_FILENAME ] ;
	printf("File Path :") ;
	fscanf(stdin , "%s",fileName) ;
	source		= fopen( fileName , "r" ) ;
	if( !source )
	{
		fprintf( stderr , "\'%s\' : No Such File\n",fileName ) ;
		return EXIT_FAILURE ;
	}
	
	int what , where ;
	while( fscanf( source , "%i %i" , &what , &where ) != EOF )
	{	
		
		if( what == -1 && where == -1 ){
			printf( " Finished Storing [%d] Entries ... \n" , validCount ) ;
			break;
		}
		
		if( validCount > ARRAY_LENGTH ){
			fprintf( stderr , "Stack Overflow, Ignoring data, Max Array Size is [%d]\n" , ARRAY_LENGTH ) ;
		}
		
		else{
		
			if( (where >= 0) && (where <= ARRAY_LENGTH) )
			{
				validCount++;
				//assuming there is no redundancy in "where" while reading file
				data[where]			= what;
				forward[validCount]	= where;
				backward[where]		= validCount;
			}
			else{
				fprintf( stderr , " Array Index[%d] Out Of Bound\n" , where ) ;
			}
			
		}
	}
	
	int probe;
	while( fscanf( source , "%i" , &probe ) != EOF )
	{
		if( probe == -1 ){
		
			printf( " Finished testing ... \n" ) ;
			break;
			
		}
		else{
		
			if( probe <= ARRAY_LENGTH ){
			
				if( backward[probe] > 0 &&
					backward[probe] <= validCount &&
					forward[ backward[probe] ] == probe )
				{
					printf( "Position %d has been initialized to value %d\n" , probe , data[probe] );	
				}
				else{
					printf( "Position %d has not been initialized\n" , probe );
				}
				
			}
			else{
				fprintf( stderr , " Probe Index[%d] Out Of Bound\n" , where ) ;
			}
			
		}
	}
	fclose( source );
	
	return EXIT_SUCCESS	;

}
