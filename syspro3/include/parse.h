#ifndef PARSING_HEADER
#define PARSING_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/defines.h"
//#include "../include/dir_functions.h"

int countlines(FILE *fp);

char * readline(FILE *fp);
                        
void parse_file(FILE *queryfp,int max_elem,char ***instructions);


#endif
