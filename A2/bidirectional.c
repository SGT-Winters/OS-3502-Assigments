#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){

	int pipe1[2];
	int pipe2[2];
	pid_t pid;

	if(pipe(pipe1) == -1 || pipe(pipe2) == -1){
		perror("pipe");
		exit(1);
	}

	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}

	if(pid == 0){
		close(pipe1[1]);
		close(pipe2[0]);

		char msgs[200];
		int n = read(pipe1[0], msgs, sizeof(msgs)-1);

		if(n > 0){
			msgs[n] = '\0';
			printf("Child got sent: %s\n", msgs);
			fflush(stdout);

			char responses[] = "Hello from child!";
			write(pipe2[1], responses, strlen(responses));
		}

		close(pipe1[0]);
		close(pipe2[1]);
		exit(0);

	} else {

		close(pipe1[0]);

		char responses[] = "Hello from parent!";
		write(pipe1[1], responses, strlen(responses));
		close(pipe1[1]);

		char msgs[200];
		int n = read(pipe2[0], msgs, sizeof(msgs)-1);
		if(n > 0){
			msgs[n] = '\0';
			printf("Parent got sent: %s\n", msgs);
			fflush(stdout);
		}

		close(pipe2[1]);
		close(pipe2[0]);

		wait(NULL);

	}

return 0;

}//end of main
