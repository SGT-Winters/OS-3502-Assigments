#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>

volatile sig_atomic_t shutdown_flag = 0;
volatile sig_atomic_t stats_flag = 0;

void handle_sigint(int sig){
    shutdown_flag = 1;
}

void handle_sigusr1(int sig){
    stats_flag = 1;
}

int main(int argc, char *argv[]){

    FILE *input = stdin;
    int buffer_size = 1024;
    char *filename = NULL;
    int opt;

    while((opt = getopt(argc, argv, ":f:b:")) != -1){
        switch (opt){
            case 'f':
                filename = optarg;
                break;
            case 'b':
                buffer_size = atoi(optarg);
                if(buffer_size <= 0){
                    fprintf(stderr, "Invalid buffer size: %s\n", optarg);
                    return 1;
                }
                break;
            case '?':
                printf("ERROR\n");
                return 1;
            case ':':
                printf("Expected argument\n");
                return 1;
        }
    }

    struct sigaction sa_int;
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    struct sigaction sa_usr1;
    sa_usr1.sa_handler = handle_sigusr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    sigaction(SIGUSR1, &sa_usr1, NULL);

    if(filename){
        input = fopen(filename, "rb");
        if(!input){
            perror("fopen");
            return 1;
        }
    }

    char *buffer = malloc(buffer_size);
    if(!buffer){
        perror("malloc");
        return 1;
    }

    size_t n;
	while(!shutdown_flag){
    
        while(!shutdown_flag && (n = fread(buffer, 1, buffer_size, input)) > 0){
            fwrite(buffer, 1, n, stdout);
            fflush(stdout);

            if(stats_flag){
                fprintf(stderr, "Producer: wrote %zu bytes\n", n);
                stats_flag = 0;
            }

            usleep(500000);
        }

        if(feof(input)){
            rewind(input);
        }
    }

    free(buffer);
    if(filename) fclose(input);

    return 0;
}
