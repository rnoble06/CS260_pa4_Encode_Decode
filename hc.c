#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// TODO: Implement a priority queue data structure to be used in the Huffman Coding algorithim
//-------------------------------------------------------------------------
// nodes for leaves
typedef struct leafnode
{
  int freq;                     // total freq value
  char ch;                      // character
  struct node *left, *right;    // if 0 then left, if 1 then right.
} leafNode;
// internal nodes
typedef struct node
{
  int freq;                     // total freq value
  struct node *left, *right;    // if 0 then left, if 1 then right.
} Node;

//-------------------------------------------------------------------------
// heap
typedef struct priorityQueue
{
  int capacity;
	int size;    
  struct node *PQptr;
} PQ;

// TODO: Implement a binary tree data structure to be used in the Huffman Coding algorithm
typedef struct binaryTree
{
	struct node *root;            // will be set to final node built (max total freq)
} Tree;

// I implemented a basic closed hash table for you to use. I use this for file
// interactions like reading and writing. You may use it however you please
typedef struct huffmanCodeTable
{
	int size;           // size of table
	int	*freqs;         // # of occurrances
	char	**codes;      // ascii
} HCTable;

//-------------------------------------------------------------------------
// These are called function declarations.
void	encode( FILE *plaintext, FILE *codetable, FILE *encodetxt );
void	decode( FILE *codetable, FILE *encodetxt, FILE *decodetxt );
HCTable	*initializehuffmanCodeTable();
void	insertToHCTable( HCTable *hct, char c, int f, char *code );
void	deletehuffmanCodeTable( HCTable *hct );


leafNode  *initializeLeafNode(char c, int freq);
internalNode  *initializeInternalNode(int freq);
PQ		*initializepriorityQueue();
void	deletepriorityQueue( PQ *pq );
Tree		*initializebinaryTree();
void	deletebinaryTree( Tree *bt );
void printRatio(int asciiBits, int cprBits, int cpRatio);

int main( int argc, char **argv) {
	
	char *mode;            // Encode/Decode argument
	char *plaintext_path;  // plainText.txt file path argument
	FILE *plaintext;       // plainText.txt file pointer
	char *codetable_path;  // codeTable.txt file path argument
	FILE *codetable;       // codeTable.txt file pointer
	char *encodetxt_path;  // encodedText.txt file path argument
	FILE *encodetxt;       // encodedText.txt file pointer
	char *decodetxt_path;  // decodedText.txt file path argument
	FILE *decodetxt;       // decodedText.txt file pointer

	if( argc < 5 ) {
		printf("Not Enough Arguments Supplied\n");
		return -1;
	}

	mode = argv[1];

	if( strcmp(mode, "encode") == 0 ) {
		plaintext_path = argv[2];
		codetable_path = argv[3];
		encodetxt_path = argv[4];

		plaintext = fopen(plaintext_path, "r"); // Open for reading
		codetable = fopen(codetable_path, "w"); // Open for writing
		encodetxt = fopen(encodetxt_path, "w"); // Open for writing

		if( (plaintext == NULL) ||
		    (codetable == NULL) ||
			(encodetxt == NULL) ) {
			printf("File Reading Error\n");
			return -1;
		}

		encode( plaintext, codetable, encodetxt );

		fclose( plaintext );
		fclose( codetable );
		fclose( encodetxt );

	}
	else if( strcmp(mode, "decode") == 0 ) {
		codetable_path = argv[2];
		encodetxt_path = argv[3];
		decodetxt_path = argv[4];

		codetable = fopen(codetable_path, "r"); // Open for reading
		encodetxt = fopen(encodetxt_path, "w"); // Open for writing
		decodetxt = fopen(decodetxt_path, "w"); // Open for writing

		if( (codetable == NULL) ||
			(encodetxt == NULL) ||
			(decodetxt == NULL) ) {
			printf("File Reading Error\n");
			return -1;
		}

		decode( codetable, encodetxt, decodetxt );

		fclose( codetable );
		fclose( encodetxt );
		fclose( decodetxt );
	}
	else {
		printf("Not A Valid Mode\n");
		return -1;
	}
}

//-------------------------------------------------------------------------
// TODO: You need to fill in this function
void encode( FILE *plaintext, FILE *codetable, FILE *encodetxt ) {

	PQ		*pq;     // A priority queue for the Huffman Coding algorithm
	Tree		*bt;     // A binary  tree for the Huffman Coding algorithm
	HCTable	*hct;    // A closed hash table for storing huffman values
	int		c;       // Temporary int/char used for iteration
	int	f,i;       // A temporary float variable used for iteration
  int code;
  int leafs = 0;
  int asciiBits = 0;
  int cprBits = 0;
  int cpRatio = 0;
  
  
	pq = initializepriorityQueue();
	bt = initializebinaryTree();
	hct = initializehuffmanCodeTable();

	// This loop will read the file character by character
	while( (c = fgetc(plaintext)) != EOF ) {
		// TODO: calculate the frequency of each character
		//       c is the variable that stores the character
    asciiBits++;
  
    if( hct->freqs[c] == 0 ){
      leafs=leafs+1;
      }
    f=hct->freqs[c]+1;
    insertToHCTable( hct, c, f, "-1" );
	}
  
	// TODO: Implement Huffman Coding algorithm by using the priority queue and Tree you implemented
    
  
	// TODO: Run the Huffman Coding algorithm on the set of character
	//       frequencies that you calculated to obtain the Huffman Code tree

	// TODO: Parse that tree to generate a Huffman Code table, where each
	//       character is assigned to a binary code
	//       Note: I have already created the table for you
	//       Use the insertToHCTable function to fill the table

	// This encodes the plain text file using the huffman code values and writes
	// it to the encoding file
    fseek( plaintext, 0, SEEK_SET );
	while( (c = fgetc(plaintext)) != EOF )
		fprintf( encodetxt, "%s", hct->codes[c]);

	// This writes the codetable file
	for( c = 0; c < 128; c++ ) {
		if( hct->freqs[c] != 0 )
			fprintf( codetable, "%c,%s,%d\n", c, hct->codes[c], hct->freqs[c]);
	}

	// TODO: Calculate compression statistics and print it to standard output.
  cprBits = 56;
  printRatio(asciiBits,cprBits,cpRatio);
  
  bt->root = NULL; // ***************************************************
	deletehuffmanCodeTable( hct );
	deletepriorityQueue( pq );
	deletebinaryTree( bt );
}

