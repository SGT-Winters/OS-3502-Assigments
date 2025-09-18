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

	} else {

	}

return 0;

}//end of main
