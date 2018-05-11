#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>

pid_t pid_curr = -1; //???

ssize_t s_getline(char **buf)
{
	size_t temp = 0;
	ssize_t len = getline(buf, &temp, stdin);

	assert(len != -1);
	if (buf[0][len - 1] == '\n')
		buf[0][len - 1] = '\0';
	return (len - 1);
}

pid_t s_fork(void)
{
	pid_t pid = fork();

	assert(pid != -1);
	return pid;
}

void s_wait(void)
{
	int *status;
	//pid_t pid_ch = wait(status);
	wait(status);

	//assert(pid_ch != -1);
}

void s_free(void *buf)
{
	assert(buf != NULL);
	free(buf);
}

void s_kill(pid_t pid_temp, int val)
{
	assert(kill(pid_temp, val) != -1);
}

char **split(char *buf)
{
	char **splitted = NULL;
	size_t i = 0;
	char *ptr = strtok(buf, " "); //???

	while (ptr != NULL) {
		splitted = realloc(splitted, (i + 2) * sizeof(char *)); //!!!
		assert(splitted != NULL);
		splitted[i++] = ptr;
		ptr = strtok(NULL, " ");
	}
	splitted[i] = NULL;
	return splitted;
}

void handler(int num_s)
{
	if (pid_curr != -1) {
		s_kill(pid_curr, SIGINT);
		pid_curr = -1;
	}
}

int main(int argc, char *argv[])
{
	signal(SIGINT, &handler); //!!!
	while (1) {
		printf("mine >> ");
		char *str = NULL;
		ssize_t len = s_getline(&str); //think later

		if (strcmp(str, "exit") == 0)
			_exit(EXIT_SUCCESS);  //???
		if (len < 1) {                //mb?
			s_free(str);
			continue;
		}
		int flag_w = 1;

		if (str[len - 1] == '&') {
			str[len - 1] = '\0';
			flag_w = 0;
		}
		char **stv = split(str);

		pid_curr = s_fork();
		if (pid_curr == 0) {
			if (execvp(stv[0], stv) == -1)
				puts("execution error");
			_exit(EXIT_SUCCESS);
		} else {
			if (flag_w) {
				s_wait();
				pid_curr = -1;
			}
		}
		s_free(stv);
		s_free(str);
	}
	return EXIT_SUCCESS;
}
