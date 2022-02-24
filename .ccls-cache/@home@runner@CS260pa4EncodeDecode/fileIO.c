#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
    //argv[1] will be "encode" or "decode" signifying the mode of the program
    //for encode mode
        //argv[2] will be the path to input text file
        //argv[3] will be the path to output code table file
        //argv[4] will be the path to output encoded text file
    //for decode mode
        //argv[2] will be the path to input code table file
        //argv[3] will be the path to input encoded text file
        //argv[4] will be the path to output decoded text file
    
    /*----------------ENCODER-----------------------*/
    /*----------------------------------------------*/
    /*To read the input text file, you might want to use a code as follows*/
    FILE *inputFile = fopen(inputTextFilePath, "r");
    if (inputFile == NULL)
    {
      printf("Could not open file to read: %s\n",inputTextFilePath);
      return;
    }

    //you will need to read the file character by character this time
    while ((c = fgetc(inputFile)) != EOF && c!='\n')
    {
      //do whatever you would like to do with this character now!!
    }
    fclose(inputFile);
    
    /*----------------------------------------------*/
    //to write the code table into the file, you might want to use a code as follows
    FILE *codeTableFile = fopen(codeTableFilePath, "w");
    if (codeTableFile == NULL)
    {
        printf("Could not open file to write: %s\n",codeTableFilePath);
        return;
    }

    // Write the code table into file:
    for(int i = numOfCharacters - 1; i >= 0; i--)
    {
      fprintf(codeTableFile, "%c,%s,%g\n", characters[i], characterCodes[i], characterFrequencies[i]);
    }
    fclose(codeTableFile);
    
    /*----------------------------------------------*/
    //To print the statistics about the compression, use print statements as follows
    printf("Original: %d bits\n", uncompressed*8); //assuming that you store the number of characters in variable "uncompressed". *8 is because ASCII table uses 8 bits to represent each character
    printf("Compressed: %d bits\n", compressed_size); //assuming that you store the number of bits (i.e., 0/1s) of encoded text in variable "compressed_size"
    printf("Compression Ratio: %.2g%%\n", (double) compressed_size/(uncompressed*8)*100); //This line will print the compression ration in percentages, up to 2 decimals.
    
    /*----------------------------------------------*/
    //to write encoded version of the text in 0/1 form into text file, you can use a code similar to fprintf statment above that is suggested for writing code table to the file.
    
    
    /*----------------DECODER-----------------------*/
    /*----------------------------------------------*/
    //When decoding, you will need to:
    //1) read code table: you can use fscanf() function, since the code table file is well structured. Alternatively, you can also use the read statements from above as was used for reading input text file.
    //2) read encoded text, which is a single line consisting of 0/1 characters: This file is better be read character by character, for which you can use a code similar to getc() code above
    //3) write the decoded text into file: for that, you can write it into the file using a code similar to fprintf() usages exemplified above.
    
}
