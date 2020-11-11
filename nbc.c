#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEP ","
#define MAXN 512    /* if you need a constant, #define one (or more) */

/* --- Begin of structure definitions --- */
/* Structue for a node in the list that stores the entire file we will read in */
typedef struct list_node {
   char *label;
   int count; 
   double P; // Probability
   struct list_node *next; // Horizontal pointer
   struct list_node *next_v; // Vertical pointer to the node in the following line
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

/* --- End of structure definitions --- */

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


    /* allocate/validate storage for count */

  //  if (!(new_node->count = malloc (sizeof (int) + 1))) {
   //     perror ("malloc-new_node->count");
    //    free (new_node);
     //   return NULL;
    //}
    new_node->count = 0;  /* initialize count to 0*/


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
            printf ("%s count=%d", current->label, current->count);
        else
            printf (",%s count=%d", current->label, current->count);
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



/* print_table - tweaked for formatted output */
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




/* --- Begin of column functions --- */

/* Create vertical pointers between a node on a list and the 
respective node on the next list */
void cross_link_nodes(table *table_in) {
    list *curr_list_ptr = table_in->head;
    while (curr_list_ptr != NULL) {
        list *next_list_ptr = curr_list_ptr->next;
        if (next_list_ptr != NULL) {
            list_node *curr_node_ptr = curr_list_ptr->head;
/* This list points to the one below curr_nod_ptr */
            list_node *below_node_ptr = next_list_ptr->head;
            while (below_node_ptr!= NULL) {

                /* Link to next line's respective node */
                curr_node_ptr->next_v = below_node_ptr;          
            //printf ("*debug* cross_link_nodes(): linking node %s to node %s\n", curr_node_ptr->label, below_node_ptr->label);

                /* Advance both pointers */ 
                curr_node_ptr = curr_node_ptr->next;
                below_node_ptr = below_node_ptr->next;
                 
            }
            
        }
        /* Move to the next line */ 
        curr_list_ptr = curr_list_ptr->next;
    }  
}

/* retrieve_column retrieve the column that has header_in */
list * retrieve_column (table *table_in, char *header_in)
{
    /* Create the return list */
    list *list_out = new_list();

    /* Find the upper left hand list_node */
    list *first_list = table_in->head;
    list_node *column_node_ptr = first_list->head;

    /* Print data column-wise using the next_v vertical pointer that matches the header_in */ 
    while(column_node_ptr != NULL) {
        if( !strcmp(column_node_ptr->label, header_in)) {
      // printf ("*debug* retrieve_column(): column_node_ptr->label = %s\n", column_node_ptr->label);
            list_node *current_node = column_node_ptr;
            while (current_node != NULL) {
                //printf ("%s,", current_node->label);

		/* Need to create a new node to insert, can't insert current_node directly */
                insert_node_at_end(list_out, create_new_list_node(current_node->label));
                current_node = current_node->next_v;
            }
       //     printf ("\n");
        } 
        /* Move to the next column */
        column_node_ptr = column_node_ptr->next;

     } 
     //putchar ('\n');
     return list_out; 
}

list * get_uniques(list *list_in) {

    list *unique_list_out = new_list();
   
    list_node *curr_node = list_in->head;


    /* The first node label is unique for sure so we initialize the unique list */
    list_node *unique_list_node = create_new_list_node(curr_node->label);
    unique_list_node->count++;
    insert_node_at_end(unique_list_out, unique_list_node);
    
    /* Step to the second item on the current node */
    curr_node = curr_node->next; 

    int numrows=1;
    while (curr_node != NULL) {
        printf("*debug* get_uniques(): evaluation curr_node->label=%s; numrows=%d\n", curr_node->label, numrows);
        numrows++;
        /* Go the beginning of the unique list */
        unique_list_node = unique_list_out->head;
        int found=0;
        //int i;
        list_node *add_node;
        /* Step through unique list */
        while (unique_list_node != NULL) { 
         //   printf("*debug* get_uniques(): in unique while(), equal unique_list_node->label=%s; curr_node->label=%s\n", unique_list_node->label, curr_node->label);
            /* If unique list contains the current node, set found to 1 
               so that we don't add it after having finished with the unique list */
            if ((strcmp(unique_list_node->label, curr_node->label) == 0)) {
            //    printf("*debug* get_uniques(): inner while(), equal unique_list_node->label=%s; curr_node->label=%s\n", unique_list_node->label, curr_node->label);
                add_node = create_new_list_node(curr_node->label);
                unique_list_node->count++;
                printf("*debug* get_uniques(): inner while(), row=%d; equal unique_list_node->label=%s; curr_node->label=%s; unique_list_node->count=%d;\n", numrows, unique_list_node->label, curr_node->label, unique_list_node->count);
                found = 1;
            } else {

                add_node = create_new_list_node(curr_node->label);
            }
            unique_list_node = unique_list_node->next;
        //printf("*debug* get_uniques(): unique_list="); print_list(unique_list_out);
        }
        //printf("*debug* get_uniques(): inner while() finished, found=%d, add_node->label=%s\n", found, add_node->label);
        if (found==0)  { 
             //add_node->count = i;
             insert_node_at_end(unique_list_out, add_node);
        
            //printf("*debug* get_uniques(): inserted node at end of uniques add_node->label=%s\n", add_node->label);
        //printf("*debug* get_uniques(): after insert unique_list="); print_list(unique_list_out);
            found=1;
        } 


        curr_node = curr_node->next; 
        //printf("*debug* get_uniques(): next curr_node\n\n");
    }


    return unique_list_out;
}


/* --- End of column functions --- */




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

    /* Cross link nodes in the training table so we can walk it vertically as well*/
    cross_link_nodes(training_table); 
    list *testcolumn = new_list();
    testcolumn = retrieve_column(training_table, "Outlook");
    print_list(testcolumn);
    list *counts_list = new_list();
    counts_list = get_uniques(testcolumn);
    print_list(counts_list);
/*
    testcolumn = retrieve_column(training_table, "Humidity");
    print_list(testcolumn);
    counts_list = new_list();
    counts_list = get_uniques(testcolumn);
    print_list(counts_list);

    testcolumn = retrieve_column(training_table, "Wind");
    print_list(testcolumn);
    counts_list = new_list();
    counts_list = get_uniques(testcolumn);
    print_list(counts_list);

    testcolumn = retrieve_column(training_table, "Play");
    print_list(testcolumn);
    counts_list = new_list();
    counts_list = get_uniques(testcolumn);
    print_list(counts_list);
*/

    /* Read and analyze all training table values */
    table *parameter_table = new_table();
    list *Ci = new_list();
    insert_list_at_end(parameter_table, Ci);


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

