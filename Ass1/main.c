#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "hashtable.h" 


//needs
//error check of input file

int counter=0;


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
	    	dt.day=atoi(p1);
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
	dt.year=atoi(p1);
		//	printf("%d\n",dt.year);	
	
	return dt;
}


int main(int argc,char **argv){
	
	int i,size_hd=0,size_hc=0,size_b=0,count=0;
	FILE * f;
	char * buffer = 0 , *p1= 0,*p2 = 0;
	int length,counter=0,j=0;
	char c = 0;
	char line[100],arguments[100];
	char prefix = ' ';
	char *name = "test.txt",*name2 = "tests2.txt";
	
	for(i=1;i<argc;i++){
		if(strcmp(argv[i], "-p")==0){
			f= fopen (argv[i+1],"rb");
		}else if(strcmp(argv[i], "-h1")==0){
			size_hd=atoi(argv[i+1]);
		}else if(strcmp(argv[i], "-h2")==0){
			size_hc=atoi(argv[i+1]);
		}else if(strcmp(argv[i], "-b")==0){
			size_b=atoi(argv[i+1]);
		}
	}
	
	int max_elem=countlines(f);
	rewind(f);
	MyRecord *elem = malloc(max_elem*sizeof(MyRecord));
	lineByline(f,elem);


//	for(i=0;i<max_elem;i++){\
		printf("%s %s %s %s %s %s %s\n",elem[i].recordID,elem[i].patientFirstName,\
		elem[i].patientLastName,elem[i].diseaseID,elem[i].country,\
		elem[i].entrydate,elem[i].exitdate );\
	}\
	
	for(i=0;i<max_elem;i++){
		char variable[9];
		if(elem[i].entrydate[0]!='-'){
			strcpy(variable,elem[i].entrydate);
		  //  printf("%s\n",variable);
			elem[i].dt=parse_date(variable);
		    //printf("date: %d %d %d\n",elem[i].dt.day,elem[i].dt.month,elem[i].dt.year);	
		}
	}
	
	//I need to know how many records a bucket can contain
	int rec_num=(size_b - sizeof(Bucket)-8) / sizeof(Pairs);
	
	
	//create disease hashtable
	Hashtable *ht = initialize_hashtable(k,size_b);

	int position=0;
	for(i=0;i<max_elem;i++){
		position=sdbm( elem[i].diseaseID);
		position=abs(position % ht->hashtable_size);
		//printf("%d\n",position);
		insert_entry_to_ht(&ht,position,elem[i].diseaseID,&elem[i],compare_dates);
	}
	//create country hashtable
	Hashtable *hc = initialize_hashtable(size_hc,size_b);
	position=0;
	for(i=0;i<max_elem;i++){
		position=sdbm( elem[i].country);
		position=abs(position % hc->hashtable_size);
		//printf("%d\n",position);
		insert_entry_to_ht(&hc,position,elem[i].country,&elem[i],compare_dates);
	}
	
	printf("\n\n\nThe bucket can contain %d records\n",rec_num);

	arguments[0]=0;
	while( fgets ( line, 100, stdin ) != NULL && strncmp(line,"/exit",strlen("/exit")) != 0){
		printf( "%s" , line );
		if(strncmp(line,"/globalDiseaseStats",18)==0){
			//printf("mpike\n");
			memcpy(arguments,&line[20],20);
			if(arguments[0]=='\n'||arguments[0]==0){
				printf("no arguments\n");
				print_hashtable(ht);
			}else{
				printf("%s\n",arguments);
				char date1[10],date2[10];
				strncpy(date1,arguments,9);
				strncpy(date2,arguments+10,9);
				//printf("date1 : %s\n",date1);
				//printf("date2 : %s\n",date2);
				
				//printf("date1 : %d %d %d\n",date_1.day,date_1.month,date_1.year);
				//printf("date1 : %d %d %d\n",date_2.day,date_2.month,date_2.year);
				char a[20],b[20],c[20];
				int d,m,y;
				if ( sscanf(date2, "%[^-]-%[^-]-%s\n", a,b,c)==3){
					d=atoi(a);\
					m=atoi(b);\
					y=atoi(c);\
					struct date date_1,date_2;
					date_1=parse_date(date1);
					date_2=parse_date(date2);
					printf("main date d %d m %d y %d\n",d,m,y);\
					
					
					if(d>0 && d<32 && m > 0 && m <13 && y >0 ){
						int result = compare_dates(date_1,date_2);
						if(result<0){
							print_hashtable2(ht,date_1,date_2,compare_dates);	
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
					// /globalDiseaseStats 23-5-2005 24-4-2018//23-3-2005 24-4-2015
					// /diseaseFrequency SARS-COV-2 23-5-2005 24-4-2018
					// /diseaseFrequency SARS-COV-2 Denmark 23-5-2005 24-4-2018
					//./my_program -p file.txt -h1 10 -h2 100 -b 102
					// /diseaseFrequency lathos 23-5-2005 24-4-2018
			}
		}else if(strncmp(line,"/diseaseFrequency",17)==0){
				
				memcpy(arguments,&line[18],40);
				printf("a%sa\n",arguments);
				//parse virus names sscanf
				char virus[20],d1[20],d2[20],a[20],b[20],c[20],country[20];
				if ( sscanf(arguments, "%[^ ] %[^ ] %[^ ] %s\n", virus,d1,d2,country)==3){
					
					printf("%s %s %s\n",virus,d1,d2);\
					
					int d,m,y,x;
					x=sscanf(d2, "%[^-]-%[^-]-%s\n", a,b,c);
					if ( x==3){
						d=atoi(a);\
						m=atoi(b);\
						y=atoi(c);
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
								print_hash_country(ht,virus,ps,date_1,date_2,compare_dates);
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
				}else if(sscanf(arguments, "%[^ ] %[^ ] %[^ ] %s\n", virus,country,d1,d2)==4){
					
					printf("mesa %s %s %s %s\n",virus,country,d1,d2);
					
					
					int d,m,y,x;
					x=sscanf(d2, "%[^-]-%[^-]-%s\n", a,b,c);
					if ( x==3){
						d=atoi(a);\
						m=atoi(b);\
						y=atoi(c);
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
								print_hash_country2(hc,country,virus,ps,date_1,date_2,compare_dates);
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
				}		
		}else if(strncmp(line,"/topk-Countries",strlen("/topk-Countries"))==0){
			
		}
		else if(strncmp(line,"/insertPatientRecord",strlen("/insertPatientRecord"))==0){
			char recid[10],patf[15],patl[20],disid[10],country[15],entrydate[6],exitdate[6];
			memcpy(arguments,&line[strlen("/numCurrentPatients")+2],100);
			// /insertPatientRecord 20100 ARIS KOSNSTANTINOU SARS-COV-2 NETHERLANDS 23-9-2019 20-10-2020
			//20100 ARIS KOSNSTANTINOU FLU-2011 NETHERLANDS 23-9-2019 20-10-2020
			arguments[ strlen(arguments) - 1 ] = '\0';
			printf("%s\n",arguments);
			MyRecord *rd = malloc(sizeof(MyRecord));
			
			rd->country = malloc(strlen(arguments)+1);
			rd->diseaseID = malloc(strlen(arguments)+1);
			rd->entrydate= malloc(strlen(arguments)+1);
			rd->exitdate = malloc(strlen(arguments)+1);
			rd->patientFirstName = malloc(strlen(arguments)+1);
			rd->patientLastName = malloc(strlen(arguments)+1);
			rd->recordID = malloc(strlen(arguments)+1);
			
			//read the specific given format
			 sscanf( arguments, "%s %[^ ] %[^ ] %[^ ] %[^ ] %[^ ] %[^ ]\n", rd->recordID,  rd->patientFirstName \
			, rd->patientLastName,rd->diseaseID  \
			,rd->country , rd->entrydate ,	rd->exitdate );
			
			printf("%s %s %s %s %s %s %s\n",rd->recordID,rd->patientFirstName,\
				rd->patientLastName,rd->diseaseID,rd->country,\
				rd->entrydate,rd->exitdate );\
				
			int pst1 = sdbm( rd->diseaseID);
			int pst2 = sdbm( rd->country);
			
			pst1=abs(pst1 % ht->hashtable_size);
			pst2=abs(pst2 % hc->hashtable_size);
			
			insert_entry_to_ht(&ht,pst1,rd->diseaseID,rd,compare_dates);
			insert_entry_to_ht(&hc,pst2,rd->country,rd,compare_dates);
			
			int fd2;
			    
			    fd2 = open(name2, O_WRONLY | O_CREAT, 0644);
			    if (fd2 == -1) {
			        perror("open failed");
			        exit(1);
			    }
			
			    if (dup2(fd2, 1) == -1) {
			        perror("dup2 failed"); 
			        exit(1);
			    }
			
			    // file descriptor 1, i.e. stdout, now points to the file
			    // "test.txt" which is open for writing
			    // You can now use printf which writes specifically to stdout
			    print_hs(ht);
			    printf("\n\n\n\n\n");
			    print_hs(hc);
			    
				//for(i=0;i<max_elem;i++){\
				printf("%s %s %s %s %s %s %s\n",elem[i].recordID,elem[i].patientFirstName,\
				elem[i].patientLastName,elem[i].diseaseID,elem[i].country,\
				elem[i].entrydate,elem[i].exitdate );\
				}
			
			
			    //close(1);
			    close(fd2);
			
		}
		else if(strncmp(line,"/recordPatientExit",strlen("/recordPatientExit"))==0){
			memcpy(arguments,&line[strlen("/numCurrentPatients")],40);
			char recid[20],exitdate[20];
			
			int x = sscanf(arguments, "%[^ ] %[^ ]\n", recid,exitdate);
			
			if(x==2){
				printf("mpike2\n");
				exitdate[ strlen(exitdate) - 1 ] = '\0';
				printf("%s\n",exitdate);
			//	printf("%s %s\n",recid,exitdate);
				print_exitdate(ht,recid,exitdate);
				printf("mpike3\n");// /recordPatientExit 561 23-10-2019
				
							
				int fd;
			    
			    fd = open(name, O_WRONLY | O_CREAT, 0644);
			    if (fd == -1) {
			        perror("open failed");
			        exit(1);
			    }
			
			    if (dup2(fd, 1) == -1) {
			        perror("dup2 failed"); 
			        exit(1);
			    }
			
			    // file descriptor 1, i.e. stdout, now points to the file
			    // "test.txt" which is open for writing
			    // You can now use printf which writes specifically to stdout
				for(i=0;i<max_elem;i++){\
				printf("%s %s %s %s %s %s %s\n",elem[i].recordID,elem[i].patientFirstName,\
				elem[i].patientLastName,elem[i].diseaseID,elem[i].country,\
				elem[i].entrydate,elem[i].exitdate );\
				}
			
			
			    //close(1);
			    close(fd);
			}
			
		}else if(strncmp(line,"/numCurrentPatients",strlen("/numCurrentPatients"))==0){
			//current patients are those that havent exited hospital(means exit date is char '-')
			memcpy(arguments,&line[strlen("/numCurrentPatients")+1],40);
			if(arguments[0]=='\n'||arguments[0]==0){
				printf("no arguments\n");
				print_hashtable3(ht);
			}else{
				printf("%s\n",arguments);
				char did[20];
				strcpy(did,arguments);
				//if(did != NULL)\
  					strncpy(did, "\0", 1);
  				did[ strlen(did) - 1 ] = '\0';
				printf("%s\n",did);
				int ps =sdbm(did);
				ps = abs(ps % ht->hashtable_size);
				print_hashtable4(ht,did,ps);
			}
		}
		arguments[0]=0;
		memset(line, 0, sizeof(line));	
		memset(arguments, 0, sizeof(arguments));
	}
	fflush(stdin);
	//delete allocated memory 
	//destruct_copies(max_elem,elem);
	delete_ht(&ht);
	//delete_ht(&hc);
	
	
//	/insertPatientRecord 20100 ARIS KOSNSTANTINOU SARS-COV-2 NETHERLANDS 23-9-2019 20-10-2020
	  
	// /globalDiseaseStats 23-5-2005 24-4-2018//23-3-2005 24-4-2015
					// /diseaseFrequency SARS-COV-2 23-5-2005 24-4-2018
					// /diseaseFrequency SARS-COV-2 Denmark 23-5-2005 24-4-2018
					//./DiseaseMonitor -p file.txt -h1 10 -h2 100 -b 102
					// /diseaseFrequency lathos 23-5-2005 24-4-2018
					
	/*
	
	printf("\n\n\n\n\n\n\n");
	for(j=0;j<count;j++){
		
		
	}
	/*char variable[8];

		    	if(array[j].entrydate[0]!='-'){
		    		array[j].date_var=parse_date(variable);
		    		printf("date: %d %d %d\n",array[j].date_var.day,array[j].date_var.month,array[j].date_var.year);	
				}
	*/
		    	
	//printBucket(ht);
	

	
	//printf("%s\n",variable.exitdate);
	
	
	//./my_program -p file.txt -h1 100 -h2 100 -b 1024
    //printf("%s \n",  buffer); 
    return 0;
}



