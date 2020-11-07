#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEP ","
char * tokenize(char *str) {

   //char str[] ="1,2,3,4,5";
   char *pt;
   pt = strtok (str,SEP);
   while (pt != NULL) {
     printf("%s\n", pt);
     pt = strtok (NULL, ",");
   }   
   return pt;
} 

int readfile(char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *tokenized = NULL;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
	tokenized = tokenize(line);
        	
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}


int main (int argc, char *argv[]) {
   char *fn=argv[1]; 
   fn="training.txt";
   readfile(fn);
   return 0;   
}
