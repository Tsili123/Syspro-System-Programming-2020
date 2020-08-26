#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h> 
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#define PATH "./input_dir/"
//"/home/users1/sdi1700170/syspro2/input_dir/"


// Global variables 
int workers_num = -1; // number of workers
int* workers;  	// will hold the id process of each worker
char** work_amount; // will store the amount of work each worker has to do
int* writer_fd,*reader_fd;	// will hold the pipe's descriptors ( read-write )
int buffersz,killed,sigchd=0,dir_num=0;
/*
int first; // Printing menu with proper menu
int killed;*/
int flag2=0;



void sighandler(int signum){
	char buffer[buffersz];
	int i,flag=0;
	if(signum==SIGUSR1){
		while(1){
		
			for(i=0; i< workers_num; i++){
				char name[30];
				//sprintf(name,"file%d",i);
				//Files[i]=fopen(name, "w");
				int l;
				char c;
				while (read(reader_fd[i], buffer, buffersz-10) > 0) {
				    //fprintf(Files[i],"%s",exchange_buffer);
				    //dup2(reader_fd[i], 1);
				    printf("parent read ok\n");
				    printf("%s\n", buffer);
				    flag=1;
				    break;
					//fflush(stdout);
				}
				if(flag==1)
					break;	
			}
			if(flag==1){
				flag=0;
				break;	
			}
		}
	}else if (signum == SIGINT || signum == SIGQUIT){
		int i;
		printf("SIGINT/SIGQUIT received on parent process\n");
		char buffer[buffersz];
		flag2=1;
		//errors 
		fflush(stdout);
		while(flag2==1){
			if(flag2==1){
				
				for(i=0; i< workers_num; i++){
					 close(reader_fd[i]);
					 close(writer_fd[i]);	
				}
				
				FILE *lp;
				// Create log files
				//sprintf(logfile,"logfile.txt",getpid()); // the next file where worker will write his log info
				int filedesc = open("logfile.txt", O_WRONLY | O_APPEND);
				//if (!lp)
			    //	perror("logfile.txt error:");
			    
			    write(filedesc,"ok",3);
			    
				for(i=0;i<dir_num;i++){
					printf("%s\n",work_amount[i]);	
					//fprintf(lp,"%s\n",work_amount[i]); 
					//fwrite(work_amount,sizeof(work_amount[i]),1,lp);
					write(filedesc,work_amount[i],strlen(work_amount[i]));
				}
				//close(filedisc);
				//errors    	
				exit(12);
			}
	}
		
		
			for (i= 0; i <workers_num; i++)
			   	kill(workers[i],SIGKILL); // inform each worker that their time is up
			   	
	}

}

