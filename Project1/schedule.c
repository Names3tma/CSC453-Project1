#include "schedule.h"

void alarm_handler(int sig) {(void)sig;}

int main(int argc, char* argv[])
{
	if (argc < 3) {exit(1);}

	char *endptr;
	long quantum = strtol(argv[1], &endptr, 10); // Base 10 (decimal) conversion

	int num_proc = 1;
	for (int i = 2; i < argc; i++)
	{
		if (strcmp(argv[i], ":") == 0) {num_proc++;}
	}

	if (num_proc > MAX_PROCESSES) {exit(1);}

	char*** arg_list = (char***)malloc((num_proc + 1) * sizeof(char**)); //2d string array
	if (arg_list == NULL) {exit(1);}

	int start = 2;
	int index = 0; // number of processes

	for (int i = 2; i <= argc; i++)
	{
		if (i == argc || strcmp(argv[i], ":") == 0) 			
		{
			int n = i - start;

			if (n == 0 || (n-1) > MAX_ARGUEMENTS) {exit(1);} // correct num of args
		
			char** list = (char**)malloc((n+1)*sizeof(char*));
			if (list == NULL) {exit(1);}

			for(int j = start; j < i; j++) {list[j-start] = argv[j];} // adding
			list[n] = NULL; // similar to \0 but for list, 

			arg_list[index] = list;
			index++;

			start = i+1; // skip ':'						
		}
	}
	arg_list[index] = NULL;
	
	pid_t pids[MAX_PROCESSES];
	int alive[MAX_PROCESSES];

	for(int p = 0; p < num_proc; p++) // creates children then pauses
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			raise(SIGSTOP); // pause
			execvp(arg_list[p][0], arg_list[p]);
			exit(1);
		}
		pids[p] = pid;
		alive[p] = 1;

		int status;
		waitpid(pid, &status, WUNTRACED); // double checks if child are properly paused
	}


	struct sigaction sa = {0};
	sa.sa_handler = alarm_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);

	int alive_count = num_proc;
	int curr = 0;

	while(alive_count > 0)
	{
		while(!alive[curr]) {curr = (curr + 1) % num_proc;} // circular

		pid_t pid = pids[curr];

		kill(pid, SIGCONT); // continues the child

		// timer
		struct itimerval timer = {0};
		timer.it_value.tv_sec = quantum / 1000;
		timer.it_value.tv_usec = (quantum % 1000) * 1000;
		setitimer(ITIMER_REAL, &timer, NULL); // timer starts here

		int status;
		pid_t result = waitpid(pid, &status, WUNTRACED);

		struct itimerval cancel = {0};
		setitimer(ITIMER_REAL, &cancel, NULL);

		if (result == -1) // if timer ran out
		{
			kill(pid, SIGSTOP);
			waitpid(pid, &status, WUNTRACED);
		}
		else if (WIFEXITED(status) || WIFSIGNALED(status)) // if the process terminated
		{
			alive[curr] = 0;
			alive_count--;
		}
		curr = (curr + 1) % num_proc;
	}

	for (int k = 0; k < num_proc; k++) 
	{
		free(arg_list[k]);
	}
	free(arg_list);
	
	return 0;
			
}

