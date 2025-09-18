#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <time.h>

volatile sig_atomic_t shutdown_flag = 0;
volatile sig_atomic_t stats_flag = 0;

void handle_sigint(int sig){
    shutdown_flag = 1;
}

void handle_sigusr1(int sig){
    stats_flag = 1;
}

int main(int argc, char *argv[]){

    int max_line = -1;
    int verbose = 0;
    int opt;

    clock_t start = clock();

    while((opt = getopt(argc, argv, "n:v")) != -1) {
        if(opt == 'n'){
            max_line = atoi(optarg);
        } else if(opt == 'v'){
            verbose = 1;
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

    char *line = NULL;
    size_t len = 0;
    ssize_t n;
    int line_count = 0;
    int char_count = 0;

    while(!shutdown_flag && (n = getline(&line, &len, stdin)) != -1){
        line_count++;
        char_count += n;

        if(verbose){
            fputs(line, stdout);
	    fflush(stdout);
        }

        if(stats_flag){
            fprintf(stderr, "Consumer stats: lines=%d chars=%d\n", line_count, char_count);
            stats_flag = 0;
        }

        if(max_line != -1 && line_count >= max_line){
            break;
        }
	usleep(10000);
    }

    free(line);

    fprintf(stderr, "Lines: %d\n", line_count);
    fprintf(stderr, "Characters: %d\n", char_count);

    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    fprintf(stderr, "Time: %1f\n", cpu_time);

    return 0;
}
