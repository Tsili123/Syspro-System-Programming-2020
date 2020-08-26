#include "parse.h"

extern int counter;

//fuction that count lines of file
//this is to determine the number of values
int countlines(FILE * fp)
{
	char c;
	int count=0;
	
	 // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n') // Increment count if this character is newline 
            count = count + 1; 
	
	printf("%d\n",count);
	return count;
}

//read file line by line 
void lineByline(FILE * file,MyRecord *Elem)
{
	char *s;
	//get one line
	while ((s = readline(file)) != NULL){
		//parse it and get the values
	    parse_line(Elem,s);
	   	counter++;
	   	
	   	//then free it so as not to accumulate space
	    free(s);
	}
	//error check
	//printf("number of airports : %d\n",counter);
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

void parse_line(MyRecord *Elem,char *buffer)
{
	int i=counter;
	int index;
	//allocate space for strings that contain file's data
	Elem[i].country = malloc(strlen(buffer)+1);
	Elem[i].diseaseID = malloc(strlen(buffer)+1);
	Elem[i].entrydate= malloc(strlen(buffer)+1);
	Elem[i].exitdate = malloc(strlen(buffer)+1);
	Elem[i].patientFirstName = malloc(strlen(buffer)+1);
	Elem[i].patientLastName = malloc(strlen(buffer)+1);
	Elem[i].recordID = malloc(strlen(buffer)+1);
	
	//read the specific given format
	index = sscanf( buffer, "%s %[^ ] %[^ ] %[^ ] %[^ ] %[^ ] %[^ ]\n", Elem[i].recordID,  Elem[i].patientFirstName \
	, Elem[i].patientLastName,Elem[i].diseaseID  \
	,Elem[i].country , Elem[i].entrydate ,	Elem[i].exitdate );

	//error check
	if(index==0)
		printf("error\n");
	
	//printf("%s %s %s %s %s %s %s\n",Elem[i].recordID,Elem[i].patientFirstName,\
	Elem[i].patientLastName,Elem[i].diseaseID,Elem[i].country,\
	Elem[i].entrydate,Elem[i].exitdate );
	
}

//destruct array of copies
void destruct_copies(int max_routes,MyRecord *Elem)
{
	int i;
	//free strings
	for(i=0;i<max_routes;i++)
	{
		free(Elem[i].country);
		free(Elem[i].diseaseID);
		free(Elem[i].entrydate);
		free(Elem[i].exitdate);
		free(Elem[i].patientFirstName);
		free(Elem[i].patientLastName);
		free(Elem[i].recordID);
	}
}



