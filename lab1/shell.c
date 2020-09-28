//+
// File:	shell.c
//
// Purpose:	This program implements a simple shell program. It does not start
//		processes at this point in time. However, it will change directory
//		and list the contents of the current directory.
//
//		The commands are:
//		   cd name -> change to directory name, print an error if the directory doesn't exist.
//		              If there is no parameter, then change to the home directory.
//		   ls -> list the entries in the current directory.
//			      If no arguments, then ignores entries starting with .
//			      If -a then all entries
//		   pwd -> print the current directory.
//		   exit -> exit the shell (default exit value 0)
//
//		if the command is not recognized an error is printed.
//-

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctype.h>

#define CMD_BUFFSIZE 1024
#define MAXARGS 10

int splitCommandLine(char * commandBuffer, char* args[], int maxargs);
void doCommand(char * args[], int nargs);

int main() {
    char commandBuffer[CMD_BUFFSIZE];
    char *args[MAXARGS];

    // print prompt.. fflush is needed because
    // stdout is line buffered, and won't
    // write to terminal until newline
    printf("%%> ");
    fflush(stdout);

    while(fgets(commandBuffer,CMD_BUFFSIZE,stdin) != NULL){
	// Remove newline at end of buffer
	// TODO Step 2: remove newline from end of buffer

	commandBuffer[strlen(commandBuffer) - 1] = '\0';
	// Split command line into words.
	// TODO Step 2: call splitCommandLine with the right parameters
	
	// Debugging for step 2
	int nargs = splitCommandLine(commandBuffer, args, MAXARGS);
	printf("%d\n", nargs);
	int i;
	for (i = 0; i < nargs; i++){
	 printf("%d: %s\n",i,args[i]);
	}

	// Execute the command
	// Remember to check if there is a command (i.e. value of nargs)
	// TODO: Step 3 call doCommand with the right arguments
	doCommand(args, nargs);
	// print prompt
	printf("%%> ");
	fflush(stdout);
    }
}

////////////////////////////// String Handling ///////////////////////////////////

//+
// Function:	skipChar
//
// Purpose:	This function skips over a given char in a string
//		For security, the function will not skip null chars.
//
// Parameters:
//    charPtr	Pointer to string
//    skip	character to skip
//
// Returns:	Pointer to first character after skipped chars.
//		Identity function if the string doesn't start with skip,
//		or skip is the null character
//-

char * skipChar(char * charPtr, char skip){
 // TODO Step 2: skip over instances of the char skip
 //    return input value of charPtr if *char is null char
    int i;
	//if null character is passed, return original string    
	if (skip== '\0' || charPtr[0]!= skip)
		return (charPtr);
	else{
	    
	    //loop through string until skip character is found
		for (i =0; charPtr[i] != '\0'; i++) {
	    	if(charPtr[i] != skip)
	       		break;  
		}
    }

    
	return &charPtr[i];

}

//+
// Function:	splitCommandLine
//
// Purpose:	This splits a string into an array of strings.
//		The array is passed as an argument. The string
//		is modified by replacing some of the space characters
//		with null characters to terminate each of the strings.
//
// Parameters:
//	commandBuffer	The string to split
//	args		An array of char pointers
//	maxargs		Size of array args (max number of arguments)
//
// Returns:	Number of arguments (< maxargs).
//
//-

int splitCommandLine(char * commandBuffer, char* args[], int maxargs){
   // TODO Step 2 split the command into words using only
   // the functions skipChar and strchr. You may use fprintf to report
   // errors that are encountered to stderr.
   int i ;

   char* newBuffer = commandBuffer;
   

   for(i = 0; i<maxargs;i++){
        
        //Jump to beginning of the next word 
        newBuffer = skipChar(newBuffer, ' ');

        // if end of string is reached, break loop
        if (newBuffer[0] == '\0'){
        	i--;
        	break;
        }

        //assign pointer to beginning of word into array

        //assign pointer to beginning of word into array.

        args[i] = newBuffer;
        
        //look for space wich signifies end of word
        newBuffer = strchr(newBuffer, ' ');

        //if NULL is returned, it means we are at the end of the string
        if(newBuffer == NULL){
        	break;
        }

        //place null character to seperate words
        newBuffer[0] = '\0';    
        newBuffer++;

   } 

   return i+1;

}


////////////////////////////// Command Handling ///////////////////////////////////

// typedef for pointer to command handling functions

// TODO STEP 3a write the typedef for the function handler pointer to
// use in the structure immediately below.
// See the description of the function prototypes at the bottom of
// the file in the comments.

typedef void (*command)(char* args[], int validArgs);


// cmdStruct type:
// Associates a command name with a command handling function

// TODO STEP 3b use the typedef above (Step 3a) to make a two element
// struct. The first is a char * for the name of the command
// the other is a function pointer for the command handling function

