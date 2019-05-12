#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>


static char * record[10];
static int count = 0;

static void execution(const char * line)
{
	char * CMD = strdup(line);
	char *variables[10];
	int argc = 0;

	variables[argc++] = strtok(CMD, " ");
	while(variables[argc-1] != NULL){	
		variables[argc++] = strtok(NULL, " ");
	}

	argc--; 

	int back = 0;
	if(strcmp(variables[argc-1], "&") == 0){
		back = 1; 
		variables[--argc] = NULL;	
	}

	int fd[2] = {-1, -1};	

	while(argc >= 3){

		if(strcmp(variables[argc-2], ">") == 0){	

			fd[1] = open(variables[argc-1], O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP|S_IWGRP);
			if(fd[1] == -1){
				perror("open");
				free(CMD);
				return;
			}

			variables[argc-2] = NULL;
			argc -= 2;
		}else if(strcmp(variables[argc-2], "<") == 0){ 
			fd[0] = open(variables[argc-1], O_RDONLY);
			if(fd[0] == -1){
				perror("open");
				free(CMD);
				return;
			}
			variables[argc-2] = NULL;
			argc -= 2;
		}else{
			break;
		}
	}

	int status;
	pid_t pid = fork();	
	switch(pid){
		case -1:	
			perror("fork");
			break;
		case 0:	
			if(fd[0] != -1){	
				if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){
					perror("dup2");
					exit(1);
				}
			}
			if(fd[1] != -1){
				if(dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO){
					perror("dup2");
					exit(1);
				}
			}
			execvp(variables[0], variables);
			perror("execvp");
			exit(0);
		default: 
			close(fd[0]);close(fd[1]);
			if(!back)
				waitpid(pid, &status, 0);
			break;
	}
	free(CMD);
}



static void add_to_history(const char * cmd){
	if(count == (10-1)){	
		int i;
		free(record[0]);	

		for(i=1; i < count; i++)
			record[i-1] = record[i];
		     count--;
	}
	record[count++] = strdup(cmd);	
}

static void run_from_history(const char * cmd){
	int index = 0;
	if(count == 0){
		printf("No commands in history\n");
		return ;
	}

	if(cmd[1] == '!') 
		index = count-1;
	else{
		index = atoi(&cmd[1]) - 1;	
		if((index < 0) || (index > count)){ 
			return;
		}
	}
	printf("%s\n", cmd);	
	execution(record[index]);	
}

static void list_history(){
	int i;
	for(i= count-1; i >=0 ; i--){
		printf("%i %s\n", i+1, record[i]);
	}
}



static void signal_handler(const int rc){
	switch(rc){
		case SIGTERM:
		case SIGINT:
			break;
		
		case SIGCHLD:
			
			while (waitpid(-1, NULL, WNOHANG) > 0);
			break;
	}
}


int main(int argc, char *argv[]){


	struct sigaction act, act_old;
	act.sa_handler = signal_handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	if(	(sigaction(SIGINT,  &act, &act_old) == -1) ||
		(sigaction(SIGCHLD,  &act, &act_old) == -1)){ 
		perror("signal");
		return 1;
	}


	size_t line_size = 100;
	char * line = (char*) malloc(sizeof(char)*line_size);
	if(line == NULL){	
		perror("malloc");
		return 1;	
	}

	int index = 0; 
	while(1){
		if(!index)
			printf(" > ");
		if(getline(&line, &line_size, stdin) == -1){	
			if(errno == EINTR){	  
				clearerr(stdin); 
				index = 1;	 
				continue; 
			}
			perror("getline");
			break;
		}
		index = 0; 

		int line_len = strlen(line);
		if(line_len == 1){	
			continue;
		}
		line[line_len-1] = '\0';

		
		if(strcmp(line, "exit") == 0){ 
			break;
		}else if(strcmp(line, "history") == 0){ 
			list_history();
		}else if(line[0] == '!'){ 
			run_from_history(line);
		}else{ 
			add_to_history(line); 
			execution(line); 
		}
	}

	free(line);
	return 0;
}
