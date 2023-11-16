#include "systemcalls.h"
#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define _XOPEN_SOURCE  //if we want WEXITSTATUS, etc

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    int uc_retval = 0;
    uc_retval = system(cmd);

    //check return value, if it's not -1 then the process is ran
    if(uc_retval == -1)
        return false;
    else
    	return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    char * remaining_command[count];
    const char * sz_slash_cmpstr = "/";
    const char * sz_dash_cmpstr = "-";
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
	//the rest of the commands
	if (i > 0)
	    remaining_command[i-1] = command[i];
    }
    
    va_end(args);
    printf("\nCommand0 is: %s\n", command[0]); 
    i = 0;
    while ((i++) < (count-1))
    {
	printf("remaining command is%s, i is %d", remaining_command[i-1], i);
	if ((strncmp(remaining_command[i-1], sz_slash_cmpstr, strlen(sz_slash_cmpstr)) != 0) && (strncmp(remaining_command[i-1], sz_dash_cmpstr, strlen(sz_dash_cmpstr)) != 0))
	{
	    //printf("\narguments does not start with either - or /.");
	    return false;
	}
    }

    // check whether the path is full path
    //if (strncmp(command[0], sz_slash_cmpstr, strlen(sz_slash_cmpstr)) != 0)
    //{
	//printf("\ncheck first character is backslash failed.");	
    //    return false;
    //}

    command[count] = NULL;
    //remaining_command[count-1] = NULL;

    //spawning child process now that we've verified command args
    int uc_status;
    pid_t child_pid;

    child_pid = fork();

    if (child_pid == -1)
    {
	//printf("\n child process not created.");
	return false;
    }
    if (child_pid == 0)
    {
	execv(command[0], command);
	
	//printf("\n should never reach here on successful run.");
	return false;
    }

    if (waitpid(child_pid, &uc_status, 0) == -1)
    {
	//printf("\nafter wait pid.");
	return false;
    }
    else if (WIFEXITED (uc_status))
    {
	//printf("\nafter wifexited with status %d.", WIFEXITED (uc_status));
	return true;
    }

    printf("\nreached end of function");
    return false;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    //char * remaining_command[count];
    //const char * sz_slash_cmpstr = "/";
    //const char * sz_dash_cmpstr = "-";

    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);

	//if (i > 0)
        //    remaining_command[i-1] = command[i];
    }

    //i = 0;
    //while ((i++) < (count-1))
    //{
    //    if ((strncmp(remaining_command[i-1], sz_slash_cmpstr, strlen(sz_slash_cmpstr)) != 0) && (strncmp(remaining_command[i-1], sz_dash_cmpstr, strlen(sz_dash_cmpstr)) != 0))
    //    {
            //return false;
    //    }
    //}

    // check whether the path is full path
    //if (strncmp(command[0], sz_slash_cmpstr, strlen(sz_slash_cmpstr)) != 0)
    //{
    //    return false;
    //}
    va_end(args);
    command[count] = NULL;
    //remaining_command[count-1] = NULL;

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    //spawning child process now that we've verified command args
    int uc_status;

    int fd = open(outputfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
    //terminate the program if file creation fails
    if (fd < 0)
    {
	perror("open");
	abort();
    }

    pid_t child_pid;

    child_pid = fork();

    if (child_pid == -1)
    {
        //printf("\n child process not created.");
	close(fd);
        return false;
    }
    if (child_pid == 0)
    {
	if (dup2(fd, 1) < 0)
	{
	    perror("duup2");
	    close(fd);
	    return false;
	}

        execv(command[0], command);

        //printf("\n should never reach here on successful run.");
	close(fd);
        return false;
    }

    if (waitpid(child_pid, &uc_status, 0) == -1)
    {
        //printf("\nafter wait pid.");
	close(fd);
        return false;
    }
    else if (WIFEXITED (uc_status))
    {
	//printf("\nwifexited finished with code %d.", uc_status);
	close(fd);
        return true;
    }

    close(fd);
    return false;
}
