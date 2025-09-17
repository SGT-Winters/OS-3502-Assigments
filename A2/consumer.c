#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

int main(int argc, char *argv[]){

	int max_line = -1;
	int verbose = 0;
	int opt;

	while((opt = getopt(argc, argv, "n:v")) != -1) {
		if(opt == 'n'){
			max_line = atoi(optarg);
		} else if(opt == 'v'){
			verbose = 1;
		}
	}//end of while loop

	char *line = NULL;
	size_t len = 0;
	ssize_t n;
	int line_count;
	int char_count;

	while ((n = getline(&line, &len, stdin)) != -1){
		line_count++;
		char_count += n;

		if(verbose){
			fputs(line, stdout);
		}

		if(max_line != -1 && line_count >= max_line){
			break;
		}
	}//end of while loop

	free(line);

	fprintf(stderr, "Lines: %d\n", line_count);
	fprintf(stderr, "Characters: %d\n", char_count);

return 0;
}
