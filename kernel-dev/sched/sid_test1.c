#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <signal.h>

#define ITERATIONS 10000

int main(int argc, char *argv[])
{
	int target_cpu = 0;
//	cpu_set_t set;
//	CPU_ZERO(&set);
//	CPU_SET(target_cpu, &set);
//
//	if (sched_setaffinity(0, sizeof(set), &set) == -1)
//		perror("sched_setaffinity of parent failed");

	pid_t pid = fork();
	if (pid < 0) {
		perror("Failed to fork");
		return 1;
	}

	if (pid == 0) {
		if (prctl(PR_SET_NAME, "sid_test2", 0, 0, 0) == -1)
			perror("prctl failed");

//		if (sched_setaffinity(0, sizeof(set), &set) == -1)
//			perror("sched_setaffinity of child failed");
		int i = ITERATIONS;
		while (i--) {
			printf("Running child process: %d\n", getpid());
		};
	} else {
		int i = ITERATIONS;
		while(i--) {
			printf("Running parent process: %d\n", getpid());	
		};
		int st;
		waitpid(pid, &st, 0);
	}

	return 0;
}
