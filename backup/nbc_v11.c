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
   list *list_out = malloc (sizeof *list_out); /* create storage for list */
   if (!list_out) {                         /* validate all allocations */
      perror("malloc-new_list");
      return NULL;
   }
   list_out->head = NULL;
   list_out->size = 0;
   return list_out;
}

/* create new dynamically allocated node, initialize all values */
list_node *create_new_list_node (const char *label)
{
    list_node *node_out = NULL;

    if (!label) {    /* validate label not NULL */
        fputs ("error: label is NULL in create_new_node.\n", stderr);
        return NULL;
    }

    node_out = malloc (sizeof (struct list_node));   /* allocate/validate node */
    if (!node_out) {
        perror ("malloc-new_node");
        return NULL;
    }


    /* allocate/validate storage for label */

    if (!(node_out->label = malloc (strlen (label) + 1))) {
        perror ("malloc-node_out->label");
        free (node_out);
        return NULL;
    } 

    /* copy data to node_out->data */
    strcpy (node_out->label, label);

    /* Initialize the non-pointers */
    node_out->count = 0;  
    node_out->P = 0;

    /* set next pointer NULL */
    node_out->next = NULL;         
 
    /* return pointer to new_node */
    return node_out;
}


/* insert new node, node_in at front to mylist_in 
 */
void insert_node_at_end (list *mylist_in, list_node *node_in)
{

    if (node_in != NULL) {
        if (mylist_in->head == NULL){
            mylist_in->head = node_in;
            return;
        }

        /* Go to the end of the list */
        list_node *current = mylist_in->head;

        while (current->next != NULL) {
            current = current->next;
        }

        current->next = node_in;
        mylist_in->size = mylist_in->size + 1;
    }

}

