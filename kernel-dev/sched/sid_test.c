#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <signal.h>

#define ITERATIONS 100

int main(int argc, char *argv[])
{
	int i = ITERATIONS;
	while(i--) {
		printf("Running process: %d\n", getpid());	
	};

	return 0;
}
