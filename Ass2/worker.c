#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include "hashstructure.h"

#define PATH "./input_dir/"

int writer_fd,reader_fd,errors=0,success=0,sig_flag=0,sig_flag2=0;

// hanle SIGINT,SIGQUIT signal
void logproc(int signum){
	if (signum == SIGINT || signum == SIGQUIT) {
	sig_flag=1;
	}else if( signum == SIGUSR1){
		
		sig_flag2=1;	
	}
	printf("inside signal handler\n");
	/*if(write(wfd,msgbuf,MSGSIZE+1) < 0) // send your answers to job executor , if you have any :P
		perror("Invalid write:");*/
}

// concatenate 2 string into 1
char* str_concatenate(const char* str1, const char* str2){
	const int length1 = strlen(str1);
	const int length2 = strlen(str2);
	char* strct = malloc((length1 + length2 + 4)*sizeof(char));
	memcpy(strct,str1,length1);
	memcpy(strct+length1,str2,length2+1);
    return strct;
}

// concatenate 2 string into 1
char* str_concatenate2(const char* str1, const char* str2){
	const int length1 = strlen(str1);
	const int length2 = strlen(str2);
	char* strct = malloc((length1 + length2 + 1)*sizeof(char));
	memcpy(strct,str1,length1);
	memcpy(strct+length1,str2,length2+1);
    return strct;
}

int compare_dates (struct date d1, struct date d2){
    if (d1.year < d2.year)
       return -1;

    else if (d1.year > d2.year)
       return 1;

    if (d1.year == d2.year){
         if (d1.month<d2.month)
              return -1;
         else if (d1.month>d2.month)
              return 1;
         else if (d1.day<d2.day)
              return -1;
         else if(d1.day>d2.day)
              return 1;
         else
              return 0;
    }
}


date parse_date(char *date){
	struct date dt;
	char *p1,*p2, prefix;
	int counte=0;
	prefix = '-';
    p1 = date;
    
	while( (p2 = strchr( p1, prefix )) != NULL ){
	    *p2 = '\0';
	    counte++;
	    if(counte==1){
	    	dt.year=atoi(p1);
		//	printf("%d\n",dt.day);	
		}
	    else if(counte==2){
	    	dt.month=atoi(p1);
	    //	printf("%d\n",dt.month);
		}
	    p1++;
	    p2++;
	    p1 = p2;
	}
	dt.day=atoi(p1);
		//	printf("%d\n",dt.year);	
	
	return dt;
}

