
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define LENGTH_OF_FILENAME 20

int totalNodes , totalEdges;
int pathFrom , pathTo;
double **adjMatrix;

typedef struct Node{
    double x , y ;
}Node;

Node *nodes;


bool initialize(int , char **);
void generalizedDijkstra(double (*heuristicCallback)(Node , Node) , int);
void optimizedDijkstra(int);
double euclidean(Node , Node);
double dijkstraHuristic(Node , Node );
int* mergePath( int [] , int [] , int , int*);
bool* mergeSelect(bool [] , bool []);


int getMin(double [] , bool []);
bool isCandidSetEmpty(bool []);
double squareRoot(double );
double power(double , int );
void printWhole( int[] , bool[] , int);
int checkCommonNode(bool [] , bool []);
void displayDistance( int [] , int  , int );
void displaytracePath( int [] , int  , int );
void displayAdditionalNodes( bool [] ,int [], int  , int );

int main (int argc , char **argv)
{

    if(!initialize(argc , argv))
    {
        printf("Returning with failure");
        return EXIT_FAILURE;
    }
    //printWhole();

    generalizedDijkstra(&dijkstraHuristic , 1);
    printf("\n");
    generalizedDijkstra(&euclidean , 2);
    printf("\n");
    optimizedDijkstra(3);

    return EXIT_SUCCESS;
}


bool initialize(int argc , char **argv)
{

    FILE    *source ;

    if( argc > 1)
    {
        source  = fopen( argv[1] , "r" );
    }
    else{
        char fileName[LENGTH_OF_FILENAME];
        printf("Enter File Name : ");
        scanf( "%s" , fileName );
        source = fopen( fileName , "r" );
    }

    if( !source )
    {
        fprintf( stderr , "Error Opening File... \n"  );
        return false;
    }

    fscanf(source , "%d" , &totalNodes);
    totalNodes++; //start indexing from 1..
    //initialize nodes...
    nodes = (Node*)malloc(sizeof(Node) * totalNodes);
    int itr = 1;
    for(itr = 1 ; itr < totalNodes ; itr++)
    {
        int nodeNumber ;
        double x , y;
        fscanf(source , "%d %lf %lf" , &nodeNumber , &x , &y);
        nodes[nodeNumber].x = x;
        nodes[nodeNumber].y = y;
    }

    //initializeAdjMatrix...
    adjMatrix = (double**)malloc(sizeof(double) * totalNodes);
    for(itr=0 ; itr < totalNodes ; itr++)
    {
        adjMatrix[itr] = (double*)malloc(sizeof(double) * totalNodes);
        int itrInner = 0;
        for(itrInner = 0 ; itrInner < totalNodes ; itrInner++)
        {
            //adjMatrix[itr][itrInner] = -1;
            if(itr == itrInner)
            {
                adjMatrix[itr][itrInner] = 0;
            }
            else{
                adjMatrix[itr][itrInner] = -1;
            }
        }
    }

    fscanf(source , "%d" , &totalEdges);
    for(itr = 0 ; itr < totalEdges ; itr++)
    {
        int from , to;
        double weight;
        fscanf(source , "%d %d %lf" , &from , &to , &weight);
        adjMatrix[from][to] = weight;
        adjMatrix[to][from] = weight;
    }

    fscanf(source , "%d %d" , &pathFrom , &pathTo);
    fclose(source);
    return true;

}


double squareRoot(double val)
{
    if(val < 0)
    {
        return 0;
    }
    double x = val;
    double y = 1;
    double a = 0.000001;
    while(x-y > a)
    {
        x = (x+y)/2;
        y = val/x;
    }

    return x;
}

double power(double val , int power)
{
    double returnVal=1;
    int itr=0;
    for(itr = 0 ; itr < power ; itr++)
    {
        returnVal *= val;
    }
    return returnVal;
}

