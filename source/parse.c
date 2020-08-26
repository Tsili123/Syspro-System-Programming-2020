#include "../include/parse.h"
//fuction that count lines of file
//this is to determine the number of values
int countlines(FILE *fp)
{
	char c;
	int count=0;
	
	 // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n') // Increment count if this character is newline 
            count = count + 1; 
	
	
	return count;
}
