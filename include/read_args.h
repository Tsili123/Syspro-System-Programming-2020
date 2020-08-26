#ifndef READ_FUNCS_HEADER
#define READ_FUNCS_HEADER

// Read command line arguments for server program
void read_whoserver_args(int argc, char const *argv[],int *query_port,int *stat_port,int *threads_num,int *buffer_size);
// Read command line arguments for client program
void read_whoclient_arguments(int argc, char const *argv[],char **server_ip,int *server_port,char **query_file,int *num_threads);
// Read message from a socket and remove newline character if it exists
void read_msg_from_socket(int sockfd, char *msg, int size);

#endif