//-------------------------------------------------------------------------
// TODO: You need to fill in this function
void decode( FILE *codetable, FILE *encodetxt, FILE *decodetxt ) {
	HCTable	*hct;        // A closed hash table used for storing huffman values
	int		c, ce, cd;   // Temporary int/char used for iteration
	int	f;           // A temporary float variable used for iteration
	char	buf[128];    // A temporary buffer used for iteration

	hct = initializehuffmanCodeTable();

	// This reads the codetable file into a huffman code table. You can use this
	// table for the remainder of the decoding
	while( fscanf(codetable, "%c,%[^,],%d", &c, buf, &f) != EOF ){
		if ( c < 32 )
			continue;
		insertToHCTable( hct, c, f, buf );
	}

	// TODO: Read the Huffman Code Table and reconstruct the Huffman Coe tree
	//       (which will be a binary  tree!)
  
	// TODO: Implement the huffman decoding algorithm
	cd = -1;
	while( (ce = fgetc(encodetxt)) != EOF ) {
		// TODO: Decode the characters in the file using your algorithm

		// This if statement will print the decoded character to the decode file
		if( cd != -1 ) {
			fputc( cd, decodetxt );
			cd = -1;
		}
	}

	deletehuffmanCodeTable( hct );
}

void printRatio(int asciiBits, int cprBits, int cpRatio){
  asciiBits = asciiBits * 8;
  cpRatio = (int)(((float)cprBits/asciiBits)*100);
  printf("Original: %d bits\nCompressed: %d bits\nCompression Ration: %d%%\n",asciiBits, cprBits, cpRatio);
}

HCTable *initializehuffmanCodeTable() {
	// This function is self explanatory. You've seen all this before
	HCTable *hct;
	int i;

	hct = malloc( sizeof(HCTable) );
	hct->size = 128;
	hct->codes = malloc( hct->size * sizeof(char *) );
	hct->freqs = malloc( hct->size * sizeof(int) );

	for( i = 0; i < hct->size; i++ )
		hct->freqs[i] = 0;
	return hct;

  printf("initHCT: Implement me\n");
}

void insertToHCTable( HCTable *hct, char c, int f, char *code ) {
	// My "hash function" is just using the ASCII value of char c as an index
	if( hct->freqs[c] != 0 )
		free(hct->codes[c]);

	hct->freqs[c] = f;
	hct->codes[c] = malloc( sizeof(char) * strlen(code) + 1 );
	strcpy( hct->codes[c], code );
}

void deletehuffmanCodeTable( HCTable *hct ) {
	// Self explanatory, you've seen this all before
	int i;

	for( i = 0; i < hct->size; i++ )
		if( hct->freqs[i] != 0 )
			free( hct->codes[i] );

	free( hct->codes );
	free( hct->freqs );
	free( hct );
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
leafNode  *initializeLeafNode(char c, int freq){
  leafNode *myNode = malloc(sizeof(leafNode));
  myNode->ch = c;
  myNode->freq = freq;
  myNode->left = NULL;
  myNode->right = NULL;
  return myNode;
}

Node  *initializeNode(int freq, ){
  Node *myNode = malloc(sizeof(Node));
  myNode->freq = min1->freq + min2->freq;
  myNode->left = min1;
  myNode->right = min2;
  return myNode;
}

PQ *initializepriorityQueue(int leafs) {
	// TODO: Implement a priority queue data structure to be used in the Huffman Coding algorithim
  PQ *myPQ = malloc(sizeof(PQ));
  myPQ->capacity=leafs;
  myPQ->size=0;
  myPQ->PQptr=malloc(sizeof(Node)*leafs);
  return myPQ;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------



void deletepriorityQueue( PQ *myPQ ) {
	// Remember to use this function to free all the data you malloc for your priority queue
}

Tree *initializebinaryTree() {
	// TODO: Implement a binary  tree (Tree) data structure to be used in
	//       the Huffman Coding algorithm
  Tree *myTree = malloc(sizeof(Tree));
  myTree->root=NULL;              // will point to root in minHeap aka PQ
  return myTree;
}

void deletebinaryTree( Tree *bt ) {
	// Remember to use this function to free all the data you malloc for your binary  tree
  
}