void createproc(int sig){
	if(killed){ // invoked if child was killed
		sigchd=1;
		pid_t pid;
	  	pid = wait(NULL); // catch which process sent the signal
	  	int a = 0,i;
	  	for (i = 0; i < workers_num;i++){ // find the worker
	  		if(workers[i] == pid)
	  			break;
	  		a++;
	   	}
	 	
	 	printf("Worker %d with pid %d exited.\nBut I will create it again\n",a,pid);

	   	int workerid;
		if((workerid = fork())== 0){ // Create him again
			char reader[30];
			char writer[30];
			sprintf(reader,"worker%d reader",a);	
			sprintf(writer,"worker%d writer",a);
			char bufvar[12];
			char bufvar2[12];
			sprintf(bufvar, "%d", buffersz);
			sprintf(bufvar2, "%d", sigchd);
			execl("./worker",work_amount[a],reader,writer,bufvar,bufvar2,NULL);
			exit(15);
		}

		workers[a] = workerid; // store the new one
		printf("Done\n");
	 	// open up again the pipeline 
	 	char name[20];
		sprintf(name,"worker%d reader",a);
		if( (reader_fd[a] = open(name,O_RDONLY)) < 0){
				perror("fifo open problem");
				exit(3);
		}
		sprintf(name,"worker%d writer",a);
		if( (writer_fd[a] = open(name,O_WRONLY)) < 0){
			perror("fifo open problem");
			exit(3);
		}
	  	printf("Alright\n");
	}
	killed=0;
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



int main(int argc,char *argv[]){
	
	int i,j,n,status,exit_status,input_flag=0,buffer_size,num_dir=0,clength;
 	DIR *id; // input directory
 	struct dirent *idir;
 	char **work_root;
 	pid_t worker_pid;
	pid_t aggegrator_pid;
		

 	// get arguments
	for(i = 0; i < argc; i++){ 
		if(!strcmp(argv[i],"-w"))//get number of workers
			workers_num = atoi(argv[++i]);
		if(!strcmp(argv[i],"-b"))
			buffer_size = atoi(argv[++i]);//get buffer size of pipe
		if(!strcmp(argv[i],"-i")){
			// Ensure we can open directory
			id = opendir (argv[++i]);
	        if (id == NULL) {
	            printf ("Cannot open directory '%s'\n", argv[i]);
	            return 1;
	        }
				
			// Process each entry
			while ((idir = readdir(id)) != NULL) {
            	//printf ("[%s]\n", idir->d_name);
            	if (idir->d_name[0] != '.' && idir->d_name[strlen(idir->d_name)-1] != '~') {
            		num_dir++;
        			puts(idir->d_name);
    			}
        	}
        	 
			/*id = fopen(argv[++i], "rb"); // open our file
			if ( id == NULL ){
				printf("Error opening file\n"); // error handling
				return -1;
			}
			*/
			input_flag = 1 ; // ready for processing
		}
	}
	rewinddir(id);
	buffersz=buffer_size;
	
	if(workers_num < 0){
		printf("Give number of workers\n");
		exit(1);
	}
	
	if(input_flag != 1){
		printf("Please give input file\n"); // error handling
		exit(2);
	}
	
	dir_num=num_dir;
	
	if(workers_num > num_dir) // if we have abunance of workers , we dont need them
		workers_num=workers_num-(workers_num-num_dir);
	
	// Balance workload and assign it to workers
	
	int work[workers_num]; // array with the number of directories each worker will get
	for(i=0;i<workers_num;i++)
		work[i] = 0;
		
	int worker = 0;//save number of directories
	while(num_dir>0){ // count them the number of directories they'll get
		work[worker]++;
		num_dir--;
		worker=(worker+1)%workers_num;
	}
	
	for(i=0;i<workers_num;i++)
		printf("%d\n",work[i]);
	
	size_t len = 0,lenght=0,str_var=(strlen(PATH)+1);
	work_root = malloc(str_var*sizeof(char*)); 
	work_amount = malloc(str_var*sizeof(char*)); // holds the directories for each worker
	
	for(i=0;i<str_var;i++){
		work_root[i]=PATH;
		//printf("%s\n",work_root[i]);
	}
	
	//skip parent and current directory
	readdir(id);
	readdir(id);
	char *c="/\n";
	
	for(i=0;i<workers_num;i++){
		char* temp;
		for(j=0;j<work[i];j++){
			idir = readdir(id);
			temp = str_concatenate(work_root[i],idir->d_name);
			strncat(temp, c, 2);
			printf("%d:%s",i,temp);
			if(j==0){
				work_amount[i]  = temp;
			}
			else{
				work_amount[i]  = str_concatenate(work_amount[i],temp);
			}
			//printf("%d:%s",i,work_amount[i]);
			//printf("%s\n",idir->d_name);
			//work_amount[i]  = str_concatenate(work_root[i],idir->d_name);
			// append c to string
		}
	}
	/*for(i=0;i<workers_num;i++){
		for(j=0;j<work[i];j++){
			idir = readdir(id);
			//printf("%s\n",idir->d_name);
			work_amount[i]  = str_concatenate(work_root[i],idir->d_name);
			// append c to string
    		strncat(work_amount[i], c, 2);
			printf("%d:%s",i,work_amount[i]);
		}
	}
*/
	
	//create pipes
	// Create workers and open up the pipeline
	printf("Creating pipeline...\n");
	for(i=0;i<workers_num;i++){
		char pipe_name[30];
		sprintf(pipe_name,"worker%d reader",i);
		if(mkfifo(pipe_name,0666) == -1){
			perror("error: \n");
		}
		puts(pipe_name);
		sprintf(pipe_name,"worker%d writer",i);
		if(mkfifo(pipe_name,0666) == -1){
			perror("error: \n");
		}
		puts(pipe_name);
	}
	
	workers = malloc(workers_num*sizeof(int)); // store the process id of each workers
	
	//create processes
	for(i=0; i<workers_num; i++){
		sleep(1);
		if((worker_pid=fork()) == 0 ) // create workers 
			break;
		workers[i] = worker_pid; // store worker's id
	}
	
	
	if(worker_pid==0){ // worker process
		//start worker process and pass pipe names
		char reader_pipe[30];
		char writer_pipe[30];
		sprintf(reader_pipe,"worker%d reader",i);	
		sprintf(writer_pipe,"worker%d writer",i);
		char bufvar[12];
		char bufvar2[12];
		sprintf(bufvar, "%d", buffer_size);
		sprintf(bufvar2, "%d", sigchd);
		execl("./worker",work_amount[i],reader_pipe,writer_pipe,bufvar,bufvar2,(char*)0); 
		exit(3);//error code 3
	}
	else{ 
		/*if((worker_pid=wait(&status)) == -1){
			perror("wait failed");
			exit(4);
		}*/
			
		printf("Aggegrator with pid %d \n",getpid());
		
	/*	if(WIFEXITED(status)){
			exit_status = WEXITSTATUS(status);
			printf("exit status from %d was %d\n",worker_pid,exit_status);
		}*/
	//	sleep(1);
	}
	
	// open pipelines
	writer_fd = malloc(workers_num*sizeof(int)); // store the write file descriptors
	reader_fd = malloc(workers_num*sizeof(int)); // store the reader file descriptors
	
	char reader_pipe[30];
		char writer_pipe[30];
		//sprintf(reader_pipe,"worker%d reader",0);	
		//sprintf(writer_pipe,"worker%d writer",0);
	// open pipelines
	printf("Open pipeline...\n");
	
//	
	/*writer_fd[0] = open("worker0 writer", O_WRONLY);
	reader_fd[0] = open("worker0 reader", O_RDONLY);
	writer_fd[1] = open("worker1 writer", O_WRONLY);
	reader_fd[1] = open("worker1 reader", O_RDONLY);*/
	
	for(i=0; i< workers_num; i++){
		char name[30];
		
		sprintf(name,"worker%d writer",i);
		printf("%s\n",name);
		if((writer_fd[i] = open(name,O_WRONLY)) < 0){
			perror("error: writer\n");
			exit(5);
		}
		
		sprintf(name,"worker%d reader",i);
		printf("%s\n",name);
		if((reader_fd[i] = open(name,O_RDONLY)) < 0){
			perror("error: reader \n");
			exit(6);
		}
	}
	
	char command[buffer_size];
	char exchange_buffer[buffer_size];
	//memset( exchange_buffer, 0, buffer_size );
	//printf("eimai ston parent\n");
	
	//receive results from workers
	FILE** Files = malloc(sizeof(FILE*) * workers_num);
	
	
		for(i=0; i< workers_num; i++){
			char name[30];
			sprintf(name,"file%d",i);
			Files[i]=fopen(name, "w");
			printf("parent read ok\n");
			int l;
			char c;
			while (read(reader_fd[i], exchange_buffer, buffer_size-10) > 0) {
			    fprintf(Files[i],"%s",exchange_buffer);
			    //dup2(reader_fd[i], 1);
			    printf("%s", exchange_buffer);
				//fflush(stdout);
			}
		}
		
		
		
    printf("kalws \n");
    // Establish signal handling
    
    
	static struct sigaction act;
	act.sa_handler = sighandler;
	sigfillset (&(act.sa_mask));
	sigaction(SIGINT,&act,NULL);
    sigaction(SIGQUIT,&act,NULL);
    sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);
	signal(SIGCHLD,createproc);
	
	killed =1;
	
	
	printf("please insert command:\n");
	
	while(fgets ( command, buffer_size, stdin ) != NULL && strncmp(command,"/exit",strlen("/exit")) != 0){
		
		
		
		printf("parent process command:  %s\n",command);
		//char *temp = strdup (command);
		//char* pch = strtok(temp," "); // get command
		
		
		if(strncmp(command,"/listCountries",strlen("/listCountries"))==0){
			printf("First instrunction detected\n");
			
			//send instruction to every worker
			for(i=0; i< workers_num; i++){
				printf("parent write ok\n");
				write(writer_fd[i],command,buffer_size-10);
			}
			
			//receive results from workers
			for(i=0; i< workers_num; i++){
				printf("parent read ok\n");
				int sp =read(reader_fd[i],exchange_buffer,buffer_size-10);
				//if(sp>0)
					printf("reader results from worker %d :\n%s\n",i,exchange_buffer);
			}
			
		}if(strncmp(command,"/diseaseFrequency",strlen("/diseaseFrequency"))==0){
			//send instruction to every worker
			printf("lalalal \n");
			for(i=0; i< workers_num; i++){
				printf("parent write ok\n");
				write(writer_fd[i],command,strlen(command)+1);
			}
			
			//receive results from workers
			for(i=0; i< workers_num; i++){
				printf("parent read ok\n");
				int sp =read(reader_fd[i],exchange_buffer,buffer_size-10);
				//if(sp>0)
					printf("reader results from worker %d :\n%s\n",i,exchange_buffer);
			}
		}else if(strncmp(command,"/searchPatientRecord",strlen("/searchPatientRecord"))==0){
			//send instruction to every worker
			printf("lalalal \n");
			
			for(i=0; i< workers_num; i++){
				printf("parent write ok\n");
				write(writer_fd[i],command,strlen(command)+1);
			}
			
			//receive results from workers
			for(i=0; i< workers_num; i++){
				printf("parent read ok\n");
				int sp =read(reader_fd[i],exchange_buffer,buffer_size-10);
				//if(sp>0)
					printf("reader results from worker %d :\n%s\n",i,exchange_buffer);
			}
		}else if(strncmp(command,"/numPatientAdmissions",strlen("/numPatientAdmissions"))==0){
			//send instruction to every worker
			printf("lalalal \n");
			
			for(i=0; i< workers_num; i++){
				printf("parent write ok\n");
				write(writer_fd[i],command,strlen(command)+1);
			}
			
		/*	//receive results from workers
			for(i=0; i< workers_num; i++){
				printf("parent read ok\n");
				int sp =read(reader_fd[i],exchange_buffer,buffer_size-10);
				//if(sp>0)
					printf("reader results from worker %d :\n%s\n",i,exchange_buffer);
			}*/
		}else if(strncmp(command,"/numPatientDischarges",strlen("/numPatientDischarges"))==0){
			//send instruction to every worker
			printf("lalalal \n");
			
			for(i=0; i< workers_num; i++){
				printf("parent write ok\n");
				write(writer_fd[i],command,strlen(command)+1);
			}
			
		/*	//receive results from workers
			for(i=0; i< workers_num; i++){
				printf("parent read ok\n");
				int sp =read(reader_fd[i],exchange_buffer,buffer_size-10);
				//if(sp>0)
					printf("reader results from worker %d :\n%s\n",i,exchange_buffer);
			}*/
		}else if(strncmp(command,"/exit",strlen("/exit"))==0){
			//send instruction to every worker
			printf("lalalal \n");
			
			for(i=0; i< workers_num; i++){
				printf("parent write ok\n");
				write(writer_fd[i],command,strlen(command)+1);
			}
			
			//wait all children to finish
			int poid;
			while ((poid = wait(&status)) > 0);
			
			FILE *lp;
			// Create log files
			char logfile[100];
			int logdir = mkdir("./logfiles",0777);
			sprintf(logfile,"./logfiles/parent%d.txt",getpid()); // the next file where worker will write his log info
			lp = fopen(logfile, "w+");
			if (!lp)
		    	perror("logfile.txt error:");
		    
		    for(i=0;i<num_dir;i++){
        			fprintf(lp,"%s\n",work_amount[i]);
    			}
        	
		
				
				
			//fprintf(lp,"\n\nSUCCESS %d\nFAIL %d\n",success,errors); 
			fclose(lp);	
			
		}
	
			for(i=0;i<buffer_size;i++)
				command[i]='\0';
		printf("please insert command:\n");
	}


	//printf("nuum dir %d\n",num_dir);
	/*
	for(i=1;i<n;i++)
		if((childpid = fork())<=0)
			break;
		else{
		//we are in parent process
			
			if((childpid=wait(&status)) == -1){
				perror("wait failed");
				exit(2);
			}
			
			
			if(WIFEXITED(status)){
				exit_status = WEXITSTATUS(status);
				printf("exit status from %d was %d\n",childpid,exit_status);
			}
		}
	
	printf("i: %d process ID: %ld parent ID:%ld child ID:%ld\n",i,(long)getpid(),(long)getppid(),(long)childpid);*/
			
			
	// Close directory 	
	// free and closes
//	free(command);
	closedir (id);
	for (i = 0; i < workers_num;i++){
		close(reader_fd[i]);
		close(writer_fd[i]); 

		char name[20];		
		sprintf(name,"worker%d reader",i);
		unlink(name);
		sprintf(name,"worker%d writer",i);
		unlink(name);		

		if( work_amount[i] != ""){
			free(work_amount[i]);
			work_root[i];
		}
			
	}
	free(work_amount);
	free(writer_fd);
	free(reader_fd);
	
 	
	
	return 0;
}
