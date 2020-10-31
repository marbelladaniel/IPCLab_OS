// C program to demonstrate use of fork() and pipe()
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
 
int main()
{
	// We use two pipes
	// First pipe to send input string from parent
	// Second pipe to send concatenated string from child
 
	int P1[2];  // Used to store two ends of first pipe
	int P2[2];  // Used to store two ends of second pipe
 
	char firstStr_fixed[] = "howard.edu";
	char secondStr_fixed[] = "gobison.org";
	char input_str[100];
	pid_t pid;
 
	if (pipe(P1)==-1)
	{
    	fprintf(stderr, "Pipe Failed" );
    	return 1;
	}
	if (pipe(P2)==-1)
	{
    	fprintf(stderr, "Pipe Failed" );
    	return 1;
	}
 
	printf("Enter a string to concatenate:");
	scanf("%s", input_str);
	pid = fork();
 
	if (pid < 0)
	{
    	fprintf(stderr, "fork Failed" );
    	return 1;
	}
 
	// Parent process
	else if (pid > 0)
	{
 
    	close(P1[0]);  // Close reading end of pipes
    	close(P2[1]);
 
    	// Write input and close writing end of first
    	// pipe.
    	write(P1[1], input_str, strlen(input_str)+1);
   	 
 
    	// Wait for child to send a string
    	wait(NULL);
 	 
 	 
    	char concat_str2[100];
    	read(P2[0], concat_str2, 100);
 	 
    	int k = strlen(concat_str2);
    	int i;
    	for (i=0; i<strlen(secondStr_fixed); i++)
        	concat_str2[k++] = secondStr_fixed[i];
    	concat_str2[k] = '\0';
   	 
 
    	printf("Concatenated string: %s\n", concat_str2);
    
	 
    	close(P1[1]);
    	close(P2[0]);
 	 
	}
 
	// child process
	else
	{
    	close(P1[1]);  // Close writing end of first pipes
    	close(P2[0]);
 	 
    	// Read a string using first pipe
    	char concat_str1[100];
    	read(P1[0], concat_str1, 100);
 
    	// Concatenate a fixed string with it
    	int k = strlen(concat_str1);
    	int i;
    	for (i=0; i<strlen(firstStr_fixed); i++)
        	concat_str1[k++] = firstStr_fixed[i];
 
    	concat_str1[k] = '\0';   // string ends with '\0'
 
    	printf("Concatenated string: %s\n", concat_str1);
 	 
    	printf("Enter a string to concatenate:");
    	scanf("%s", input_str);
    	write(P2[1], input_str, strlen(input_str)+1);
	 
    	close(P1[0]);
    	close(P2[1]);
 
    	exit(0);
	}
}

