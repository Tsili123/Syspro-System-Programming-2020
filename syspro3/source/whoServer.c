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
//./whoClient -q queryFile -w 11 -sp 10900 -sip linux03.di.uoa.gr
//./whoServer -q 10900 -s 9801 -w 2 -b 5


pthread_cond_t empty_cond;
pthread_cond_t full_cond;
pthread_mutex_t mutex;

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
    int threads_num;
    pthread_t *thread_ids;
    int flag;
} Arg_struct;

void create_n_threads(int threads_num,pthread_t *thread_ids,Arg_struct *args);
void *child_server(void* args);
//void child_server(int newsock);
void perror_exit(char *message);
void stop_threads(int threads_num, pthread_t *thread_ids);

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

    cb->ssocket=sock;
    struct Arg_struct *args = malloc(sizeof(Arg_struct));
    args->cb = cb;
    args->indx = 0;
    pthread_t *tid = malloc(threads_num* sizeof(pthread_t));
    args->thread_ids=tid;
    args->threads_num=threads_num;
    args->flag=1;
    for(i=0;i<threads_num;i++){	

		if(pthread_create(&tid[i], NULL, child_server,args) != 0 ){
			printf("Failed to create thread\n");
			break;
		}else{
			puts("Thread successfully creating \n");
		}
	}

   while(1){

       /* if(one == 0){
            args->instructions = malloc(threads_num*sizeof(char*));
            one=1;
        }else if(one == 1){
            args->instructions = realloc(args->instructions,(args->indx+threads_num)*sizeof(char*));
        } */

        void *newpointer;
         if(one == 0){
            args->instructions = malloc(threads_num*sizeof(char*));
            one=1;
        }else if(one == 1 && args!=NULL && (newpointer = realloc(args->instructions,(args->indx+threads_num)*sizeof(char*)) )){
            args->instructions = newpointer;
        } 
        else
        {
            break;    //handle out-of-memory
        }

       if((newsock = accept(sock,client_ptr,&clientlen)) < 0){
                break;
        } 
        
       // if(args->flag==0)
         //   break;

        push_back_circbuf(args->cb,&newsock);
        pthread_cond_signal(&empty_cond);
   }

    printf("ola ok\n");
}



void* child_server(void* args){
    int sock_fd;
    char buf[200];
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    //printf("Inside Thread Function  sock ->struct_sock = %d  sock->gFlag=> %d \n\n ",args->,sock->gFlag);
   while(1){
        struct Arg_struct *my_arg = (struct Arg_struct*) args;
        int index = (int)my_arg->indx;
    
          //  printf("wait to find a free socket_fd\n");
            //sleep(1);
            usleep(1);
            pthread_mutex_lock(&mutex);
            pop_front_circbuf(my_arg->cb,&sock_fd);
            pthread_cond_signal(&full_cond);
             pthread_mutex_unlock(&mutex);
         //   printf("socket found %d\n",sock_fd);
                 //do job

                while(read(sock_fd,buf,195)>0){
                    //printf("accepted connection %d\n",sock_fd);
                
                    printf("%s\n",buf);
                    my_arg->instructions[index] = malloc((strlen(buf)+1)*sizeof(char));
                    strcpy((char*)my_arg->instructions[index],buf);
                    (int)my_arg->indx++;
                
                    //reply
                    if(write(sock_fd,buf,195) < 0)
                        perror_exit("write");
                    close(sock_fd);//close socket 
                }
                /* //ending condition
                if(strcmp(buf,"ends")==0){
                        close(my_arg->cb->ssocket);
                        my_arg->flag=0;
                }
                     */
             //   printf("closing connenction.\n");
           
    }
    pthread_exit(0);//terminate calling thread after execution
}


void stop_threads(int threads_num, pthread_t *thread_ids){
    int i;
    for (i = 1; i < threads_num; i++) {
        if (pthread_cancel(thread_ids[i])!= 0) {//pthread_kill()
            perror("Error while killing threads");
            exit(EXIT_FAILURE);
        }
        /* if (pthread_join(thread_ids[i], NULL) != 0){
            perror("Error waiting for threads to terminate");
            exit(EXIT_FAILURE);
        } */
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&empty_cond);
    pthread_cond_destroy(&full_cond);
}

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
    pthread_mutex_lock(&mutex);
    //if full buffer then block
    if (cb->counter == cb->capacity) {
        printf("full buffer wait\n");
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
    pthread_mutex_unlock(&mutex);
}

void pop_front_circbuf(Circ_buffer*cb, void *item){
   // pthread_mutex_lock(&mutex);
    //if empty then block
    if (cb->counter == 0){
        printf("empty buffer wait\n");
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

/* //deosnt work
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
} */

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