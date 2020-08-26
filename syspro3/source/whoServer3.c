#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h> /* toupper */
#include <pthread.h>
 #include <fcntl.h>
#include "../include/defines.h"
#include "../include/read_args.h"
#include "../include/parse.h"
#define less 1
#define fl 0

pthread_cond_t empty_cond;
pthread_cond_t full_cond;
pthread_mutex_t mutex;
pthread_cond_t cond ; /* the condition variable */


//data of node
typedef struct cb_data {
    int socket_fd;
} cb_data;

//circular buffer created with list like structure
typedef struct Circ_buffer {
    void *buffer;      // data buffer
    void *bufend;  // end of data buffer
    int counter;         // number of items in the buffer
    int capacity;      // maximum number of items in the buffer
    void *headbuf;        // pointer head
    void *tailbuf;        // pointer tail
    size_t item_size;  // size of each item in the buffer
    int ssocket;
} Circ_buffer;

typedef struct Arg_struct {
    char **instructions;
    Circ_buffer *cb;
    int indx;
} Arg_struct;

void create_n_threads(int threads_num,pthread_t *thread_ids,Arg_struct *args);
void *child_server(void* args);
//void child_server(int newsock);
void perror_exit(char *message);

// Allocate memory for a new circular buffer return it
Circ_buffer *initialize_circbuf(int capacity, int item_size);
// Release memory of buffer
void delete_circbuf(Circ_buffer *cb);
// Add an item to the back 
void push_back_circbuf(Circ_buffer *cb, const void *item);
// Remove an item from the front 
void pop_front_circbuf(Circ_buffer *cb, void *item);
// Print all items in circular buffer
void print_circbuf(Circ_buffer *cb);

int one=0;