double euclidean(Node pointOne , Node pointTwo)
{
    int xSqr = power( (pointOne.x - pointTwo.x) , 2 );
    int ySqr = power( (pointOne.y - pointTwo.y) , 2 );
    return squareRoot( (xSqr + ySqr));
}

double dijkstraHuristic(Node pointOne , Node pointTwo)
{
    return 0;
}

int getMin(double distance[] , bool SNodes[])
{
    double minVal = -1;
    int minValueIndex = 0;
    int itr=0 ;
    for(itr=0 ; itr < totalNodes ; itr++)
    {
        if( (SNodes[itr] != true) )
        {
            if(minVal == -1){
                minVal = distance[itr];
                minValueIndex = itr;
            }
            else if( (minVal > distance[itr]) && (distance[itr] != -1) )
            {
                minVal = distance[itr];
                minValueIndex = itr;
            }
        }
    }
    return minValueIndex;
}

bool isCandidSetEmpty(bool CSet[])
{
    int itr=0;
    for(itr=0 ; itr < totalNodes ; itr++)
    {
        if(CSet[itr])
        {
            return false;
        }
    }
    return true;
}
//required variables are global..
void generalizedDijkstra(double (*heuristicCallback)(Node , Node) , int debugParam)
{

    int path[totalNodes];
    double distance[totalNodes];
    bool selected[totalNodes] , candidNodes[totalNodes];

    //initialize above variables...
    int itr=0;
    for(itr=0 ; itr < totalNodes ; itr++)
    {
        path[itr] = pathFrom;
        distance[itr] = -1;
        selected[itr] = false;
        candidNodes[itr] = false;
    }

    distance[pathFrom] = 0;
    path[pathFrom] = -1;


    do{
        int vertex = getMin(distance , selected);

        selected[vertex] = true;
        if(vertex == pathTo)
        {
            break;
        }
        candidNodes[vertex] = false;

        for(itr=0 ; itr < totalNodes ; itr++)
        {
            if( (!selected[itr]) &&
                (adjMatrix[vertex][itr] != -1) )
            {

                double heuristicValue = heuristicCallback(nodes[itr] , nodes[pathTo]);

                if(distance[itr] != -1)
                {
                    if( adjMatrix[vertex][itr] > (distance[itr]+adjMatrix[vertex][itr]+heuristicValue) )
                    {
                        distance[itr] = distance[itr]+adjMatrix[vertex][itr]+heuristicValue;
                        candidNodes[itr] = true;
                        path[itr] = vertex;
                    }
                }
                else{
                    distance[itr] = distance[vertex] + adjMatrix[vertex][itr] + heuristicValue;
                    candidNodes[itr] = true;
                    path[itr] = vertex;
                }

            }
        }

    }while(( !isCandidSetEmpty(candidNodes) ));

    printWhole(path , selected , debugParam);
}

int checkCommonNode(bool s1[] , bool s2[])
{
    int vertex = -1;
    int itr=0;
    for(itr=0 ; itr < totalNodes ; itr++)
    {
        if( (s1[itr]==true) && (s2[itr]==true) ){
            vertex = itr;
            break;
        }
    }
    return vertex;
}

