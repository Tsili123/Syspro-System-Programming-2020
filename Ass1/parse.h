#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct date {
   int month;
   int day;
   int year;
}date;

typedef struct MyRecord{
	char*  	recordID;
	char*	patientFirstName;
	char*	patientLastName;
	char*	diseaseID;
	char*	country;
	char*	entrydate;
	char*	exitdate;
	date dt;
}MyRecord;

int countlines(FILE * fp);
void lineByline(FILE * file,MyRecord *Elem);
char * readline(FILE *fp);
void parse_line(MyRecord *Elem,char *buffer);
void destruct_copies(int max_routes,MyRecord *Elem);

