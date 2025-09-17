#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char *argv[]){

	FILE *input =stdin;
	int buffer_size =4096;
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
				break;
			case ':':
				printf("expected argument\n");
				break;
		}//end of switch statement

	}//end of while loop

	if(filename){
		input = fopen(filename, "rb");
		if(!input){
			perror("fopen");
			return 1;
		}//end of if(!input)
	}//end of if(filename)

	int *buffer = malloc(buffer_size);
	if(!buffer){
		perror("malloc");
		return 1;
	}//end of if (!buffer)

	size_t n;
	while((n = fread(buffer, 1, buffer_size, input)) > 0){
		fwrite(buffer, 1, n, stdout);
	}// end of while look

	free(buffer);
	if(filename) fclose(input);

return 0;
}