int* mergePath( int pathSrc[] , int pathDest[] , int commonVert , int *totalNodesInMerge)
{

    int* path = (int*)malloc(sizeof(int)*100);

    int FinalPathSrc[100];
    int itr = 0 ;
    for(itr=0 ; itr < 100 ; itr++)
    {
        FinalPathSrc[itr] = -1;
    }

    itr=0;
    FinalPathSrc[itr] = commonVert;
    itr++;
    int pathNode = pathSrc[commonVert];
    int totalNodesInPathSrc = 0;
    while(pathNode != -1)
    {
        FinalPathSrc[itr] = pathNode;
        pathNode = pathSrc[pathNode];
        itr++;
    }
    totalNodesInPathSrc = itr;


    int FinalPathDest[100];
    itr = 0 ;
    for(itr=0 ; itr < 100 ; itr++)
    {
        FinalPathDest[itr] = -1;
    }

    itr=0;
    FinalPathDest[itr] = commonVert;
    itr++;
    pathNode = pathDest[commonVert];
    int totalNodesInPathDest = 0;
    while(pathNode != -1)
    {
        FinalPathDest[itr] = pathNode;
        pathNode = pathDest[pathNode];
        itr++;
    }
    totalNodesInPathDest = itr;

    for(itr=0 ; itr < 100 ; itr++)
    {
        path[itr] = -1;
    }

    itr = totalNodesInPathSrc-1;
    int pathItr=0;
    for( ; itr >=1 ; itr--)
    {
        path[pathItr] = FinalPathSrc[itr];
        pathItr++;
    }
    itr = 0;
    for( ; itr < totalNodesInPathDest ; itr++)
    {
        path[pathItr] = FinalPathDest[itr];
        pathItr++;
    }
    *totalNodesInMerge = 0;
    for(itr=0 ; path[itr] != -1 ; itr++)
    {
        (*totalNodesInMerge)++;
    }
    return path;

}
bool* mergeSelect(bool sSrc[] , bool sDest[])
{
    bool* selected = (bool*)malloc(sizeof(bool)*totalNodes);
    int itr=0;
    for(itr=0 ; itr < totalNodes ; itr++)
    {
        if(sSrc[itr]==true || sDest[itr]==true)
        {
            selected[itr] = true;
        }
        else{
            selected[itr] = false;
        }
    }
    return selected;
}

void optimizedDijkstra(int debugParam)
{
    int pathSrc[totalNodes] , pathDest[totalNodes];
    double distanceSrc[totalNodes] , distanceDest[totalNodes];
    bool selectedSrc[totalNodes] , candidNodesSrc[totalNodes];
    bool selectedDest[totalNodes] , candidNodesDest[totalNodes];

    int itr=0;
    for(itr=0 ; itr < totalNodes ; itr++)
    {
        pathSrc[itr] = pathFrom;
        distanceSrc[itr] = -1;
        selectedSrc[itr] = false;
        candidNodesSrc[itr] = false;

        pathDest[itr] = pathTo;
        distanceDest[itr] = -1;
        selectedDest[itr] = false;
        candidNodesDest[itr] = false;
    }
    distanceSrc[pathFrom] = 0;
    pathSrc[pathFrom] = -1;
    distanceDest[pathTo] = 0;
    pathDest[pathTo] = -1;

    int outerItr = 0 ;
    for(outerItr  = 1 ; outerItr < totalNodes ; outerItr++)
    {
        int vertexSrc = getMin(distanceSrc , selectedSrc);
        int vertexDest = getMin(distanceDest , selectedDest);

        selectedSrc[vertexSrc] = true;
        selectedDest[vertexDest] = true;
        int commonVert = checkCommonNode(selectedSrc , selectedDest);
        if(commonVert != -1)
        {

            int totalNodesInMergePath = 0;
            int* mergedPath = mergePath(pathSrc , pathDest , commonVert , &totalNodesInMergePath);
            bool* selectedCommon = mergeSelect(selectedDest , selectedSrc);
            displayDistance(mergedPath , totalNodesInMergePath , debugParam);
            displaytracePath(mergedPath , totalNodesInMergePath , debugParam);
            displayAdditionalNodes(selectedCommon , mergedPath , totalNodesInMergePath , debugParam);

            break;
        }

        candidNodesSrc[vertexSrc] = false;
        candidNodesDest[vertexDest] = false;

        for(itr=0 ; itr < totalNodes ; itr++)
        {
            if( (!selectedSrc[itr]) &&
                (adjMatrix[vertexSrc][itr] != -1) )
            {

                if(distanceSrc[itr] != -1)  // check if path detected for first time..
                {
                    if( adjMatrix[vertexSrc][itr] > (distanceSrc[itr]+adjMatrix[vertexSrc][itr]) )
                    {
                        distanceSrc[itr] = distanceSrc[itr]+adjMatrix[vertexSrc][itr];
                        candidNodesSrc[itr] = true;
                        pathSrc[itr] = vertexSrc;
                    }
                }
                else{
                    distanceSrc[itr] = distanceSrc[vertexSrc] + adjMatrix[vertexSrc][itr];
                    candidNodesSrc[itr] = true;
                    pathSrc[itr] = vertexSrc;
                }

            }


            if( (!selectedDest[itr]) &&
                (adjMatrix[vertexDest][itr] != -1) )
            {

                if(distanceDest[itr] != -1)  // check if path detected for first time..
                {
                    if( adjMatrix[vertexDest][itr] > (distanceDest[itr]+adjMatrix[vertexDest][itr]) )
                    {
                        distanceDest[itr] = distanceDest[itr]+adjMatrix[vertexDest][itr];
                        candidNodesDest[itr] = true;
                        pathDest[itr] = vertexDest;
                    }
                }
                else{
                    distanceDest[itr] = distanceDest[vertexDest] + adjMatrix[vertexDest][itr];
                    candidNodesDest[itr] = true;
                    pathDest[itr] = vertexDest;
                }

            }
        }

    }

}
void printWhole(int path[] , bool selected[] , int debugParam)
{

    int finalPath[100];
    int tempPath[100];
    int itr = 0 ;
    for(itr=0 ; itr < 100 ; itr++)
    {
        tempPath[itr] = -1;
        finalPath[itr] = -1;
    }

    itr=0;
    tempPath[itr] = pathTo;
    itr++;
    int pathNode = path[pathTo];
    int totalNodesInPath = 0;
    while(pathNode != -1)
    {
        tempPath[itr] = pathNode;
        pathNode = path[pathNode];
        itr++;
    }
    totalNodesInPath = itr;

    itr = totalNodesInPath-1;
    int tempItr=0;
    for( ; itr >=0 ; itr--)
    {
       finalPath[tempItr] = tempPath[itr];
       tempItr++;
    }

    //distance more generalized
    displayDistance(finalPath , totalNodesInPath , debugParam);

    //trace path...
    displaytracePath(finalPath , totalNodesInPath , debugParam);

    //print aditinal nodes visited, apart from required
    displayAdditionalNodes(selected ,finalPath, totalNodesInPath , debugParam);


}

