#include <stdio.h>
#include <stdlib.h>
#include <iomanip>


#define	TOTAL_LETTERS	4
#define MOD		(int)pow(2.0,31)
#define TEXT_SIZE 5000
#define PATTERN_SIZE 10
#define LENGTH_OF_FILENAME 20

int		hash(char* , int , int);
int		roll(char* , int , int , int);
int		compareString(char * , char * , int , int);

int main(int argc, char **argv)
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

	char text[TEXT_SIZE], pattern[PATTERN_SIZE];
	
	if (fscanf(source, "%s", text) != EOF)
	{
		fscanf(source, "%s", pattern);
	}
	fclose(source);
	//printf("%s\n", text);
	//printf("%s\n", pattern);
	long patternHash, hashValue;
	int patternLength, textLength;

	patternLength	= strlen(pattern);
	textLength		= strlen(text);
	patternHash		= hash(pattern , 0 , patternLength);
	hashValue		= hash(text, 0, patternLength);
	
	for (int itr = 1; itr < (textLength - patternLength); itr++)
	{
		if (hashValue == patternHash)
		{
			//printf("Hash matched\n");
			int indexNumber = compareString(text, pattern, itr - 1, patternLength);
			if (indexNumber != -1)
			{
				printf("%d ," , indexNumber);
			}
		}
		hashValue = roll(text , hashValue, itr , patternLength);
	}
	printf("\n");
	return EXIT_SUCCESS;
}


int		hash(char *text, int startIndex, int width)
{
	int hashValue = 0;

	for (int itr = startIndex; itr < (startIndex + width); itr++)
	{
		hashValue = ( hashValue + ((int)text[itr] * (int)pow((double)TOTAL_LETTERS, width - (itr - startIndex) - 1)) ) % MOD;
	}
	//printf("%d\t", hashValue);
	return hashValue;
}


int		roll(char *text , int hashVal , int startIndex , int width)
{
	int hashValue	= 0;
	int previous	= text[startIndex - 1];
	int next = text[startIndex + width - 1];

	for (int itr = startIndex; itr < (startIndex + width); itr++)
	{
		hashValue = (hashValue + ((int)text[itr] * (int)pow((double)TOTAL_LETTERS, width - (itr-startIndex) -1))) % MOD;
	}

	//hashValue = ( (hashVal - ((previous * (int)pow((double)TOTAL_LETTERS, width - 1))%MOD)) + text[startIndex + width - 1]%MOD );
	//printf("%d\n",hashValue);
	return hashValue;
}


int		compareString(char *text , char *pattern , int start , int width)
{
	int matchIndex = start;

	for (int itr = start; itr < width; itr++)
	{
		if (text[itr] != pattern[itr - start])
		{
			matchIndex = -1;
			break;
		}
	}

	return matchIndex;
}