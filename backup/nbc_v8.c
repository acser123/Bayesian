#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEP ","
#define MAXN 512    /* if you need a constant, #define one (or more) */

/* Structue for a node in the list that stores the entire file we will read in */
typedef struct list_node {
   char *label;
   int count; 
   double P; // Probability
   struct list_node *next; // Horizontal pointer
} list_node;

/* Structure to hold the list */ 
typedef struct list {
   list_node *head;
   struct list *next;
   size_t size;
} list;

/* Structure to hold the table of lists */ 
typedef struct table {
   list *head;
   size_t size;
} table;

/* --- Begin of list functions --- */

/* Create a dynamically allocated list */
list *new_list(void) {
   list *temp = malloc (sizeof *temp); /* create storage for list */
   if (!temp) {                         /* validate all allocations */
      perror("malloc-new_list");
      return NULL;
   }
   temp->head = NULL;
   temp->size = 0;
   return temp;
}

/* create new dynamically allocated node, initialize all values */
list_node *create_new_list_node (const char *label)
{
    list_node *new_node = NULL;

    if (!label) {    /* validate label not NULL */
        fputs ("error: label is NULL in create_new_node.\n", stderr);
        return NULL;
    }

    new_node = malloc (sizeof (struct list_node));   /* allocate/validate node */
    if (!new_node) {
        perror ("malloc-new_node");
        return NULL;
    }


    /* allocate/validate storage for label */

    if (!(new_node->label = malloc (strlen (label) + 1))) {
        perror ("malloc-new_node->label");
        free (new_node);
        return NULL;
    }
    strcpy (new_node->label, label);  /* copy data to new_node->data */
    new_node->next = NULL;          /* set next pointer NULL */

    return new_node;    /* return pointer to new_node */
}


/* insert new node at front, returning pointer to head
 * to guage success/failure of addition.
 */
void insert_node_at_end (list *mylist_in, list_node *node_in)
{

    if (node_in != NULL) {
        if (mylist_in->head == NULL){
            mylist_in->head = node_in;
            return;
        }
        list_node *tmp = mylist_in->head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = node_in;
    }

}

/* print_list - tweaked for formatted output */
void print_list (list *list_in)
{
    list_node *current = list_in->head;

    while (current != NULL)
    {
        if (current == list_in->head)
            printf ("%s", current->label);
        else
            printf (",%s", current->label);
        current = current->next;
    }

    putchar ('\n');
}


/* you are responsible for freeing any memory you allocate */
void free_list (list *mylist)
{
    list_node *current = mylist->head;
    while (current) {
        list_node *victim = current;
        current = current->next;
        free (victim->label);
        free (victim);
    }
    free (mylist);
}
/* --- End of list functions --- */


/* --- Begin of table functions --- */

/* Create a dynamically allocated table */
table *new_table(void) {
   table *temp = malloc (sizeof *temp); /* create storage for list */
   if (!temp) {                         /* validate all allocations */
      perror("malloc-new_table");
      return NULL;
   }
   temp->head = NULL;
   temp->size = 0;
   return temp;
}


/* create new dynamically allocated node, initialize all values */
list *create_new_list (list_node *list_node_h)
{
    list *new_list = NULL;
    if (!list_node_h) {    /* validate data not NULL */
        fputs ("error: list_node_in is NULL in create_new_list.\n", stderr);
        return NULL;
    }

    new_list = malloc (sizeof *new_list);   /* allocate/validate list */
    if (!new_list) {
        perror ("malloc-new_list");
        return NULL;
    }


    /* allocate/validate storage for data */

    new_list->head = NULL; 
    new_list->next = NULL;          /* set next pointer NULL */
    
    return new_list;    /* return pointer to new_node */
}

/* insert new list at front returning pointer to head
 * to guage success/failure of addition.
 */
