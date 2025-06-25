#include "systemcalls.h"
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

bool do_system(const char *cmd)
{
	int result = system(cmd);
	// system() returns -1 if it fails to invoke the command
	if (result == -1) 
	{
		return false; // Error invoking system()
	}
	// Check if the command's exit status is 0 (success)
	return (result == 0);
}

bool do_exec(int count, ...)
{
	va_list args;
	va_start(args, count);
	char * command[count + 1];
	for (int i = 0; i < count; i++)
	{
		command[i] = va_arg(args, char *);
	}
	command[count] = NULL; // execv requires a NULL-terminated array of arguments
	// command[count] = command[count];
	va_end(args);

	// create a child process
	printf("Attempting to run :%s\n", command[0]);
	fflush(stdout); // flush stdout
	pid_t pid = fork();
	if (pid == -1) 
	{
		perror("fork failed");
		return false;
	}
	else if (pid == 0) 
	{
		// child process
		execv(command[0], command); 
		perror("execv failed with return value -1");
		_exit(1); // exit child process with failure
	}
	else
	{
		// parent process: wait for the child to finish
		// waitpid will block until the child process terminates
		// and will return the child's PID.
		int status;
		waitpid(pid, &status, 0);
		return WIFEXITED(status) && WEXITSTATUS(status) == 0;
	}
}

bool do_exec_redirect(const char *outputfile, int count, ...)
{
	int i;
	va_list args;
	va_start(args, count);
	char * command[count+1];
	for(i=0; i<count; i++)
	{
		command[i] = va_arg(args, char *);
	}
	command[count] = NULL;
	command[count] = command[count];
	va_end(args);

	// Create a child process
	pid_t pid = fork();
	if (pid == -1) 
		return false;
	else if (pid == 0) 
	{
		int fd = open (outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open");
			exit(-1); // Exit child process with failure if open fails
		}
		// Redirect stdout (file descriptor 1) to the file
		if (dup2(fd, 1) == -1) 
		{
			close(fd);
			exit(-1); // Failed to redirect
		}
		close(fd); // No longer need the file descriptor
		
		// Run the command with execv
		execv(command[0], command);
		// If execv returns, it failed
		exit(-1);
	} 
	else 
	{
		// parent process: wait for the child to finish
		// waitpid will block until the child process terminates
		// and will return the child's PID.
		int status;
		waitpid(pid, &status, 0);
		return WIFEXITED(status) && WEXITSTATUS(status) == 0;
	}
}

