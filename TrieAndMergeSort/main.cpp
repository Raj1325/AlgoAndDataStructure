/*	

	Algorithm and Datastructures : Assignment 1
	
	Name		: Raj Dighe
	Login		: rpd328
	Student ID 	: 6147690 
	
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include <fstream>

#define 	MAX_WORDS 			50000
#define 	TOTAL_ALPHABETS 	26
#define 	NODE_POOL			100000

using namespace std;

typedef unsigned int uint;

//structure to store words and there count
typedef struct STRING_ARRAY
{
	static uint 	totalStrings;
	
	string	word;
	uint 	count;
	
} STRING_ARRAY;
 
 uint 	STRING_ARRAY::totalStrings = 0;

//struct represents Trie node
typedef struct TRIE_NODE
{

	char 	alphabet;
	long 	indexOfArray;
	
	/*as each alphabet can be linked to further 26 alphabets.
	create array of 26 pointers to hold the reference.*/
	struct TRIE_NODE *link[TOTAL_ALPHABETS];
	
} TRIE_NODE;

/*create a new node for TRIE , and intialize all the pointers to null,
if creation is successfull than return node address else return null.*/
TRIE_NODE*	createTrieNode	( );

/*This function will insert a new string if not present in the TRIE, and at its leaf
node it stores the index where the new string is stored inside the array, and returns
true. If the string is already present, it fetches the index of the string stored in array
and increment the count and return false.*/
bool 	insertString	( string , int );

/*
Based on count of the word sort it in descending order,
if words have the same count, it will sort alphabetically(a-z).
*/
void merge( int left , int center , int right  );
void mergeSort( int left , int right );

void initializeArray ();

TRIE_NODE	*root 	=	NULL;

STRING_ARRAY	wordArray[MAX_WORDS];  //array of struct to store words from file
STRING_ARRAY 	sortedArray[MAX_WORDS]; // this array is used while merge sorting

TRIE_NODE NODEPOOL[NODE_POOL]; // Node pool for trie
int nextNode	=	-1;

const int INVALID_ARRAY_INDEX = -1;
const char REF_CHAR 		  = 'a'; 

int main ( int argc , char **argv )
{
	
	string fileName;
	cout << "Enter File Name : ";
	cin >> fileName;
	
	ifstream source(fileName.c_str()); //create file input stream
	
	if( !source.is_open() )
	{
		cerr << " Error opening file... " << endl;
		return EXIT_FAILURE;
	}
	
	initializeArray();
	string	fileWord = "";
	char	fileChar;
	uint 	arrayIndex = 0;
	
	while( source.get( fileChar ) )  //get single character from file
	{
		
		if( !ispunct( fileChar )  ) //check weather character is punctuation
		{
			if( isalnum(fileChar) ) // check if word is formed
			{
				//append char to string.
				
				fileChar 	= tolower( fileChar );
				fileWord	+= fileChar;
				
			}
			else{
				
				if( fileWord.length() > 0)
				{
					//a word is formed
					if( insertString( fileWord , arrayIndex ) )
					{
						// increment index to store next new string
						arrayIndex++;
						STRING_ARRAY::totalStrings++;
					}
					fileWord 	= "";
				}
			}
				
		}
		
	}
	
	//sort the array of words
	mergeSort(0 , STRING_ARRAY::totalStrings-1 );
	
	cout << endl << endl;
	cout << "First 10 words with there count : " << endl;
	for(int itr = 0 ; itr < 10; itr++ )
	{
		if(itr < STRING_ARRAY::totalStrings && itr >=0){
			cout << wordArray[itr].word << "(" << wordArray[itr].count <<") ";
		}
	}
	cout << endl << endl;
	
	cout << "Last 10 words with there count : " << endl;
	int lastTenth = STRING_ARRAY::totalStrings-10;
	for(int itr = 0 ; itr < 10; itr++ )
	{
		if((lastTenth + itr) < STRING_ARRAY::totalStrings && (lastTenth + itr) >= 0){
			cout << wordArray[lastTenth + itr].word << "(" << wordArray[lastTenth+itr].count <<") ";
		}
	}
	cout << endl << endl;
	
	return EXIT_SUCCESS;	
	
}


void initializeArray ()
{
	
	for(int itr = 0; itr < MAX_WORDS ; itr++ )
	{
		wordArray[itr].word		= "";
		wordArray[itr].count 	= 0;
	}
	
}

//fetch node from node pool and initialize the node.
TRIE_NODE*	createTrieNode	( )
{
	nextNode++;
	if( nextNode < NODE_POOL ){
		
		NODEPOOL[nextNode].indexOfArray 	= INVALID_ARRAY_INDEX;
		for(int itr = 0; itr < TOTAL_ALPHABETS ; itr++ )
		{
			NODEPOOL[nextNode].link[itr] = NULL;
		}
		
		return &NODEPOOL[nextNode];
		
	}
	
	return NULL;
	
}

