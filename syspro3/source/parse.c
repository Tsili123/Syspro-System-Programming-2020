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

//read one line of file
char * readline(FILE *fp)
{
    int ch;
    int i = 0;
    int buff_len = 0;
	char *buffer;
	
    buffer = malloc(buff_len + 1);
    if (!buffer) return NULL;  // Out of memory
	//read characters until change of line and put them in buffer
    while ((ch = fgetc(fp)) != '\n' && ch != EOF)
    {
        buff_len++;
        void *tmp = realloc(buffer, buff_len + 1);//increase string's size
        if (tmp == NULL)
        {
            free(buffer);
            return NULL; // Out of memory
        }
        buffer = tmp;//copy address of new string

        buffer[i] = (char) ch;//copy character
        i++;//increment index of array
    }
    buffer[i] = '\0';//end of string

    // Detect end
    if (ch == EOF && (i == 0 || ferror(fp)))
    {
    	printf("\n");
        free(buffer);
        return NULL;
    }
    return buffer;//get string
}

void parse_file(FILE *queryfp,int max_elem,char ***instructions){

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    *instructions = malloc(max_elem*sizeof(char*));
    //fclose(queryfp);
    int i=0;
    char *s;

	//get one line
	while ((read = getline(&line, &len, queryfp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
        *instructions[i] = malloc(100);
        strcpy(*instructions[i],line);
       // printf("%s\n",instructions[i]);
       if ((s = strchr(*instructions[i], '\n')) != NULL) {
        *s = '\0';
        }
        i++;
    }

    
	printf("\n\n\n\n");
    for(i=0;i< max_elem;i++){
        printf("%s\n",*instructions[i]);
    }

    

}