typedef struct {
	char *cmdName;
	command cmd;
} commandStruct;



// prototypes for command handling functions
// TODO STEP 4b,6: add a function prototype
// for each command handling function


void exitFunc(char *args[], int nargs);
void cdFunc(char *args[], int nargs);
void lsFunc(char *args[], int nargs);
void pwdFunc(char *args[], int nargs);
int dotCheck(const struct dirent *d);

// Array that provides the list commands and functions
// must be terminated by {NULL, NULL} 
// in a real shell, this would be a hashtable.

// TODO Step 4a: add a global array of 
// the type above that contains initializers
// of strings and command handling funciton names

commandStruct commandArray[] = {
	{"exit", exitFunc},
	{"cd", cdFunc},
	{"ls", lsFunc},
	{"pwd", pwdFunc},
	{NULL, NULL}
};


//+
// Function:	doCommand
//
// Purpose:	This command calls a command handling funciton from
//		the commands array based on the first argument passed
//		in the args array.
//
// Parameters:
//	args	command and parameters, an array of pointers to strings
//	nargs	number of entries in the args array
//
// Returns	nothing (void)
//-

void doCommand(char * args[], int nargs){
   // TODO Step 5 this function is small
   //  this is the command search loop
	int i = 0;
	for (i = 0; i < nargs; i++)
	{
		// if (strcmp(args[i],args[0]))
		// {
			if (strcmp(args[i], "exit") == 0)
			{
				exitFunc(args, nargs);
			}

			else if (strcmp(args[i], "cd") == 0)
			{
				cdFunc(args, nargs);
				break;
			}

			else if (strcmp(args[i], "ls") == 0)
			{
				lsFunc(args, nargs);
			}

			else if (strcmp(args[i], "pwd") == 0)
			{
				pwdFunc(args, nargs);
			}

		// }
		    else if (args[i] == NULL) break;

		else fprintf(stderr, "Error: command not recognized");
	}
}

//////////////////////////////////////////////////
//            command Handling Functions        //
//////////////////////////////////////////////////
// all command handling functions have the same
// parameter types and return values.
//////////////////////////////////////////////////

//+
// Function:	all handling functions
//
// Purpose:	this command performs the funcdtion
//		associated with the commands.
//		The name of the comomand is in first argument.
//	 	Since it is called by doCommand, it is known
//		that arg is at least one element long
//
// Parameters:
//	args	command and parameters, an array of pointers to strings
//	nargs	number of entries in the args array
//
// Returns	nothing (void)
//-

// TODO step 4b put command handling function for exit here
// e.g. void exitFunc(char * args[], int nargs){
// }

void exitFunc(char *args[], int nargs)
{
	exit(0);
}



// TODO step 6 put rest of command handling functions here

void cdFunc(char *args[], int nargs)
{
	//value to check if the directory was changed succesfully
	int  chDirVal=0;
	
	//if no directory is specified
	if (nargs == 1){
		struct passwd *pw = getpwuid(getuid());
		
		//checks to see if pw is null
		if (pw == NULL){
			fprintf(stderr, "Error: passwd struct is null");
		}
		
		//change to home directory
		else{
			 chDirVal = chdir(pw->pw_dir);
		}
	}
	//directory specified
	else if(nargs ==2){
		chDirVal = chdir(args[1]);

	}

	if(chDirVal != 0){
		fprintf(stderr, "Error: Directory does not exist");
	}



}

void lsFunc(char *args[], int nargs)
{
    int numEnts;
	struct dirent ** namelist;

	printf("test");

	//this compares in the case that there is a second argument, and the second argument is "-a", then it will print all files including hidden ones.
	if (nargs == 2 && strcmp(args[1], "-a") == 0)
	{
		numEnts = scandir(".", &namelist, NULL, NULL);
	}

	//this compares if there is only 1 argument, and that being the ls, then it will not print the hidden files by using the filter.
	else if (nargs == 1)
	{
		numEnts = scandir(".", &namelist, (int) *dotCheck(*namelist), NULL);
	}

	else fprintf(stderr, "Error: invalid second argument");

	//for loop loops through the namelist which contains all the file names and prints them.
	int i;
	for (i = 0; i < numEnts; i++)
	{
		printf("%s%s\n", getcwd(NULL, 0), namelist[i]->d_name);
		free(namelist[i]);
	}
	free(namelist);
}

void pwdFunc(char *args[], int nargs)
{
	char *cwd = getcwd(NULL, 0);
	printf("%s", cwd);
	free(cwd);
}

int dotCheck(const struct dirent *d)
{
	//this checks if the first file in the directory contains a '.'
	if (d->d_name[0] == '.'){
		return 0;
	}
	else{
		return 1;
	}
	
}