/* print_list - print nodes's values on a list */
void print_list (list *list_in)
{
    list_node *current = list_in->head;

    while (current != NULL)
    {
        if (current == list_in->head)
            printf ("%s count=%d P=%f", current->label, current->count, current->P);
        else
            printf (",%s count=%d P=%f", current->label, current->count, current->P);
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
   table *table_out = malloc (sizeof *table_out); /* create storage for list */
   if (!table_out) {                         /* validate all allocations */
      perror("malloc-table_out");
      return NULL;
   }
   table_out->head = NULL;
   table_out->size = 0;
   return table_out;
}


/* insert new list at front returning pointer to head
 * to guage success/failure of addition.
 */
void insert_list_at_end (table *table_in, list *mylist_in)
{
    if (mylist_in != NULL) {
        if (table_in->head == NULL) {
            table_in->head = mylist_in;
            table_in->size++;
            return;
        }

        /* Step to the end of the list */
        list  *current = table_in->head;

        while (current->next != NULL) {
            current = current->next;
        }

        current->next = mylist_in;

        table_in->size++;
    }
}


/* print_table - tweaked for formatted output */
void print_table (table *table_in)
{
    list *current = table_in->head;

    while (current != NULL)
    {
    	print_list (current);

        current = current->next;
    }
}


/* you are responsible for freeing any memory you allocate */
void free_table (table *training_table)
{ list *current = training_table->head; while (current) {
        list *victim = current;
        current = current->next;
        free_list (victim);
    }
    free (training_table);
}

/* --- End of table functions --- */




/* --- Begin of column functions --- */

/* Create vertical pointers between a node on a list (row) and the 
respective node on the next list (row) for the whole table_in */
void cross_link_nodes(table *table_in) {

    list *curr_list_ptr = table_in->head;

    /* Step to the end of the first list (line) of the table */
    while (curr_list_ptr != NULL) {
        list *next_list_ptr = curr_list_ptr->next;

        if (next_list_ptr != NULL) {
                 
            list_node *curr_node_ptr = curr_list_ptr->head;

            /* This list points to the one below curr_nod_ptr */
            list_node *below_node_ptr = next_list_ptr->head;

            while (below_node_ptr!= NULL) {

                /* Link to next line's respective node */
                curr_node_ptr->next_v = below_node_ptr;          

                /* Advance both current and below node pointers */ 
                curr_node_ptr = curr_node_ptr->next;
                below_node_ptr = below_node_ptr->next;
                 
            }
            
        }
        /* Move to the next line (row) */ 
        curr_list_ptr = curr_list_ptr->next;
    }  
}

/* retrieve_column retrieve the column that has header_in */
list * retrieve_column (table *table_in, char *header_in) {
    /* Create the return list */
    list *list_out = new_list();

    /* Find the upper left hand list_node */
    list *first_list = table_in->head;
    list_node *column_node_ptr = first_list->head;

    /* Print data column-wise using the next_v vertical pointer that matches the header_in */ 
    while(column_node_ptr != NULL) {
        if( !strcmp(column_node_ptr->label, header_in)) {
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

    /* We will advance this pointer */   
    list_node *curr_node = list_in->head;


    /* The first node label is unique for sure so we initialize the unique list */
    list_node *unique_list_node = create_new_list_node(curr_node->label);
    unique_list_node->count++;
    insert_node_at_end(unique_list_out, unique_list_node);
    
    /* Step to the second item on the current node */
    curr_node = curr_node->next; 

    /* We already looked at the first row */
    int numrows=1; 

    /* Step through nodes of the list_in */
    while (curr_node != NULL) {
        numrows++;

        /* Go the beginning of the unique list */
        unique_list_node = unique_list_out->head;

        int found=0;
        /* We need to remember what we need to add to unique list */
        list_node *add_node;

        /* Step through unique list */
        while (unique_list_node != NULL) { 

            /* If unique list contains the current node, set found to 1 
               so that we don't add it after having finished with the unique list */
            if ((strcmp(unique_list_node->label, curr_node->label) == 0)) {

                add_node = create_new_list_node(curr_node->label);

                /* Need to increase the count of times we found this node */
                unique_list_node->count++;
                /* Set found flag so we are sure to not add this node to uniques */
                found = 1;

            } else {
                /* Need to remember what we're adding below, otherwise we dump core below */ 
                add_node = create_new_list_node(curr_node->label);
            }
            unique_list_node = unique_list_node->next;
        }

        /* If we have not found the current on the unique list, then add the current node to the unique list */
        if (found==0)  { 
            add_node->count++;
            insert_node_at_end(unique_list_out, add_node);
            found=1;
        } 

        /* Step to examine the next node */
        curr_node = curr_node->next; 
    }


    /* Calculate and populate probabilities */
    list_node *current = unique_list_out->head; 

    /*  Step through unique_list_out */
    while (current != NULL) {

        /* Calculate probability for each unique value */
        current->P = (current->count) / (double) numrows; 

        /* Step to calculate P for the next node */
        current = current->next;
    }

    return unique_list_out;
}


/* --- End of column functions --- */


/* --- Begin file processing functions --- */

/* Tokenize a line to strings and put them into a list consisting of nodes */

list * tokenize(char *str) {

   list *list_out = new_list();
   char *pt;
   list_node *node;
   pt = strtok (str,SEP);
   while (pt != NULL) {
    node = create_new_list_node(pt);
    insert_node_at_end (list_out, node);         /* insert name */
    pt = strtok (NULL, SEP);
   }   
   return list_out; 
} 

/* Read file line by line and store it into a table consiting dynamic lists e */
table * readfile(char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    table * table_out  = new_table();

    fp = fopen(filename, "r");

    if (fp == NULL)
        exit(EXIT_FAILURE);

    /* Step to the end of the file */
    while ((read = getline(&line, &len, fp)) != -1) {
        list *mylist;
        mylist = new_list();


        line[strlen(line)-1] = '\0';  // Remove the last character of line \n

        /* Get comma separated values into mylist */ 
	mylist = tokenize(line);

        /* insert the list into the table */
        insert_list_at_end(table_out, mylist);
        	
    }

    fclose(fp);
    if (line)
        free(line);

    return table_out;
}

/* --- End file processing functions --- */

int main (int argc, char *argv[]) {
    char *fn=argv[1]; 
    fn="training.txt";

    /* --- Begin of reading in the training data --- */ 
    table *training_table;

    training_table = new_table();
    
    training_table = readfile(fn);

    //print_table(training_table);

    /* End of reading in the training data */ 
 
    /* --- Begin training --- */ 

    /* Cross link nodes in the training table so we can walk it vertically as well*/
    cross_link_nodes(training_table); 
    list *testcolumn = new_list();
    testcolumn = retrieve_column(training_table, "Outlook");
    //print_list(testcolumn);
    list *counts_list = new_list();
    counts_list = get_uniques(testcolumn);
    print_list(counts_list);

    testcolumn = retrieve_column(training_table, "Humidity");
    //print_list(testcolumn);
    counts_list = new_list();
    counts_list = get_uniques(testcolumn);
    print_list(counts_list);

    testcolumn = retrieve_column(training_table, "Wind");
    //print_list(testcolumn);
    counts_list = new_list();
    counts_list = get_uniques(testcolumn);
    print_list(counts_list);

    testcolumn = retrieve_column(training_table, "Play");
    //print_list(testcolumn);
    counts_list = new_list();
    counts_list = get_uniques(testcolumn);
    print_list(counts_list);


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

