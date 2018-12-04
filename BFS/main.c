

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define LENGTH_OF_FILENAME 20
typedef struct List{
    int data;
    struct List *next , *back;
}List;

int     **adjMatrix;
bool    *visited;
List    *head = NULL;
List    *tail = NULL;

void    enqueue( int );
int     dequeue( );

bool    initialize( int );


int     main( int argc , char **argv )
{

    FILE *source = NULL;
	char fileName[LENGTH_OF_FILENAME];
	printf("File Path :");
	fscanf(stdin, "%s", fileName);
	source = fopen(fileName, "r");
	if (!source)
	{
		fprintf(stderr, "\'%s\' : No Such File\n", fileName);
		return EXIT_FAILURE;
	}

    int totalVertices , pvert , cvert;
    fscanf(source, "%d", &totalVertices);
    initialize( (totalVertices+1) );
    while(fscanf(source, "%d %d", &pvert , &cvert) != EOF )
    {
        adjMatrix[pvert][cvert] = 1;
        adjMatrix[cvert][pvert] = 1;
    }

    enqueue(0);
    visited[0] = true;
    int current;
    while( (current = dequeue()) != -1 )
    {
            int itr= 0;
            for(itr = 0 ; itr <= totalVertices ; itr++)
            {
                if(adjMatrix[current][itr] == 1)
                {
                    if( visited[itr] == false )
                    {
                        enqueue(itr);
                        visited[itr] = true;
                        printf("%d %d\n" , current , itr);

                    }
                }
            }
    }


     return EXIT_SUCCESS;

}


bool    initialize( int totalVertices )
{

    adjMatrix   = (int**)malloc(sizeof(int)*totalVertices);
    int itr=0;
    for(itr = 0 ; itr < totalVertices ; itr++)
    {
        adjMatrix[itr] = (int*)malloc(sizeof(int)*totalVertices);
        int i = 0;
        for(i = 0 ; i < totalVertices ; i++)
        {
            adjMatrix[itr][i] = 0;
        }
    }


    visited = (bool*)malloc(sizeof(bool)*totalVertices);
    for(itr = 0 ; itr < totalVertices ; itr++)
    {
        visited[itr] = false;
    }

}


void    enqueue( int data )
{

    List *node  = (List*)malloc(sizeof(List));
    node->data  = data;
    node->next  = NULL;
    node->back  = NULL;
    if( head == NULL )
    {
        head = node;
        tail = head;
    }
    else
    {
        tail->next = node;
        node->back = tail;
        tail = node;
    }

}


int     dequeue( )
{

    if(head != NULL)
    {
        int data = head->data;
        List *temp = head;
        head = head->next;
        if(head == NULL)
        {
            tail = NULL;
        }
        free(temp);
        return data;
    }

    return -1;
}


