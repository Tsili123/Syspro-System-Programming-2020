#include "../include/read_args.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/defines.h"

void read_whoserver_args(int argc, char const *argv[],int *query_port,int *stat_port,int *threads_num,int *buffer_size){
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-q") == 0) {
            *query_port = atoi(argv[i + 1]);
        }else if (strcmp(argv[i], "-s") == 0) {
            *stat_port = atoi(argv[i + 1]);
        }else if (strcmp(argv[i], "-w") == 0) {
            *threads_num = atoi(argv[i + 1]);
        }else if (strcmp(argv[i], "-b") == 0) {
            *buffer_size = atoi(argv[i + 1]);
        }
    }
}

void read_whoclient_arguments(int argc, char const *argv[],char **server_ip,
                            int *server_port,char **query_file,int *num_threads){
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-q") == 0) {
            *query_file = malloc(strlen(argv[i + 1]) + 1);
            strcpy(*query_file, argv[i + 1]);
        } else if (strcmp(argv[i], "-w") == 0) {
            *num_threads = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-sp") == 0) {
            *server_port = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-sip") == 0) {
            *server_ip = malloc(strlen(argv[i + 1]) + 1);
            strcpy(*server_ip, argv[i + 1]);
        }
    }
}

void read_msg_from_socket(int sockfd, char *msg, int size) {
    // read message from socket
    if (read(sockfd, msg, size) < 0) {
        perror("Error reading from socket");
        exit(EXIT_FAILURE);
    }
    // remove '\n' if it exists inside the message
    char *pos;
    if ((pos = strchr(msg, '\n')) != NULL) {
        *pos = '\0';
    }
}