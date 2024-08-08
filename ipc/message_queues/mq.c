#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>


#define P2C_Q_NAME "/p2c_queue"

int child_process(mqd_t mq_p2c) {

	printf("Child process is running\n");

	ssize_t bytes_read;
	unsigned long rx_msg;

	bytes_read = mq_receive(mq_p2c, (char *)&rx_msg, sizeof(rx_msg), NULL);

	printf("Child process received %ld\n", bytes_read);
	printf("Child process received the message %lx\n", rx_msg);


	printf("Child process is exiting\n");
	return 0;
}

int parent_process(mqd_t mq_p2c) {

	printf("Parent process is running\n");
	
	unsigned long tx_msg = 0xdeaddead;
	mq_send(mq_p2c, (const char *)&tx_msg, sizeof(tx_msg), 0);

	wait(NULL);

	mq_close(mq_p2c);

	mq_unlink(P2C_Q_NAME);

	printf("Parent process is exiting\n");
	
	return 0;
}

int main() {
	printf("Starting the IPC with Message Queues\n");

	struct mq_attr attr;
	mqd_t mq_p2c;

	attr.mq_flags  = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(unsigned long);
	attr.mq_curmsgs = 0;
	
	mq_p2c = mq_open(P2C_Q_NAME, O_CREAT | O_RDWR, 0644, &attr);

	if (mq_p2c == (mqd_t) - 1)
	{
		perror("Error creating message queue\n");
		exit(1);
	}

	pid_t pid = fork();

	if (pid == 0) {
		printf("Triggering Child Process\n");
		child_process(mq_p2c);
	} else {
		printf("The Parent process continues its execution\n");
		parent_process(mq_p2c);
	}
}
