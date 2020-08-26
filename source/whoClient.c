#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/defines.h"
#include "../include/read_args.h"
#include "../include/parse.h"
#define perror2(s,e) fprintf(stderr,"%s : %s \n",s,strerror(e))
#define COUNT_PER_THREAD 8 /* Count increments by each thread */
//# define THRESHOLD 21 /* Count value to wake up thread */
/*int count = 0;  The counter */
//int thread_ids [4] = {0,1,2,3}; /* My thread ids */
pthread_mutex_t mutex;
pthread_cond_t cond ; /* the condition variable */

typedef struct Arg_struct {
    int index;
    struct sockaddr *server_ptr;
    char **instruction_name;
} Arg_struct;

void perror_exit(char* message );
void create_n_threads(int threads_num,pthread_t *thread_ids,char **instructions,struct sockaddr *server_ptr);
void *read_from_buffer(void *args);

void create_n_threads(int threads_num,pthread_t *thread_ids,char **instructions,struct sockaddr *server_ptr){
    // create worker threads
    int i;
    struct Arg_struct *args = malloc(sizeof(Arg_struct));

    for (i = 0; i < threads_num; i++){
        pthread_mutex_lock(&mutex);
        args->index=i;
        args->instruction_name = instructions;
        args->server_ptr = server_ptr;
        if((pthread_create(&thread_ids[i],NULL,read_from_buffer,args ) !=0 )){
            perror("Error creating threads");
            exit(EXIT_FAILURE);
        }
          //printf("%s\n",args->instruction_name[i]);
    }

    // give time for all threads to lock
        sleep(1);

        printf("Master: Now releasing the condition\n");

        pthread_cond_broadcast(&cond);

        for( i = 0; i < threads_num; ++i)
        pthread_join(*(thread_ids + i), NULL);
  
}

void *read_from_buffer(void *args){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    
    int i,index;
    struct Arg_struct *my_arg = (struct Arg_struct*) args;
    
    index = (int)my_arg->index;
    printf("Thread %d : waiting for release\n",index);

    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex); // unlocking for all other threads

    //printf("%s\n",(char*)my_arg->instruction_name[index]);
    char buf[200];
    strcpy(buf,(char*)my_arg->instruction_name[index]);

    int sock;
    // Create socket 
    if((sock = socket (AF_INET ,SOCK_STREAM,0) ) < 0)
        perror_exit("socket") ;

    // Initiate connection 
    if(connect(sock,(struct sockaddr *)my_arg->server_ptr, sizeof(struct sockaddr_in)) < 0)
        perror_exit("connect") ; 

    
    if(write(sock,buf,195)<0)
       perror_exit("write");
      // receive string
    if(read(sock,buf,195)<0)
       perror_exit("read") ;
    
    printf("Received string : %s\n",buf);

    //printf("%s\n",(char*)my_arg->instruction_name);*/
    
    pthread_exit(0);//terminate calling thread after execution
}


void perror_exit(char *message){
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]){
    //variables for parsing arguments
    int threads_num, buf_size, server_port ,sock;
    char *server_ip,*query;

    //variables for main program
    int max_elem,i;
    FILE *queryfp;
    char **instructions ;
    char * line = NULL;
    size_t len = 0;
    ssize_t reads;


    //variables for sockets
    struct sockaddr_in server, client;
    struct sockaddr *server_ptr = (struct sockaddr *)&server;
    struct sockaddr *client_ptr = (struct sockaddr *)&client;
    struct hostent *remserver;
    int port;
    char buf[256];

    read_whoclient_arguments(argc, argv,&server_ip,
                           &server_port,&query,&threads_num);
    
    printf("%s %d %s %d \n",query,threads_num,server_ip,server_port);
    //printf("%d\n",server_port);

    //get server address
    if ((remserver = gethostbyname(server_ip)) == NULL) {
        herror("Error in gethostbyname");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;  // internet domain
    memcpy(&server.sin_addr, remserver->h_addr, remserver->h_length);
    server.sin_port = htons(server_port);

    /*// get clients ip address
    struct in_addr ipclient = get_client_info();
    printf("This client's port: %d, address: %s\n", ntohs(client.sin_port),inet_ntoa(ipclient)); 

    client.sin_family = AF_INET;  // internet domain
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_port = htons(port); */

    queryfp = fopen(query,"rw");
    max_elem=countlines(queryfp);
    max_elem++;
    printf("Count of query's lines is %d\n",max_elem);
    rewind(queryfp);
    
    //parse_file(queryfp,max_elem,&instructions);
    instructions = malloc(max_elem*sizeof(char*));
    
    i=0;
    char *s;
	//get one line
	while ((reads = getline(&line, &len, queryfp)) != -1) {
        printf("Retrieved line of length %zu:\n", reads);
        printf("%s", line);
        instructions[i] = malloc(strlen(line));
        strcpy(instructions[i],line);
        if ((s = strchr(instructions[i], '\n')) != NULL) {
                *s = '\0';
        }
        i++;
    }

    fclose(queryfp);
    printf("\n\n\n\n\n");

    /* for(i=0;i< max_elem;i++){ 
        printf("%s\n",instructions[i]);
    } */


    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }

    pthread_t *t_ids = malloc(threads_num * sizeof(pthread_t));
    // create worker threads
    create_n_threads(threads_num, t_ids, instructions,server_ptr);

    
    char bufs[200];
    strcpy(bufs,"ends");

    //END CONNECTION WITH SERVER

    // Create socket 
    if((sock = socket (AF_INET ,SOCK_STREAM,0) ) < 0)
        perror_exit("socket") ;

    // Initiate connection 
    if(connect(sock,server_ptr, sizeof(struct sockaddr_in)) < 0)
        perror_exit("connect") ; 

    
    if(write(sock,bufs,195)<0)
       perror_exit("write");
    
    /* if(read(sock,bufs,195)<0)
       perror_exit("read") ;
    
    printf("Received string : %s\n",bufs); */

    close(sock);
/*
    // Create socket 
    if((sock = socket (AF_INET ,SOCK_STREAM,0) ) < 0)
        perror_exit("socket") ;

    // Initiate connection 
    if(connect(sock,server_ptr, sizeof(server)) < 0)
        perror_exit("connect") ;

    printf ("Connecting to %s port % d \n", server_ip , server_port ) ;

     do{
        printf("Give input string : ");
        fgets(buf,sizeof(buf),stdin); // Read from stdin 
        for(i=0;buf[i]!='\0';i++){ // For every char 
        // Send i - th character 
        if(write(sock,buf+i,1)<0)
            perror_exit("write");
            // receive i - th character transformed 
            if(read(sock,buf+i,1)<0)
            perror_exit("read") ;
        }
        printf("Received string : %s",buf);
    }while(strcmp(buf,"END\n") != 0) ; // Finish on " end " 
    close(sock); // Close socket and exit   */
}
// ./whoClient –q queryFile -w 3 –sp 0 –sip 0 // parse_file(queryfp,max_elem,&instructions);

// ./whoClient -q queryFile -w 5 -sip linux02.di.uoa.gr -sp 1512
//./master -w 3 -b 10000 -s linux02.di.uoa.gr -p 1800 -i input_dir
//./whoServer -q 1512 -s 1111 -w 3 -b 10