void displayDistance( int finalPath[] , int totalNodesInPath , int debugParam)
{
    double dist = 0;
    int itr=0;
    for(itr=0; itr < totalNodesInPath-1 ; itr++)
    {
        int src = finalPath[itr];
        int dest= finalPath[itr+1];
        dist += adjMatrix[src][dest];
    }
    printf("Distance with solution%d : %lf\n" , debugParam , dist);
}

void displaytracePath( int finalPath[] , int totalNodesInPath , int debugParam){
    printf("Path with solution%d : " , debugParam);
    int itr = 0;
    for( ; itr <totalNodesInPath ; itr++)
    {
        printf("%d " , finalPath[itr]);
        if(itr < (totalNodesInPath-1) ){
            printf(" -> ");
        }
    }
    printf("\n");
}

void displayAdditionalNodes( bool selected[] ,int finalPath[], int totalNodesInPath , int debugParam)
{

    int itr=0, total=0;
    int arr[100] , arrItr=0;
    for(itr=0;itr<totalNodes;itr++)
    {
        int innerItr = 0;
        bool isExtra = true;
        if(selected[itr])
        {
            for(innerItr=0 ; innerItr < totalNodesInPath ; innerItr++){
                if( itr == finalPath[innerItr] ){
                    isExtra = false;
                    break;
                }
            }
        }
        else{
            isExtra = false;
        }

        if(isExtra){
            total++;
            arr[arrItr] = itr;
            arrItr++;
        }
    }
    printf("Additional nodes in the solution tree with solution%d is {%d} : ",debugParam,total);
    itr=0;
    for( ; itr < total ; itr++)
    {
        printf("%d , ", arr[itr]);
    }
    printf("\n");
}