/*This function will insert a new string if not present in the TRIE, and at its leaf
node it stores the index where the new string is stored inside the array, and returns
true. If the string is already present, it fetches the index of the string stored in array
and increment the count and return false.*/
bool 	insertString	( string word , int newArrayIndex )
{
	bool isNewString = false;
	
	TRIE_NODE *tempNode 	= NULL;
	
	if( root == NULL )
	{
		root = createTrieNode( );
	}
	
	tempNode = root;
	
	for( int itr =0 ; itr < word.length() ; itr++ )
	{
		
		if( tempNode->link[word[itr] - REF_CHAR] == NULL )
		{
			//create new node
			tempNode->link[word[itr] - REF_CHAR] = createTrieNode();
			if(tempNode->link[word[itr] - REF_CHAR] != NULL ){
				tempNode = tempNode->link[word[itr] - REF_CHAR];
				tempNode->alphabet = word[itr];
			}
		}
		else
		{
			tempNode = tempNode->link[word[itr] - REF_CHAR];
		}
		
	}
	
	if( tempNode->indexOfArray == INVALID_ARRAY_INDEX )
	{
		//its a new string, insert this string inside array with provided index;
		wordArray[newArrayIndex].word = word;
		wordArray[newArrayIndex].count++;
		tempNode->indexOfArray = newArrayIndex;
		isNewString = true;
	}
	else
	{
		//string is already present, go to the array index where string is strored 
		//and increment the conunt
		long index = tempNode->indexOfArray;
		wordArray[index].count++;
	}	
	
	return isNewString;
}


void mergeSort( int left , int right ){

	if( left < right ){
		int center = (left+right)/2;
		mergeSort( left , center );
		mergeSort( center+1 , right );
		merge(left , center , right );
	}
	
}


void merge( int left , int center , int right )
{
	int leftItr = left;
	int rightItr = center+1;
	int tempPos 	= 0;
	int size 		= right-left;
	while( (leftItr <= center) && (rightItr <= right) )
	{
		//first try to sort using count, if they are not same.
		if( wordArray[leftItr].count != wordArray[rightItr].count )
		{
			if( wordArray[leftItr].count < wordArray[rightItr].count )
			{
				sortedArray[tempPos].count = wordArray[rightItr].count;
				sortedArray[tempPos].word = wordArray[rightItr].word;
				rightItr++;
			}
			else{
				sortedArray[tempPos].count = wordArray[leftItr].count;
				sortedArray[tempPos].word = wordArray[leftItr].word;
				leftItr++;
			}
			tempPos++;
		}
		else{
			//if the counts are same , sort alphabetically
			
			//check which is the highest length string and use  it at the top most
			//for loop
			if( wordArray[leftItr].word.length() > wordArray[rightItr].word.length() )
			{
				for( int itr=0 ; itr < wordArray[leftItr].word.length() ; itr++ )
				{
					if( itr < wordArray[rightItr].word.length()  )
					{
						if( wordArray[leftItr].word[itr] != wordArray[rightItr].word[itr] )
						{
							if( wordArray[leftItr].word[itr] > wordArray[rightItr].word[itr] )
							{
								sortedArray[tempPos].count = wordArray[rightItr].count;
								sortedArray[tempPos].word = wordArray[rightItr].word;
								rightItr++;
								tempPos++;
								break;
							}
							else{
								sortedArray[tempPos].count = wordArray[leftItr].count;
								sortedArray[tempPos].word = wordArray[leftItr].word;
								leftItr++;
								tempPos++;
								break;
							}
						}
						
					}
					else{
						sortedArray[tempPos].count = wordArray[rightItr].count;
						sortedArray[tempPos].word = wordArray[rightItr].word;
						rightItr++;
						tempPos++;
						break;
					}
				}
			}
			else
			{
				for( int itr=0 ; itr < wordArray[rightItr].word.length() ; itr++ )
				{
					if( itr < wordArray[leftItr].word.length()  )
					{
						if( wordArray[leftItr].word[itr] != wordArray[rightItr].word[itr] )
						{
							if( wordArray[leftItr].word[itr] > wordArray[rightItr].word[itr] )
							{
								sortedArray[tempPos].count = wordArray[rightItr].count;
								sortedArray[tempPos].word = wordArray[rightItr].word;
								rightItr++;
								tempPos++;
								break;
							}
							else{
								sortedArray[tempPos].count = wordArray[leftItr].count;
								sortedArray[tempPos].word = wordArray[leftItr].word;
								leftItr++;
								tempPos++;
								break;
							}
						}
						
					}
					else{
						sortedArray[tempPos].count = wordArray[leftItr].count;
						sortedArray[tempPos].word = wordArray[leftItr].word;
						leftItr++;
						tempPos++;
						break;
					}
				}
			}
		}
		
	}
	
	while( leftItr <= center )
	{
		sortedArray[tempPos].count = wordArray[leftItr].count;
		sortedArray[tempPos].word = wordArray[leftItr].word;
		leftItr++;
		tempPos++;
	}
	while(  rightItr <= right )
	{
		sortedArray[tempPos].count = wordArray[rightItr].count;
		sortedArray[tempPos].word = wordArray[rightItr].word;
		rightItr++;
		tempPos++;
	}
	
	for(int itr=0 ; itr <= size ; itr++)
	{
		wordArray[left+itr].count	= sortedArray[itr].count;
		wordArray[left+itr].word 	= sortedArray[itr].word;
	}
	
}