void insert_list_at_end (table *training_table_in, list *mylist_in)
{
    if (mylist_in != NULL) {
        if (training_table_in->head == NULL) {
            training_table_in->head = mylist_in;
            training_table_in->size++;
            return;
        }
        /* Step to the end of the list */
        list  *tmp = training_table_in->head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = mylist_in;
        training_table_in->size++;
    }


}


/* print_list - tweaked for formatted output */
void print_table (table *table_in)
{
    list *current = table_in->head;
    int i;
    while (current != NULL)
    {
	i++;
        //printf("*debug* print_list(): i=%d\n", i);

    	print_list (current);

        current = current->next;
   
    }

}


/* you are responsible for freeing any memory you allocate */
void free_table (table *training_table)
{
    list *current = training_table->head;
    while (current) {
        list *victim = current;
        current = current->next;
//        free (victim->list);
        free (victim);
    }
    free (training_table);
}
/* --- End of table functions --- */

/* you are responsible for the state of stdin when doing user input */
void empty_stdin (void)
{
    int c = getchar();

    while (c != '\n' && c != EOF)
        c = getchar();
}

/* Tokenize a line to strings and put them into a list consisting of nodes */

char * tokenize(list *mylist, char *str) {
   //char str[] ="1,2,3,4,5";
   char *pt;
   list_node *node;
   pt = strtok (str,SEP);
   while (pt != NULL) {
    //printf("*debug* tokenize(): create_new_list_node() called with %s\n", pt);
    node = create_new_list_node(pt);
    //printf("*debug* tokenize(): create_new_list_node() completed\n");
    insert_node_at_end (mylist, node);         /* insert name */
     pt = strtok (NULL, SEP);
   }   
   //printf("*debug* tokenize(): print_list():"); print_list (mylist);
   return pt;
} 

/* Read file line by line and store it into a table consiting dynamic lists e */
int readfile(table *training_table, char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        list *mylist;
        mylist = new_list();
        insert_list_at_end(training_table, mylist);

        //printf("*debug* readfile() before tokenization: print_list():"); print_list (mylist);

        //printf("*debug* readfile(): Retrieved line of length %zu:\n", read);
        //printf("%s", line);
        line[strlen(line)-1] = '\0';  // Remove the last character of line \n


        
	tokenize(mylist, line);
        //printf("*debug* readfile(): line done\n\n");
        	
        //printf("*debug* print_list(): i=%d\n", i);
    }
        //printf("*debug* readfile(): tokenize() done\n\n\n");

    fclose(fp);
    if (line)
        free(line);
    return 0;
}


/* --- Column functions --- */
void get_unique_counts(list *list_in, table *table_in) {

    list *current_list = table_in->head;
    int i;
    while (current_list != NULL)
    {
	i++;

        list_node *current_node = current_list->head;

        while (current_node != NULL) {
            printf ("%s,", current_node->label);
            current_node = current_node->next;
        }
        printf ("\n");

        current_list = current_list->next;
   
    }
}

int main (int argc, char *argv[]) {
    char *fn=argv[1]; 
    fn="training.txt";

    /* --- Begin of reading in the training data --- */ 
    table *training_table;

    training_table = new_table();
    
    readfile(training_table, fn);

    //print_table(training_table);

    /* End of reading in the training data */ 
 
    /* --- Begin training --- */ 

    /* Read and analyze all training table values */
    table *parameter_table = new_table();
    list *Ci = new_list();
    insert_list_at_end(parameter_table, Ci);

    get_unique_counts(Ci, training_table);

    /* Identify unique values of the last column, C  or Class */
    /* Count unique values of the last column, count(Ci) */
    /* Compute probability values of each of the last column's unique values, P(Ci) */

    /* Identify unique values of each column, xi */
    /* Count unique values of each column, count(xi) */
    /* Compute probability values of each column's unique values, P(xi) */

    /* Calculate probability values of P(xi|Ci) */

    /* End of training */ 


    free_table (training_table);     /* don't forget to free memory you allocate */

    return 0;
}