int main(int argc, char const *argv[]){

    //variables for parsing arguments
    int threads_num, buf_size, server_port ,sock,newsock,port;
    char *server_ip,*query;

    //variables for main program
    int max_elem,i;
    FILE *queryfp;
    char **instructions ;
    char * line = NULL;
    size_t len = 0;
    //ssize_t read;


    //variables for sockets
    struct sockaddr_in server, client;
    struct sockaddr *server_ptr = (struct sockaddr *)&server;
    struct sockaddr *client_ptr = (struct sockaddr *)&client;
    struct hostent *remserver;
    int query_port,statistics_port;
    socklen_t clientlen;
    Circ_buffer *cb;

    if(argc < 2){
        printf("PLease give query port number\n");
        exit(1);
    }

    read_whoserver_args(argc,argv,&query_port,&statistics_port,&threads_num,&buf_size);

    port = query_port ; // port -------

    //Reap dead children asychronously 
        //signal(SIGCHD,sigchld_handler);
        
    //create socket
    if((sock = socket(AF_INET,SOCK_STREAM,0))<0)
        perror_exit("socket");

    //reuse option allows multiple sockets to be open on the same port.
    u_int yes = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    server.sin_family = AF_INET;//internet domain
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port); //the given port
    
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }

    cb=initialize_circbuf(buf_size,sizeof(int));

    //Bind socket to address
    if(bind(sock,server_ptr,sizeof(server))<0)
        perror_exit("bind");

    //Listen for connections
    if(listen(sock,5) < 0)
        perror_exit("listen");
    printf("listening for connections to port %d\n",port); 

    int flag=0,j,ex=0;
    pid_t child_pid, wpid;
    int status = 0,end=0;
    int count=buf_size;
    #ifdef fl
        flag=1;
    #endif
    cb->ssocket=sock;
    struct Arg_struct *args = malloc(sizeof(Arg_struct));
    args->cb = cb;
    args->indx = 0;

    //get instructions from who client
    while(1){
        // we use curcular buffer instead of a very big buffer(space efficient)
        if(flag==1){
            printf("----------------\n");
            if((newsock = accept(sock,client_ptr,&clientlen)) < 0){
                break;
                perror_exit("accept");
            } 
            //enter critical section
            pthread_mutex_lock(&mutex);
        
            //execute push function
            push_back_circbuf(cb,&newsock);

             //we have to unblock empty cond -> send singal
            pthread_cond_signal(&empty_cond); 

            //exit critical section
            pthread_mutex_unlock(&mutex);
            ex=1;
        }

        #ifdef less
            while(flag==0){
                // end++;
                // printf("----------------%d\n",end);

                //accept connections , fill the buffer
                if((newsock = accept(sock,client_ptr,&clientlen)) < 0){
                    perror_exit("accept");
                } 
                
                //enter critical section
                pthread_mutex_lock(&mutex);
                push_back_circbuf(cb,&newsock);

                //we have to unblock empty cond -> send singal
                pthread_cond_signal(&empty_cond); 

                //exit critical section
                pthread_mutex_unlock(&mutex);
                //count--;
                ex++;
                if(ex==count){
                    flag=1;
                    break;
                }
            }
        #endif
        
        //printf("ex %d\n",ex);
        pthread_t *t_ids = malloc(ex* sizeof(pthread_t));
        // create threads
        create_n_threads(ex, t_ids, args);

        //find client address
        /////

        //printf("kala\n");
        free(t_ids);
        
        #ifdef less
        count=ex;
        ex=0; 
        #endif
    }
    
    for(i=0;i<args->indx;i++){
        printf("%s\n",args->instructions[i]);
    }

    //receive statistics
    int sock2,port2=statistics_port,buffer_size=10000;
    char buft[buffer_size];

    
    //variables for sockets
    struct sockaddr_in server2, client2;
    struct sockaddr *server_ptr2 = (struct sockaddr *)&server2;
    struct sockaddr *client_ptr2 = (struct sockaddr *)&client2;
    socklen_t clientlen2;
    int countw=0;

    //Reap dead children asychronously 
        //signal(SIGCHD,sigchld_handler);


    //HANDLE STATISTICS PORT NUM WITH NEW SOCKET///////////////////////////////

    //create socket
    if((sock2 = socket(AF_INET,SOCK_STREAM,0))<0)
        perror_exit("socket");

    //reuse option allows multiple sockets to be open on the same port.
    u_int yes2 = 1;
    setsockopt(sock2, SOL_SOCKET, SO_REUSEADDR, &yes2, sizeof(yes2));

    server2.sin_family = AF_INET;//internet domain
    server2.sin_addr.s_addr = htonl(INADDR_ANY);
    server2.sin_port = htons(port2); //the given port

    //Bind socket to address
    if(bind(sock2,server_ptr2,sizeof(server2))<0)
        perror_exit("bind");

    //Listen for connections
    if(listen(sock2,5) < 0)
        perror_exit("listen");
    printf("listening for connections to port %d\n",port2);  

    while(1){
            printf("----------------\n");
            if((newsock = accept(sock2,client_ptr2,&clientlen2)) < 0){
                break;
            } 
            printf("accepted connection %d\n",sock2);


            while(read(newsock,buft,buffer_size-10)>0){
    
                printf("%s\n",buft);
            }
            
            countw++;
            if(countw==3) ////IMPORTANT TO READ STATISTICS,if we reached statistics from every worker
            {
                //sleep(3);
                break;
            }
    }

    printf("statistics received.\n");

    fflush(stdin);
    
     //now the server becomes client
    //we have to send every instruction to every worker
    //and then receive the results
    
    //get parameter to send instruction to every worker
    //this will be done by filling an array of ints 
    //we get the right param from worker process

    //we take into consideration the fact that 
    //we use for every worker sequentially the 
    //port 1,2,3,..,n.That is represented by i.

    

    /* //for(i=1;i<countw+1;i++){    
        for(j=0;j<args->indx;j++){
            strcpy(buft,args->instructions[j]);
            printf("%s\n",args->instructions[j]);
             //write instructions to every worker
            while(write(newsock,buft,buffer_size-10)>0){
    
                printf("%s\n",buft);
            }
            //receive results from them 
            while(read(newsock,buft,buffer_size-10)>0){
    
                printf("%s\n",buft);
            }  

    } 
    //}

    printf("closing connenction.\n"); */
 
    //communicate with workers 
    close(sock2);
 


    /* server.sin_family = AF_INET;  // internet domain
    memcpy(&server.sin_addr, remserver->h_addr, remserver->h_length);
    server.sin_port = htons(server_port); */

   /*  int sock3;

    //variables for sockets
    struct sockaddr_in server3
    struct sockaddr *server_ptr3 = (struct sockaddr *)&server3;
    int countw=0;

    //create socket
    if((sock3 = socket(AF_INET,SOCK_STREAM,0))<0)
        perror_exit("socket");

    //reuse option allows multiple sockets to be open on the same port.
    u_int yes2 = 1;
    setsockopt(sock3, SOL_SOCKET, SO_REUSEADDR, &yes2, sizeof(yes2));

    //get server address
    if ((remserver = gethostbyname("linux04.di.uoa.gr")) == NULL) {
        herror("Error in gethostbyname");
        exit(EXIT_FAILURE);
    }

    server3.sin_family = AF_INET;//internet domain
    server3.sin_addr.s_addr = htonl(INADDR_ANY);
    server3.sin_port = htons(0); //the given port

   // Create socket 
    if((sock3 = socket (AF_INET ,SOCK_STREAM,0) ) < 0)
        perror_exit("socket");

    // Initiate connection 
    if(connect(sock3,server_ptr2, sizeof(struct sockaddr_in)) < 0)
        perror_exit("connect"); 

    

    for(i=1;i<countw+1;i++){    
        for(j=0;j<args->indx;j++){
            strcpy(buft,args->instructions[j]);
            
            while(write(i,buft,buffer_size-10)>0){
    
                printf("%s\n",buft);
            }

            while(read(i,buft,buffer_size-10)>0){
    
                printf("%s\n",buft);
            }

        } 
    }

    printf("closing connenction.\n");
 
    //communicate with workers 
    close(sock2); */

    //send instructions / receive 
    //forward them to workers

}

