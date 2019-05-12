#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
using namespace std;



int main(int argc, char const *argv[])
{
	pid_t pid;

	pid =fork();

	if(pid<0)
	{
		fprintf(stderr, "ForK Failed" );
		exit(-1);
	}

	else if(pid>0) /*Child Process*/
	{ 
		cout<<"This is Parent process PID = "<<pid<<",waiting for my chid"<<"\n";
		wait(NULL);

		cout<<"This is Parent process,catch my chid , PID = "<<getpid()<<"\n";
	}

	else /*Parent process*/
	{
		cout<<"This is Chid process PID = "<<getppid();
		cout<<"My parent's PID="<<getpid()<<"\n";
	}
	return 0;
}