int main(int argc,char* argv[]){
	printf("\nWorker process here,my workload is :%s",argv[0]);
	//printf("\n worker ---------\n");
	
	// Store all dirs into an array
	int i = 0,num_dir=0,limit,j=0,k=0,l=0,flag=0,hold=0,svar,flag2=0,sr;
	char **country,**date,*token,*str,*file;
	char *bufferio;
	FILE* fp;
	char buffer[150];
	int line = 1,counter=1;
	struct dirent *rdir_var;
	DIR *dir_var;
	char *position,*bufcontent;
	MyRecord *elem;
	char **directory;
	
	int sigchd=atoi(argv[4]);
	
	
    directory = malloc(1 * sizeof(char*)); // save all directories
    country = malloc(1 * sizeof(char*)); // save all countries
    date = malloc(1 * sizeof(char*)); // save all dates
    elem = malloc(1 * sizeof(MyRecord));//save record structure array
	token = strtok(argv[0],"\n"); // get directory by argument string
	
	
	//append dynamic array with directories
	do{
	  directory = realloc(directory,(num_dir+2)*sizeof(char*));
	  country = realloc(country,(num_dir+2)*sizeof(char*));
      num_dir++;
      directory[num_dir-1] = malloc((strlen(token)+1)*sizeof(char));
      limit=strlen(token)-strlen(PATH);
      country[num_dir-1] = malloc((strlen(token)+1)*sizeof(char));
      strcpy(directory[num_dir-1],token);
      strncpy(country[num_dir-1], directory[num_dir-1]+strlen(PATH), limit-1);
      //printf("country: .%s. \n", country[num_dir-1]);
     // printf("directory: .%s. \n", directory[num_dir-1]);
    }while(token = strtok(NULL,"\n"));

	//for(i=0;i<num_dir;i++){
	//	printf("country: .%s. \n", country[i]);
	//}

	if ( (reader_fd  = open(argv[2],O_RDONLY)) < 0){
		perror("fifo open problem");
		exit(3);
	}
	
	if ( (writer_fd = open(argv[1], O_WRONLY )) < 0){
		perror("fifo open problem");
		exit(3);
	}
	
	printf("buffer size is  %s\n",argv[3]);
	int buffer_size = atoi(argv[3]);
	
    // Get all the files from workers and create required structures
	for(i=0;i<num_dir;i++){ 
	// open each dir
		dir_var = opendir (directory[i]);
		if (dir_var == NULL){
			printf("%s gave  \n",directory[i]);
		    perror("lathos directory");
			continue;
		}
		
		l=k;
		
		while ((rdir_var = readdir(dir_var)) != NULL){ // open each file in dir
			hold=k;
    		file = str_concatenate(directory[i],rdir_var->d_name); // get path name
    		if(!strcmp(rdir_var->d_name,".") || !strcmp(rdir_var->d_name,".."))
    			continue;
    			
    		fp = fopen(file,"r");//we open file with absolute path
    		
    		if(j>0)
    			date = realloc(date,(j+1)*sizeof(char*));
    		date[j] = malloc(strlen(file)*sizeof(char));
			limit=strlen(file)-strlen(directory[i])+1;
			//printf("%s\n%s\n",file,directory[i]);
			//printf("limit is:%d \n",limit);
    		strncpy(date[j], file+strlen(directory[i]), limit+1);
    		//printf("date: .%s. \n", date[j]);
    				
		    if (fp){
		  		// get each line of  the file and parse the words
    			while(fgets(buffer,150,fp)){
    				
    				bufcontent = strdup(buffer); 
    				
    				if((position=strchr(bufcontent,'\n')) != NULL) // remove end line from word
    						*position = '\0';
    				//	printf("line:%s\n",bufcontent);
    				
        			str = strtok (bufcontent," \t"); // start getting words
			  		// start tokenizing document
			  		
			  		
					while (str != NULL){ // create trie word by word
					
						//char *temp = strdup(str);
						//printf("temp: %s \n",temp);	
						
			    		if(counter==1){
			    			elem[k].recordID = strdup(str);
			    			//	printf("recordID: %s \n",elem[k].recordID);
			    			
			    			elem[k].country = strdup(country[i]);
			    			//	printf("country: %s \n",elem[k].country);
						}else if(counter==2){
							char *temp = strdup(str);
							if(!strcmp(temp,"ENTER")){
								elem[k].entrydate = strdup(date[j]);
			    				//	printf("entrydate: %s \n",elem[k].entrydate);
								elem[k].exitdate = strdup("-");
			    				//	printf("exitdate: %s \n",elem[k].exitdate);	
							}else if(!strcmp(temp,"EXIT")){
								int p;
							
								for(p=l;p<k;p++){
									if(strcmp(elem[p].recordID,elem[k].recordID)==0){
										elem[p].exitdate = strdup(date[j]);
										flag=1;
									}
								}
								
								if(p<k){
								//	success++;
									//errors++;
									break;
								}
								else
								{
									
									//success++;
								//	errors++;
									//printf("ERROR\n");
									break;
									//print new line error to file 
									
									/*FILE *fp2=fp;
									fseek(fp2, 100, SEEK_END);
									fprintf(fp2, "ERROR\n");
									flag=1;*/
								}
								
								
								//	break;
									//ERROR
									
								//	elem[k].entrydate = strdup("-");
				    				//	printf("entrydate: %s \n",elem[k].entrydate);	
								//	elem[k].exitdate = strdup(date[j]);
				    				//	printf("exitdate: %s \n",elem[k].exitdate);
							}
						}else if(counter==3){
							elem[k].patientFirstName = strdup(str);
			    			//	printf("patientFirstName: %s \n",elem[k].patientFirstName);
						}else if(counter==4){
							elem[k].patientLastName = strdup(str);
			    			//	printf("patientLastName: %s \n",elem[k].patientLastName);
						}else if(counter==5){
							elem[k].diseaseID = strdup(str);
			    		//		printf("diseaseID: %s \n",elem[k].diseaseID);
						}else if(counter==6){
							elem[k].age = strdup(str);
		    			//		printf("age: %s \n",elem[k].age);
						}
			    			
			    		if ((position=strchr(str,'\n')) != NULL)
    						*position = '\0';
			    		
			    		counter++;
						str = strtok (NULL, " \t");
						//if(flag==1)	
						//	break;
			  		}
					if(flag==1){
						flag=0;
						free(bufcontent);	
			  			counter=1;
						continue;	
					}
					
					  	
			  		k++;
			  		
			  		if(k>0)
    					elem = realloc(elem,(k+1)*sizeof(MyRecord));
			  		
			  		free(bufcontent);
			  		//line++;	
			  		counter=1;
			  	}
			  	j++;
			  	rewind(fp);
		    }
			else
		        printf("Failed to open the file %s\n",file);
			
			
			if(flag2==0){
				svar=k-hold;
				flag2=1;	
			}
			if(sigchd==0){
			//printf("svar %d\n",svar);
			bool repeate[svar];//mark whether the string is unique or not
			int g=0,m=0,h=0,o=0;
			int unique=0;
			char **distemp;
			distemp = malloc(svar * sizeof(char*));
			for (g = 0; g < svar; g++)
    			distemp[g] = malloc((20) * sizeof(char)); 
			
		
			for (m=0;m < svar;m++) {
			  		//printf("%s\n",elem[m].diseaseID);
			  		
			    	for (h= m + 1;h < svar;h++) {
			    	//	printf("%s %s \n",elem[m].diseaseID,elem[h].diseaseID)	;
					    if (strcmp(elem[m].diseaseID,elem[h].diseaseID)==0) {
					      	break;
					    }
			    	}
			    	
			    	if(h==svar){
			    	//	printf("lalal\n");
			    		distemp[unique]=strdup(elem[m].diseaseID);
			    	//	printf("vrika : %s\n", elem[m].diseaseID);
						unique++;
					}
			  }
			
			//for(g=0;g<unique;g++){
			//	printf("%s\n", distemp[g]);
			//}
			
			//printf("unique %d\n",unique);
			
					
			int counters[unique][4];
			for(g=0;g<unique;g++){
				for(h=0;h<4;h++){
					counters[g][h]=0;
				}
			}
			
			for(g=0;g<unique;g++){
				for(h=0;h<svar;h++){
					if(!strcmp(distemp[g],elem[h].diseaseID)){
						if (atoi(elem[h].age)<21)
							counters[g][0]++;
						else if (atoi(elem[h].age)<41)
							counters[g][1]++;
						else if (atoi(elem[h].age)<61)
							counters[g][2]++;
						else
							counters[g][3]++;
					}
				}
			}
			
			bufferio=malloc(buffer_size*sizeof(char));
			
			sprintf(bufferio,"statistics %s %s\n",country[i],rdir_var->d_name);
			//sprintf(bufferio,"statistics\n %s PID: %d\n",directory[i],getpid());
			
			for(g=0;g<unique;g++){
				sprintf(bufferio+ strlen(bufferio),"%s \n",distemp[g]);
				for(o=0;o<4;o++){
						if (o==0)
							sprintf(bufferio+ strlen(bufferio),"age 0-20: %d\n",counters[g][0]);
						else if (o==1)
							sprintf(bufferio+ strlen(bufferio),"age 20-40: %d\n",counters[g][1]);
						else if (o==2)
							sprintf(bufferio+ strlen(bufferio),"age 40-60: %d\n",counters[g][2]);
						else
							sprintf(bufferio+ strlen(bufferio),"age 60+: %d\n",counters[g][3]);
							
				}	
			}
			
			sr = write(writer_fd,bufferio,buffer_size-10);
			
			if( sr < 0){
				perror("error in write worker");exit(5);
			}
			
			//on the fly
			printf("%s\n",bufferio);
			free(bufferio);	
			
					
			for (g = 0; g < svar; g++)
    			free(distemp[g]);
    	}
			fclose(fp);
			free(file);
			file=NULL;
		}	
		closedir (dir_var);
	}
	
	//printf("buf size %d\n",strlen(bufferio));
	//write(writer_fd,bufferio,strlen);
	//printf("%s\n",bufferio);
	//sr = write(writer_fd,bufferio,sizeof(bufferio)-10);
	
    close(writer_fd);
    if ( (writer_fd = open(argv[1], O_WRONLY )) < 0){
		perror("fifo open problem");
		exit(3);
	}
	
    //print all records
//	for(i=0;i<k;i++){
//		printf("%s %s %s %s %s %s\n",elem[i].recordID,elem[i].entrydate,elem[i].exitdate,elem[i].patientFirstName,elem[i].patientLastName,elem[i].diseaseID);
//	}
	
	printf("k is  %d\n",k);
	sleep(1);
	
    for(i=0;i<k-1;i++){
		char variable[10]="kalo";
		if(elem[i].entrydate[0]!='-'){
			strcpy(variable,elem[i].entrydate);
		 	//printf("%s\n",variable);
			elem[i].dt=parse_date(variable);
		    //printf("date: %d %d %d\n",elem[i].dt.day,elem[i].dt.month,elem[i].dt.year);	
		    //free(variable);
		}
	//printf("kal\n");
	}
	

	k=k-1;
	//num_var=k;
	
	
	
	//size of bucket default 100(hard-coded)
	
	//I need to know how many records a bucket can contain
	int rec_num=(60 - sizeof(Bucket)-8) / sizeof(Pairs);
	printf("rec num is : %d\n",rec_num);
	
	
	//create disease hashtable
	Hashtable *ht = initialize_hashtable(4,60);

	int index=0;
	for(i=0;i<k;i++){
		index=sdbm( elem[i].diseaseID);
		index=abs(index % ht->hashtable_size);
		//printf("%d\n",index);
		insert_entry_to_ht(&ht,index,elem[i].diseaseID,&elem[i],compare_dates);
	}
	
	
	//create country hashtable
	Hashtable *hc = initialize_hashtable(4,60);
	
	index=0;
	for(i=0;i<k;i++){
		index=sdbm( elem[i].country);
		index=abs(index % hc->hashtable_size);
		//printf("%d\n",index);
		insert_entry_to_ht(&hc,index,elem[i].country,&elem[i],compare_dates);
	}
    
    print_hashtable(ht);
    
    
    // Establish signal handling
	static struct sigaction act;
	act.sa_handler = logproc;
	sigfillset (&(act.sa_mask));
	sigaction(SIGINT,&act,NULL);
    sigaction(SIGQUIT,&act,NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);
    
    time_t rawtime;
  	struct tm * timeinfo;
	time ( &rawtime );
  	timeinfo = localtime ( &rawtime );
  	//printf ( "Current local time and date: %s", asctime (timeinfo) );*/
    
    //   /diseaseFrequency DIABETES 19-9-1990 20-10-2020
    
    
    printf("%s %s\n\n",argv[1],argv[2]);
    char arguments[100];
    char msgbuf[buffer_size],buf[buffer_size];
    char *dynam;
    char strm[30];
    char answer[100];
    char answer2[1000];
    while(1){
	
		// read command from job Executor
		int sr= read(reader_fd,msgbuf,buffer_size-10);
		if( sr < 0){
			perror("error in read");
			errors++;
		}
		
		
		if(strncmp(msgbuf,"/listCountries",strlen("/listCountries"))==0){
			for(i=0;i<num_dir-1;i++){
				//printf("mesa 1h edolh\n");
				sprintf(strm,"%s PID: %d PPID %d\n",directory[i],getpid(),getppid());
				if(i==0)
					strcpy(buf,strm);
				if(i==1)
					dynam=str_concatenate2(buf,strm);
				if(i>1)
					dynam=str_concatenate2(dynam,strm);
				//printf("buf %s\n",buf);
			}	
			sr= write(writer_fd,dynam,buffer_size-10);//atomic read/write
		
			//sr = write(writer_fd,buf,buffer_size+1);
			if( sr < 0){
				perror("error in read");
				errors++;
			}else{
				success++;
			}
		}else if(strncmp(msgbuf,"/diseaseFrequency",strlen("/diseaseFrequency"))==0){
			
				char arg[100];
				int flag9=0;
				//memcpy(arg,msgbuf,strlen("/diseaseFrequency")+1,40);
				strncpy( arg,msgbuf+strlen("/diseaseFrequency")+1, 60 );
				printf("mesauoa %s\n",arg);
			//	arg[strcspn(arg, "\n")] = 0;
				
				//parse virus names sscanf
				char virus[20],d1[20],d2[20],a[20],b[20],c[20],country[20];
				if ( sscanf(arg, "%[^ ] %[^ ] %[^ ] %s\n", virus,d1,d2,country)==3){
					
					printf("%s %s %s\n",virus,d1,d2);\
					
					int d,m,y,x;
					x=sscanf(d2,"%[^-]-%[^-]-%s\n", a,b,c);
					if ( x==3){
						y=atoi(a);\
						m=atoi(b);\
						d=atoi(c);
						struct date date_1,date_2;
						date_1=parse_date(d1);
						date_2=parse_date(d2);
						printf("main date d %d m %d y %d\n",d,m,y);\
						printf("main date 1 d %d m %d y %d\n",date_1.day,date_1.month,date_1.year);\
						
						if(d>0 && d<32 && m > 0 && m <13 && y >0 ){
							int result = compare_dates(date_1,date_2);
							if(result<0){
								printf("mpike\n");
								int ps =sdbm(virus);
								ps = abs(ps % ht->hashtable_size);
								flag9=print_hash_country(ht,virus,ps,date_1,date_2,compare_dates,answer);
							}else{
								printf("date 1 bigger than date 2 \n");
								fflush(stdin);
							}
						}else{
								printf("wrong date 2 \n");
								fflush(stdin);
						}	
					}else{
					//	printf("%s %s %s\n",a,b,c);
					//	d=atoi(a);\
						m=atoi(b);\
						y=atoi(c);\
						printf("%d d %d m %d y %d\n",x,d,m,y);
						printf("wrong date 2 \n");
						fflush(stdin);
					}		
				}else if(sscanf(arg, "%[^ ] %[^ ] %[^ ] %s\n", virus,d1,d2,country)==4){
					
					printf("mesa %s %s %s %s\n",virus,country,d1,d2);
					
					int d,m,y,x;
					x=sscanf(d2, "%[^-]-%[^-]-%s\n", a,b,c);
					printf("ok 4\n");
					if (x==3){
						
						y=atoi(a);\
						m=atoi(b);\
						d=atoi(c);
						struct date date_1,date_2;
						date_1=parse_date(d1);
						date_2=parse_date(d2);
						printf("main date d %d m %d y %d\n",d,m,y);\
						printf("main date 1 d %d m %d y %d\n",date_1.day,date_1.month,date_1.year);\
						
						if(d>0 && d<32 && m > 0 && m <13 && y >0 ){
							int result = compare_dates(date_1,date_2);
							if(result<0){
								printf("ela mpike\n");
								int ps =sdbm(country);
								ps = abs(ps % hc->hashtable_size);
								flag9=print_hash_country2(hc,country,virus,ps,date_1,date_2,compare_dates,answer);
							}else{
								printf("date 1 bigger than date 2 \n");
								fflush(stdin);
							}
						}else{
								printf("wrong date 2 \n");
								fflush(stdin);
						}	
					}else{
						//printf("%s %s %s\n",a,b,c);
						//y=atoi(a);\
						m=atoi(b);\
						d=atoi(c);\
						printf("%d d %d m %d y %d\n",x,d,m,y);
						printf("wrong date 2 \n");
						fflush(stdin);
					}
				}
					//send answer
					if(flag9==1){
						printf("%s\n",answer);
					}		
					else{
						strncpy(answer,"nothing found\n",strlen("nothing found\n"));
						printf("answer:%s\n",answer);	
					}
					
					sr = write(writer_fd,answer,90);//atomic read/write
		
					//sr = write(writer_fd,buf,buffer_size+1);
					if( sr < 0){
						perror("error in read");
						errors++;
					}else{
						success++;
					}
						
					flag9=0;
				
				
				
				}else if(strncmp(msgbuf,"/searchPatientRecord",strlen("/searchPatientRecord"))==0){
					char arg[100];
					int ab;
					strncpy( arg,msgbuf+strlen("/searchPatientRecord")+1, 30 );
					printf("mesauoa %s\n",arg);
					arg[strcspn(arg, "\n")] = 0;
					
					MyRecord** recptr;
					recptr = malloc(1 * sizeof(MyRecord*)); // save all dates
					int *size;
					*size=1;
					//parse record id
					search_hashtable2(&ht,arg,&recptr,&size);
					printf("size %d\n",*size);
				//	for(ab=0;ab<*size;ab++){
					if(*size>1)
						printf("worker :%s %s %s %s %s %s\n",recptr[0]->recordID,recptr[0]->patientFirstName,recptr[0]->patientLastName,recptr[0]->country,recptr[0]->entrydate,recptr[0]->exitdate);
					//}
					if(*size>1)
						sprintf(answer,"%s %s %s %s %s %s\n",recptr[0]->recordID,recptr[0]->patientFirstName,recptr[0]->patientLastName,recptr[0]->country,recptr[0]->entrydate,recptr[0]->exitdate);
					else
					strncpy(answer,"nothing found\n",strlen("nothing found\n"));
					
					sr = write(writer_fd,answer,90);//atomic read/write
						//sr = write(writer_fd,buf,buffer_size+1);
					if( sr < 0){
						perror("error in read");
						errors++;
					}else{
						success++;
					}
						
					
				}else if(strncmp(msgbuf,"/numPatientAdmissions",strlen("/numPatientAdmissions"))==0){
			
				char arg[100];
				int flag9=0;
				strncpy( arg,msgbuf+strlen("/numPatientAdmissions")+1, 60 );
				printf("mesauoa %s\n",arg);
				arg[strcspn(arg, "\n")] = 0;
				
				//parse virus names sscanf
				char virus[20],d1[20],d2[20],a[20],b[20],c[20],country[20];
				if ( sscanf(arg, "%[^ ] %[^ ] %[^ ] %s\n", virus,d1,d2,country)==3){
					
					printf("%s %s %s|\n",virus,d1,d2);\
					
					int d,m,y,x;
					x=sscanf(d2, "%[^-]-%[^-]-%s\n", a,b,c);
					if ( x==3){
								printf("mpike\n");
								search_hashtable3(&hc,virus,d1,d2,answer2);
								printf("%s\n",answer2);
					
					}else{
					//	printf("%s %s %s\n",a,b,c);
					//	d=atoi(a);\
						m=atoi(b);\
						y=atoi(c);\
						printf("%d d %d m %d y %d\n",x,d,m,y);
						printf("wrong date  \n");
						fflush(stdin);
					}		
				}else if(sscanf(arg, "%[^ ] %[^ ] %[^ ] %s\n", virus,d1,d2,country)==4){
					
						printf("%s %s %s|\n",virus,d1,d2);\
						
						int d,m,y,x;
						x=sscanf(d2, "%[^-]-%[^-]-%s\n", a,b,c);
						if ( x==3){
									printf("mpike\n");
									search_hashtable4(&hc,virus,d1,d2,answer2,country);
									printf("%s\n",answer2);
						
						}else{
						//	printf("%s %s %s\n",a,b,c);
						//	d=atoi(a);\
							m=atoi(b);\
							y=atoi(c);\
							printf("%d d %d m %d y %d\n",x,d,m,y);
							printf("wrong date  \n");
							fflush(stdin);
						}	
					}
				}else if(strncmp(msgbuf,"/numPatientDischarges",strlen("/numPatientDischarges"))==0){
			
				char arg[100];
				int flag9=0;
				strncpy( arg,msgbuf+strlen("/numPatientDischarges")+1, 60 );
				printf("mesauoa %s\n",arg);
				arg[strcspn(arg, "\n")] = 0;
				
				//parse virus names sscanf
				char virus[20],d1[20],d2[20],a[20],b[20],c[20],country[20];
				if ( sscanf(arg, "%[^ ] %[^ ] %[^ ] %s\n", virus,d1,d2,country)==3){
					
					printf("%s %s %s|\n",virus,d1,d2);\
					
					int d,m,y,x;
					x=sscanf(d2, "%[^-]-%[^-]-%s\n", a,b,c);
					if ( x==3){
								printf("mpike\n");
								search_hashtable5(&hc,virus,d1,d2,answer2);
								printf("%s\n",answer2);
					
					}else{
					//	printf("%s %s %s\n",a,b,c);
					//	d=atoi(a);\
						m=atoi(b);\
						y=atoi(c);\
						printf("%d d %d m %d y %d\n",x,d,m,y);
						printf("wrong date  \n");
						fflush(stdin);
					}		
				}else if(sscanf(arg, "%[^ ] %[^ ] %[^ ] %s\n", virus,d1,d2,country)==4){
					
						printf("%s %s %s|\n",virus,d1,d2);\
						
						int d,m,y,x;
						x=sscanf(d2, "%[^-]-%[^-]-%s\n", a,b,c);
						if ( x==3){
									printf("mpike\n");
									search_hashtable6(&hc,virus,d1,d2,answer2,country);
									printf("%s\n",answer2);
						
						}else{
						//	printf("%s %s %s\n",a,b,c);
						//	d=atoi(a);\
							m=atoi(b);\
							y=atoi(c);\
							printf("%d d %d m %d y %d\n",x,d,m,y);
							printf("wrong date  \n");
							fflush(stdin);
						}	
					}
				}else if(strncmp(msgbuf,"/exit",strlen("/exit"))==0){
					printf("Exit\n");
					break;
				}
				
		
				
		
		if(sig_flag==1){
		    FILE *lp;
			// Create log files
			char logfile[100];
			int logdir = mkdir("./logfiles",0777);
			sprintf(logfile,"./logfiles/workerid%d.txt",getpid()); // the next file where worker will write his log info
			lp = fopen(logfile, "w+");
			if (!lp)
		    	perror("logfile.txt error:");
		    
		    for(i=0;i<num_dir;i++){
				//printf("%s\n",directory[i]);	
				fprintf(lp,"%s\n",directory[i]); 
			}
			fprintf(lp,"\n\nSUCCESS %d\nFAIL %d\n",success,errors); 
			fclose(lp);	
			sig_flag=0;
			exit(10);
		}
		else if(sig_flag2==1){
			printf("USR1 catched\n");
			for(i=0;i<num_dir;i++){ 
			// open each dir
			dir_var = opendir (directory[i]);
			if (dir_var == NULL){
				continue;
			}

			while ((rdir_var = readdir(dir_var)) != NULL){ // open each file in dir
				hold=k;
	    		file = str_concatenate(directory[i],rdir_var->d_name); // get path name
	    		if(!strcmp(rdir_var->d_name,".") || !strcmp(rdir_var->d_name,".."))
	    			continue;
	    			
	    		fp = fopen(file,"r");//we open file with absolute path
	    		
				  struct stat attr;
    			  stat(file, &attr);
				  if(difftime(attr.st_mtime,rawtime)>0){//IMPORTANT I compare time of creation
				  //if if its greater than the current time, that means that SIGUSR1 was received 
				  	
				  printf("inside new file process SIGUSR1\n");
				  printf("file:%s\n",file);	
				  
				  write(writer_fd,file,300);//atomic read/write				
				}
			}
		}sig_flag2=0;
	}
		
		for(i=0;i<buffer_size;i++){
			msgbuf[i]='\0';
			buf[i]='\0';
		}
		
		for(i=0;i<30;i++){
			strm[i]='\0';
		}
		//printf("%s\n",dynam);
		//char* pch = strtok(msgbuf,"\n"); // get comman
		//printf("ola kala %s %d \n",pch,getpid());
		//strcpy(buf,pch);
		//printf("msgbuf %s \n",buf);
		free(dynam);
		dynam=NULL;
	}
    
	/*
    //EXIT CONTROL
	
    for (i = 0; i < num_dir; i++){
		free(directory[i]);
    }
   
    for(i=0;i<j;i++){
    	date[i];
	}
    
	close(reader_fd);
	close(writer_fd);*/

    
    return 0;
}