void create_n_threads(int threads_num,pthread_t *thread_ids,Arg_struct *args){//Circ_buffer *cb
    // create worker threads
    int i,newsock;
    if(one == 0){
        args->instructions = malloc(threads_num*sizeof(char*));
        one=1;
    }else if(one == 1){
        args->instructions = realloc(args->instructions,(args->indx+threads_num)*sizeof(char*));
    } 
    
    printf("mesa\n");
    for (i = 0; i < threads_num; i++){
        if((pthread_create(&thread_ids[i],NULL,child_server,args) !=0 )){
            perror("Error creating threads");
            exit(EXIT_FAILURE);
        }
          //printf("%s\n",args->instruction_name[i]);
          printf("kala\n");
    }
    //printf("kala\n");
     for(i = 0; i < threads_num;i++){
        pthread_join(*(thread_ids + i), NULL);
    }  
}

void* child_server(void* args){
    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
   // pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    
    //Circ_buffer *cbt = (Circ_buffer *)cb;
    struct Arg_struct *my_arg = (struct Arg_struct*) args;
    
    int index = (int)my_arg->indx;
    printf("Thread %d : waiting for release\n",index);

    char buf[200];
    int i,sock_fd;
    //enter critical section
    pthread_mutex_lock(&mutex);
    pop_front_circbuf(my_arg->cb,&sock_fd);
    printf("sock fd %d\n",sock_fd);
    //we have to unblock empty cond -> send singal
    pthread_cond_signal(&full_cond); 
    //exit critical section
    pthread_mutex_unlock(&mutex);

    while(read(sock_fd,buf,195)>0){
        printf("accepted connection %d\n",sock_fd);
    
        printf("%s\n",buf);
        my_arg->instructions[index] = malloc((strlen(buf)+1)*sizeof(char));
        strcpy((char*)my_arg->instructions[index],buf);
        (int)my_arg->indx++;
        
        //reply
        if(write(sock_fd,buf,195) < 0)
            perror_exit("write");
        close(sock_fd);//close socket 
    }
    printf("closing connenction.\n");
    if(strcmp(buf,"ends")==0)
            close(my_arg->cb->ssocket);
    pthread_exit(0);//terminate calling thread after execution
}

