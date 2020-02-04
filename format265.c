/*
Name: Lucas Carvalho
Student ID: V00843641
File: format265.c
Last Modified: Oct 13th, 2016
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 300
#define MAX_SIZE 132

/* Global Variables */
char output[MAX_LINES * MAX_SIZE];
char string[MAX_SIZE];         /* Formatted string. */
int line_size = 0;             /* Keeps track of the size of the line */

/* Variables for checking formatting */
int FT = 0;
int LM = 0;
int LW = 0; 
int LS = 0;

int formatting_commands(char* line){
  /* Make a copy of the string */
  char test[MAX_SIZE];
  strncpy (test, line, MAX_SIZE);
  strtok (test, " \n");
  /* Detect a control sequence */
  if (!strncmp(test, ".FT", MAX_SIZE)) {
  	char* r = strtok (NULL, " \n");
  	if (!strncmp(r, "on", 3)) {
  	FT = 1;
    	}
	else {
    	FT = 0;
    	}
    return 1;
  }
  if (!strncmp(test, ".LM", MAX_SIZE)) {
  	int r = atoi( strtok (NULL, " \n") );
  	LM = r;
  	return 1;
  }
  if (!strncmp(test, ".LW", MAX_SIZE)) {
  	int r = atoi( strtok (NULL, " \n") );
   	LW = r;
    	FT = 1;
	return 1;
  }
  if (!strncmp(test, ".LS", MAX_SIZE)){
	int r = atoi( strtok (NULL, " \n") );
	LS = r;
	return 1;
  }
  return 0;
}

void process_line (char* line) {
  /* Split the list into tokens */
  char* word = strtok (line, " \n");
  
  /* For each word, check if need a new line, set margins, and cat it. */
  while (word) {
    if (line_size + strlen (word) >= LW) {
      strncat (string, "\n", 1);
      line_size = 0;
    if (LS){
          strncat(string, "\n", 1);
        }
	/* Check if need to set margins. */
      if (LM) {
     	while (line_size < LM) {
        	strncat (string, " ", 1);
        	line_size++;
        }
     }

    }
    /* Otherwise, add a space. */
    else if (line_size >= LM+1) {
      line_size++;
      strncat (string, " ", 1);
    }
    
    line_size += strlen (word);
    strncat (string, word, MAX_SIZE);
    word = strtok (NULL, " \n");
  }
  
  return;
}

int main (int argc, char* argv[]) {
  FILE* file;
  char line[MAX_SIZE];
  
  /* Read the file, return an error if it's not valid. */
  file = fopen (argv[1], "r");
  if (file == NULL) {
	printf("usage: argv[1] = filename\n");
    return -1;
  }
  
  /* Read through each line, stop at the end. */
  while (fgets(line, MAX_SIZE, file)){
    /* Check for control sequences. */
    if (formatting_commands(line)){
      continue;
    }

    /* Checking cases for the first line of the output. */
    if (LM){
      /* Create as many margin spaces as we need. */
      while (line_size < LM){
        strncat (string, " ", 1);
        line_size++; /* Make sure we adjust the line size. */
      }
    }

    if (FT && !strncmp (line, "\n", 1)){
      strncat (output, "\n\n", MAX_SIZE);
      line_size = 0;
    }
    if (FT){
      process_line (line);
      strncat (output, string, MAX_SIZE);
      strncpy (string, "", MAX_SIZE);
    }
    else{
      strncat (output, line, MAX_SIZE);
    }
  }

  if (FT){
    strncat (output, "\n", MAX_SIZE);
  }
  
  printf("%s", output);
  
  return 0;
}