/*
void child_server(int newsock){
    char buf[200];
    int i;
    while(read(newsock,buf,195)>0){
        printf("mesa\n");
        //capitalize char
        printf("%s\n",buf);
        //for(i=0;buf[i]!='\0';i++)
         //   buf[i] = toupper(buf[i]);
        //reply
        if(write(newsock,buf,195) < 0)
            perror_exit("write");
    }
    printf("closing connenction.\n");
    close(newsock);//close socket
}*/

/*
        //for(j=0;j<ex;j++){

            //pop_front_circbuf(cb,&newsock);
            //printf("accepted coonection %d\n",newsock);

            /* switch(fork()){
            case -1:
                perror("fork");break;
            case 0: //child process
                close(sock);
                child_server(newsock);
                exit(0);
            default:
                //wait(NULL);
                close(newsock);//parent process closes socket to client
            // must be closed before it gets re - assigned 
            }
            //while ((wpid = wait(&status)) > 0); // this way, the father waits for all the child processes  

        //}
*/
void perror_exit(char *message){
    perror(message);
    exit(EXIT_FAILURE);
}

//generic circular buffer
Circ_buffer *initialize_circbuf(int capacity, int item_size){

    // initialize conditional variable
    pthread_cond_init(&empty_cond, NULL);
    Circ_buffer *cb = malloc(sizeof(Circ_buffer));
    cb->buffer = malloc(capacity * item_size);
    cb->bufend = (char *)cb->buffer + capacity * item_size;
    cb->counter = 0;
    cb->capacity = capacity;
    cb->item_size = item_size;
    cb->headbuf = cb->buffer;
    cb->tailbuf = cb->buffer;

    return cb;
}

void delete_circbuf(Circ_buffer *cb) {
    free(cb->buffer);
    free(cb);
}

void push_back_circbuf(Circ_buffer *cb, const void *item){


    //if full buffer then block
    if (cb->counter == cb->capacity) {
        pthread_cond_wait(&full_cond, &mutex);
    }

    //copy item to head place
    memcpy(cb->headbuf, item, cb->item_size);

    //move pointer head to next available place
    cb->headbuf = (char *)cb->headbuf + cb->item_size;

    //if we reached end then move circular
    if (cb->headbuf == cb->bufend){
        cb->headbuf = cb->buffer;
    }
    cb->counter++;
    
}

void pop_front_circbuf(Circ_buffer*cb, void *item){
    
    //if empty then block
    if (cb->counter == 0){
        pthread_cond_wait(&empty_cond, &mutex);
    }

    //pop item
    memcpy(item, cb->tailbuf, cb->item_size);

    // fix tail pointer
    cb->tailbuf = (char *)cb->tailbuf + cb->item_size;

    //if we reached end then move circular
    if (cb->tailbuf == cb->bufend) {
        cb->tailbuf = cb->buffer;
    }
    cb->counter--;
    
}

//deosnt work
void print_circbuf(Circ_buffer *cb) {
    void *cur = cb->tailbuf;
    //traverse buffer of fixed size(list structure)
    printf("buffer contents:\n");
    while (cur != cb->headbuf){
        printf("ok\n");
        cb_data *item = malloc(sizeof(cb_data));
        memcpy(item, cur, cb->item_size);
        printf("%d \n", item->socket_fd);
        free(item);

        // get next item
        cur = (char *)cur + cb->item_size;
    }
}

/*void stop_threads(int threads_num, pthread_t *thread_ids){
    int i;
    for (i = 0; i < threads_num; i++) {
        if (pthread_cancel(thread_ids[i]) != 0) {
            perror("Error while canceling threads");
            exit(EXIT_FAILURE);
        }
        pthread_mutex_unlock(&mutex);
        if (pthread_join(thread_ids[i], NULL) != 0){
            perror("Error waiting for threads to terminate");
            exit(EXIT_FAILURE);
        }
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&empty_cond);
    pthread_cond_destroy(&full_cond);
}
*///we have to unblock empty cond -> send singal
             //   pthread_cond_signal(&empty_cond); 