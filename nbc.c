#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEP ","
#define DECISION_FOR "?"
#define MAXN 512    /* if you need a constant, #define one (or more) */
#define CLASS_NAME "Outlook" /* This is the variable or header column we are trying to forecast */


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
    struct table *next;
    size_t size;
    char *label;
} table;

/* Structure to hold a folder of tables */
typedef struct folder {
    table *head;
    struct folder *next;
    size_t size;
    char *label;
} folder; 

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

/* Create a new dynamically allocated node and initialize all its values.
   This function ensures that all fields of the list_node struct are properly
   initialized to prevent uninitialized value errors.
   - label: The string label for the node.
   - returns: A pointer to the newly created list_node, or NULL on failure.
*/
list_node *create_new_list_node(const char *label) {
    if (!label) {
        fputs("error: label is NULL in create_new_list_node.\n", stderr);
        return NULL;
    }

    list_node *node_out = malloc(sizeof(list_node));
    if (!node_out) {
        perror("malloc-new_node");
        return NULL;
    }

    node_out->label = malloc(strlen(label) + 1);
    if (!node_out->label) {
        perror("malloc-node_out->label");
        free(node_out);
        return NULL;
    }
    strcpy(node_out->label, label);

    node_out->count = 0;
    node_out->P = 0.0;
    node_out->next = NULL;
    node_out->next_v = NULL;

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

/* print_node - print a node */
void print_node(list_node *node_in) {
   if (node_in != NULL) {
       printf("print_node: label=%s\n", node_in->label);
       printf("print_node: count=%d\n", node_in->count);
       printf("print_node: P=%f\n", node_in->P);
       if (node_in->next != NULL) {
           printf("print_node: next=%s\n", (node_in->next)->label);
       } else {
           printf("print_node: next=NULL\n");
       }
       if (node_in->next_v != NULL) {
           printf("print_node: next_v=%s\n", (node_in->next_v)->label);
       } else {
           printf("print_node: next_v=NULL\n");
       }
   } 
   
   
}
/* print_list - print nodes's values on a list */
void print_list (list *list_in)
{
if (list_in != NULL ) {
    list_node *mycurrent = list_in->head;
    int i=0;
    while (mycurrent != NULL)
    {      
            i++;
        if (mycurrent == list_in->head)
            //printf ("%s count=%d P=%f", mycurrent->label, mycurrent->count, mycurrent->P);
            //printf ("%s P=%f", mycurrent->label, mycurrent->P);
            printf ("%s", mycurrent->label);
        else
            //printf (",%s count=%d P=%f", mycurrent->label, mycurrent->count, mycurrent->P);
            //printf (",%s P=%f", mycurrent->label, mycurrent->P);
            printf (",%s", mycurrent->label);
            //print_node(mycurrent);
        mycurrent = mycurrent->next;
    }

    putchar ('\n');
 }
    //printf ("*debug*: print_list finished\n" );
}


/* you are responsible for freeing any memory you allocate */
void free_list (list *mylist)
{
    list_node *current = mylist->head;
    while (current) {
        list_node *victim = current;
        free (victim->label);
        free (victim);
        current = current->next;
    }
    free (mylist);
}
/* --- End of list functions --- */


/* --- Begin of table functions --- */
/* print_table - tweaked for formatted output */
void print_table (table *table_in)
{
    if (table_in != NULL) {
        list *current = table_in->head;
        while (current != NULL)
        {
    	    print_list (current);
            current = current->next;
        }
    }
}

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

/* Add a label to a table. This function is now safer and avoids a double-free vulnerability. */
void add_label_to_table(table *table_in, const char *label) {
    if (!label) {
        table_in->label = NULL;
        return;
    }
    table_in->label = malloc(strlen(label) + 1);
    if (!table_in->label) {
        perror("malloc-table_in->label");
        return; /* Avoid freeing the table here, which could lead to a double-free. */
    }
    strcpy(table_in->label, label);
}
/* insert new list at front returning pointer to head
 * to guage success/failure of addition.
 */
void insert_list_at_end (table *table_in, list *mylist_in) {

         
    if (mylist_in != NULL) {
        if (table_in->head == NULL) {
            table_in->head = mylist_in;
            mylist_in->next = NULL;
            return;
        } else {

            /* Step to the end of the list */
            list *current = table_in->head;

            while (current->next != NULL) {
                current = current->next;
            }

            current->next = mylist_in;
	    /* Make sure to terminate the list, otherwise printing coredumps */
            mylist_in->next = NULL;
        }
    }
}



/* Safely free a table, including its label and all lists within it. */
void free_table(table *table_in) {
    if (!table_in) {
        return;
    }
    list *current = table_in->head;
    while (current) {
        list *victim = current;
        current = current->next;
        free_list(victim);
    }
    free(table_in->label);
    free(table_in);
}

/* --- End of table functions --- */


/* --- Begin of folder functions --- */


/* Create a dynamically allocated folder */
folder *new_folder(void) {
   folder *folder_out = malloc (sizeof *folder_out); /* create storage for folder */
   if (!folder_out) {                         /* validate all allocations */
      perror("malloc-folder_out");
      return NULL;
   }
   folder_out->head = NULL;
   folder_out->size = 0;
   return folder_out;
}


/* insert new table at front returning pointer to head
 * to guage success/failure of addition.
 */
void insert_table_at_end (folder *folder_in, table *table_in)
{
    if (table_in != NULL) {
        if (folder_in->head == NULL) {
            folder_in->head = table_in;
            folder_in->size++;
            return;
        }

        /* Step to the end of the tables */
        table  *current = folder_in->head;

        while (current->next != NULL) {
            current = current->next;
        }

        current->next = table_in;

        folder_in->size++;
    }
}


/* Add a label to a folder, ensuring safety against double-frees. */
void add_label_to_folder(folder *folder_in, const char *label) {
    if (!label) {
        folder_in->label = NULL;
        return;
    }
    folder_in->label = malloc(strlen(label) + 1);
    if (!folder_in->label) {
        perror("malloc-folder_in->label");
        return; /* Avoid freeing the folder here */
    }
    strcpy(folder_in->label, label);
}

/* print_folder - tweaked for formatted output */
void print_folder (folder *folder_in)
{
    table *current = folder_in->head;

    while (current != NULL)
    {
    	print_table (current);

        current = current->next;
    }
}


/* Safely free a folder, its label, and all tables within it. */
void free_folder(folder *folder_in) {
    if (!folder_in) {
        return;
    }
    table *current = folder_in->head;
    while (current) {
        table *victim = current;
        current = current->next;
        free_table(victim);
    }
    free(folder_in->label);
    free(folder_in);
}



/* --- End of folder functions --- */

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

    /* Go to the last line of the table */
    curr_list_ptr = table_in->head;
    while(curr_list_ptr->next != NULL) { 
        curr_list_ptr = curr_list_ptr->next;
    }


    list_node *curr_node_ptr = curr_list_ptr->head;

    while( curr_node_ptr != NULL) {
       /* terminate the last rows' next_v's with NULLs */
       curr_node_ptr->next_v = NULL; 
       curr_node_ptr  = curr_node_ptr->next;
    }
}

/* retrieve_column retrieve the column whose header (first row) matches header_in */
list * retrieve_column (table *table_in, const char *header_in) {
    /* Create the return list */
    list *list_out = new_list();

    //list *list_tmp = new_list();   

    /* Find the upper left hand list_node */
    list *first_list = table_in->head;
    list_node *column_node_ptr = first_list->head;

    /* Print data column-wise using the next_v vertical pointer that matches the header_in */ 
    while(column_node_ptr != NULL) {

        if( !strcmp(column_node_ptr->label, header_in)) {
            list_node *current_node = column_node_ptr;

            while (current_node != NULL) {
                
  
		/* Need to create a new node to insert, can't insert current_node directly */
                //insert_node_at_end(list_out, current_node);
                insert_node_at_end(list_out, create_new_list_node(current_node->label));
                /* Move down one node vertically */
                current_node = current_node->next_v;
            }
        } 

        /* Move to the next column */
        column_node_ptr = column_node_ptr->next;

    } 

    return list_out; 
}

/* Get unique values from a list, along with their counts and probabilities.
   This function is memory-safe and avoids leaks by only allocating new nodes
   when a unique label is discovered.
   - list_in: The input list to process.
   - returns: A new list containing unique nodes with counts and probabilities.
*/
list *get_uniques(list *list_in) {
    list *unique_list_out = new_list();
    list_node *curr_node = list_in->head;

    if (!curr_node) {
        return unique_list_out; /* Return empty list if input is empty */
    }

    /* The first node is the header, add it to the unique list */
    list_node *unique_list_node = create_new_list_node(curr_node->label);
    unique_list_node->count++;
    insert_node_at_end(unique_list_out, unique_list_node);
    curr_node = curr_node->next;

    int numrows = 1;

    /* Step through the rest of the nodes in the list */
    while (curr_node != NULL) {
        numrows++;
        unique_list_node = unique_list_out->head;
        int found = 0;

        /* Check if the current node's label is already in our unique list */
        while (unique_list_node != NULL) {
            if (strcmp(unique_list_node->label, curr_node->label) == 0) {
                unique_list_node->count++;
                found = 1;
                break; /* Found, no need to check further */
            }
            unique_list_node = unique_list_node->next;
        }

        /* If the label was not found in the unique list, add it as a new node */
        if (!found) {
            list_node *add_node = create_new_list_node(curr_node->label);
            add_node->count++;
            insert_node_at_end(unique_list_out, add_node);
        }

        curr_node = curr_node->next;
    }

    /* Calculate and populate probabilities */
    list_node *current = unique_list_out->head;
    while (current != NULL) {
        /* Subtract 1 from numrows for the header row */
        if (numrows > 1) {
            current->P = (current->count) / ((double)numrows - 1);
        }
        current = current->next;
    }

    return unique_list_out;
}



/* Finds unique values and their probabilities for a specific column, based on a criterion in another column.
   This function is memory-safe and more efficient than its predecessor.
   - table_in: The table containing the data.
   - examine_header_in: The header of the column to analyze for unique values.
   - header_name_in: The header of the column to use for the criterion.
   - field_value_in: The value to match in the criterion column.
   - returns: A list of unique values with their counts and probabilities.
*/
list *get_uniques_with_criterion(table *table_in, const char *examine_header_in, const char *header_name_in, const char *field_value_in) {
    list *unique_list_out = new_list();
    if (!table_in || !table_in->head) {
        return unique_list_out;
    }

    list_node *header_row = table_in->head->head;
    list_node *examine_col_head = NULL;
    list_node *criterion_col_head = NULL;

    /* Find the starting nodes for the columns we are interested in */
    for (list_node *h = header_row; h != NULL; h = h->next) {
        if (strcmp(h->label, examine_header_in) == 0) {
            examine_col_head = h;
        }
        if (strcmp(h->label, header_name_in) == 0) {
            criterion_col_head = h;
        }
    }

    if (!examine_col_head || !criterion_col_head) {
        /* One of the headers was not found */
        return unique_list_out;
    }

    /* Add the header of the examined column to the unique list */
    list_node *unique_header_node = create_new_list_node(examine_col_head->label);
    unique_header_node->count++;
    insert_node_at_end(unique_list_out, unique_header_node);

    list_node *examine_node = examine_col_head->next_v;
    list_node *criterion_node = criterion_col_head->next_v;
    int matching_rows = 0;

    /* Iterate through the rows of the table */
    while (examine_node != NULL && criterion_node != NULL) {
        if (strcmp(criterion_node->label, field_value_in) == 0) {
            matching_rows++;
            list_node *unique_list_node = unique_list_out->head;
            int found = 0;

            /* Check if the value from the examine_node is already in our unique list */
            while (unique_list_node != NULL) {
                if (strcmp(unique_list_node->label, examine_node->label) == 0) {
                    unique_list_node->count++;
                    found = 1;
                    break;
                }
                unique_list_node = unique_list_node->next;
            }

            /* If not found, add a new node for it */
            if (!found) {
                list_node *add_node = create_new_list_node(examine_node->label);
                add_node->count++;
                insert_node_at_end(unique_list_out, add_node);
            }
        }
        examine_node = examine_node->next_v;
        criterion_node = criterion_node->next_v;
    }

    /* Calculate probabilities, skipping the header node */
    list_node *current_prob = unique_list_out->head;
    if (current_prob) {
        current_prob = current_prob->next; /* Skip header */
    }
    while (current_prob != NULL) {
        if (matching_rows > 0) {
            current_prob->P = (double)current_prob->count / matching_rows;
        } else {
            current_prob->P = 0.0;
        }
        current_prob = current_prob->next;
    }

    return unique_list_out;
}

/* --- End of column functions --- */

/* --- Begin of global training functions --- */

/* Create table of Pxi values */
/* This is a table of counts and probabilities of each unique value in each column of the training table */
void create_Pxi_table(table *table_out, table *table_in){

    list *curr_list = table_in->head;
    list_node *curr_node = curr_list->head;
    
    while (curr_node != NULL) {

        list *curr_column =  new_list();
        curr_column = retrieve_column(table_in, curr_node->label);
        list *counts_list = new_list();

	/* Find out the uniques values from the column */
        counts_list = get_uniques(curr_column);

        insert_list_at_end(table_out, counts_list);        
        curr_node = curr_node->next;
    }
}

/* Create a folder of tables containing P(xi|Ci) values.
   For each unique value of the class variable (Ci), this function calculates
   the conditional probabilities for all other features. This is a core part
   of training the Naive Bayes model.
   - training_table_in: The table with training data.
   - Cname_in: The name of the class variable.
   - returns: A folder containing tables of conditional probabilities.
*/
folder *create_PxiCi_folder(table *training_table_in, char *Cname_in) {
    folder *folder_out = new_folder();

    /* First, get the unique values of the class variable */
    list *C_column = retrieve_column(training_table_in, Cname_in);
    if (!C_column) {
        return folder_out; /* Class variable not found */
    }
    list *Ci_list = get_uniques(C_column);
    free_list(C_column); /* C_column is no longer needed */

    if (!Ci_list || !Ci_list->head) {
        if (Ci_list) free_list(Ci_list);
        return folder_out;
    }

    /* For each unique value of the class variable (e.g., "Y" or "N"), create a table */
    list_node *c_val_node = Ci_list->head->next; /* Skip header */
    while (c_val_node != NULL) {
        table *Ci_table = new_table();
        add_label_to_table(Ci_table, c_val_node->label);

        list_node *header_ptr_node = training_table_in->head->head;
        while (header_ptr_node != NULL) {
            /* For each feature, get uniques with the class criterion */
            list *counts_list = get_uniques_with_criterion(training_table_in, header_ptr_node->label, Cname_in, c_val_node->label);
            insert_list_at_end(Ci_table, counts_list);
            header_ptr_node = header_ptr_node->next;
        }

        insert_table_at_end(folder_out, Ci_table);
        c_val_node = c_val_node->next;
    }

    free_list(Ci_list); /* Clean up the list of unique class values */
    return folder_out;
}


/* --- End of global training functions --- */

/* --- Begin of decisioning functions --- */

/* From the Pxi table, for header_in column for value xi_in, get the P probability value we calculated when we built the Pxi_table_in */
double lookup_Pxi_value(table *Pxi_table_in, char *header_in, char *xi_in) {
    list *curr_list = Pxi_table_in->head;
    char *found_header=NULL; 
    /* Step through the rows of Pxi_table_in */
    while (curr_list != NULL) {

        list_node *curr_node = curr_list->head;

	/* Step through each column's value */
        while (curr_node != NULL) {

            /* Found the list in Pxi_table_in that begins with header_in */
            if (!strcmp(curr_node->label, header_in)) {
		found_header=curr_node->label;
                
		/* Look for xi_in and if found returnn the P value */
		while (curr_node != NULL) {
		    if(!strcmp(curr_node->label, xi_in)){
	                return(curr_node->P);
		    }
			    
		    curr_node = curr_node->next;
		}
	    } else {
            curr_node = curr_node->next;
	    }

        }
         
	curr_list = curr_list->next;
    }

    found_header=found_header;
    return 0;
}

/* From the PxiCi_folder_in's table which has P values for Ci_in, retrieve the probability where headder_in=xi_in */
/* This function is similar to lookup_Pxi_value() */

double lookup_PxiCi_value(folder *PxiCi_folder_in, char *header_in, char *xi_in, char *Ci_in) {

    table *curr_table = PxiCi_folder_in->head;
    /* Step through all tables with unique Ci_in values */
    while (curr_table != NULL) {

        /* When we find the table with Ci_in in it, step through it row by row */
        if (!strcmp(curr_table->label, Ci_in)) {
            list *curr_list = curr_table->head;

	    /* Step through rows */
            while (curr_list != NULL) {
            list_node *curr_node = curr_list->head;

	        /* Step through column values */ 
                while (curr_node != NULL) {

                    /* We found a column where the header is header_in */
                    if (!strcmp(curr_node->label, header_in)) {
                        
                        /* Step through individual values in header_in column */
		        while (curr_node != NULL) {

                            /* when we find the value of xi_in in the table, return the probability P of it */
		            if(!strcmp(curr_node->label, xi_in)){
	                        return(curr_node->P);
		            }
		            curr_node = curr_node->next;
	                }
	            } else {
                        curr_node = curr_node->next;
	            }
                }
         
	        curr_list = curr_list->next;
            }
	}       
        curr_table = curr_table->next;
    }
    return 0;
}


/* Get class variable */
char * get_class_variable(table *decisioning_table_in) {
    char *class_variable_out = "";
    list *curr_list = (decisioning_table_in->head)->next;
    list_node *curr_node = curr_list->head;
    /* Step through the whole second row of values in the decisioning table and find the position where the DECISION_FOR character (in our casea '?') is */
    int i = 0;
    int class_variable_position = 0;
    while (curr_node != NULL) {
	i++;
	if(!strcmp(curr_node->label, DECISION_FOR)) {
	   class_variable_position = i;  
	}
        curr_node = curr_node->next; 
    }

    /* Count the number of headers to class_variable_position times */
    curr_list = decisioning_table_in->head;
    curr_node = curr_list->head;
    i = 0;
    while (curr_node != NULL) {
        i++;
	if (i == class_variable_position) {
             class_variable_out = curr_node->label;
	}
        curr_node = curr_node->next;
    }
    return class_variable_out;
}


/* Get a decisioned table with calculated values in it using the
 * Naive Bayes Classifier method */
/* This is the worker function to generate the decisioned table based on the decisioning tables */
/* Pxi_table_in is the table with individual Pxi values, 
 * PxiCi_folder_in is the folder of tables of Pxi values for each Ci lalues for
 * decisioning_table_in is the table for whose rows we need to decision the Ci output */
table * get_decisioned_table(table *Pxi_table_in, folder *PxiCi_folder_in, table *decisioning_table_in) {
    table *table_out = new_table();

    /* For each value of the C, Ci compute the following !@#*/
    /* Copy the header row into header_list from decisioning_table_in */ 
    list *header_list = new_list();
    list_node *header_node_ptr = (decisioning_table_in->head)->head;

    /* Get the header row into the beginning of the decisioned table */
    while (header_node_ptr != NULL) {
        list_node *add_node; 
	add_node = create_new_list_node(header_node_ptr->label);
        insert_node_at_end(header_list, add_node);
        header_node_ptr = header_node_ptr->next;
    }

    /* Add header column */
    insert_list_at_end(table_out, header_list);

    /* Start from the second row of the decisioning table */
    list *curr_list = (decisioning_table_in->head)->next;

    /* Walk through the entire decisioning table row by row,  values only, but not headers, see above line */
    while (curr_list != NULL) {


	/* Initialize Bayes statistic value  */
	//double bayes_Ci = 1.0;
	

        list_node *curr_row_node = curr_list->head;
	list_node *curr_head_node = header_list->head;
        	
	table *curr_table2 = PxiCi_folder_in->head;
        /* Store bayes_Ci values into a list */
        list *bayes_values_list = new_list();
	while (curr_table2 != NULL) {
	list_node *curr_node = create_new_list_node(curr_table2->label);
	    curr_node->P = 1.0;
	    insert_node_at_end(bayes_values_list, curr_node);
	    curr_table2 = curr_table2->next;
	}

        list *curr_decisioned_list = new_list();
	/* Walk through the current row of values in decsioning table the  and find the Pxi value for each, then append them one by one to a list */
        while (curr_row_node != NULL) {
	    
	    char *header_value = curr_head_node->label;
	    char *field_value = curr_row_node->label;


	    /* Don't do this for the header row */
	    if(strcmp(header_value, field_value) && strcmp(field_value,DECISION_FOR)) {
                double Pxi = lookup_Pxi_value(Pxi_table_in, header_value, field_value); 
	        /* These can be improved to look at all values of the column where you see ?s in the decisioning table */
		list_node *add_node = create_new_list_node(field_value);

		/* Compute PxiCi/Pxi for each field_value */
		insert_node_at_end(curr_decisioned_list, add_node);


                /* Get the name of the class */
		char *Cname = PxiCi_folder_in->label;

		/* For every table of distinct Cname value in the PxiCi folder  */
		table *curr_table = PxiCi_folder_in->head;
		list_node *curr_node = bayes_values_list->head;
		while (curr_table != NULL) {

		    
                    /* Look up Pxi, Pxi|Ci and PCi values for this value */
                    double PxiCi_Ci = lookup_PxiCi_value(PxiCi_folder_in, header_value, field_value, curr_table->label);
                    //printf("*d curr_table->label=%s, header_value=%s, field_value=%s, Cname=%s, Pxi=%f, PxiCi=%f\n", curr_table->label, header_value, field_value, Cname, Pxi, PxiCi_Ci);


		    /* Calculate the product of all PxiCi/Pxi for a row for both C1 and C2 (can be generalized in a loop if necessary, right now this works only with two (Y/N) Ci classes */


		    if (strcmp(header_value, Cname)){ 
		    //printf("*d before * bayes_Ci=%f, PxiCi_Ci=%f, Pxi=%f\n", bayes_Ci, PxiCi_Ci, Pxi );
                        curr_node->P = curr_node->P * PxiCi_Ci/Pxi;
		    //printf("*d after * bayes_Ci=%f, PxiCi_Ci=%f, Pxi=%f\n", bayes_Ci, PxiCi_Ci, Pxi );

		        curr_node = curr_node->next;
		    }

		    curr_table = curr_table->next;
		}
	    }
            curr_row_node = curr_row_node->next;
            curr_head_node = curr_head_node->next;
        }


        //printf("*d before PCi bayes_values_list=\n"); print_list(bayes_values_list); printf("\n");

	curr_table2 = PxiCi_folder_in->head;
	list_node *curr_bayes_ptr = bayes_values_list->head;
        /* Store multiplicative PCi_Ci  values into a list */
	while (curr_table2 != NULL) {

            double PCi_Ci = lookup_Pxi_value(Pxi_table_in, PxiCi_folder_in->label, curr_table2->label); 
            //printf("*d curr_table2->label=%s, PCi_Ci=%f\n", curr_table2->label, PCi_Ci);
	    curr_bayes_ptr->P = (curr_bayes_ptr->P) * PCi_Ci;
	    curr_table2 = curr_table2->next;
	    curr_bayes_ptr = curr_bayes_ptr->next;
	}
        //printf("*d after Pxi bayes_values_list=\n"); print_list(bayes_values_list); printf("\n");


	char *prediction = "UNKNOWN";
	double bayes_final=0.0;

	/* Find maximum values of bayes_values_list node Ps */
	curr_bayes_ptr = bayes_values_list->head;

        /* Update bayes_Ci values */

	while (curr_bayes_ptr != NULL) {
        //printf("*d before bayes_final=%f\n", bayes_final);
        //printf("*d before curr_bayes_ptr->P=%f\n", curr_bayes_ptr->P);
            if (curr_bayes_ptr->P > bayes_final) {
	        bayes_final = curr_bayes_ptr->P;
                prediction = curr_bayes_ptr->label;    	     
	    }
        //printf("*d after bayes_final=%f\n", bayes_final);
        //printf("*d after curr_bayes_ptr->P=%f\n", curr_bayes_ptr->P);
        //printf("*d after prediction=%s\n", prediction);
	    curr_bayes_ptr = curr_bayes_ptr->next;
	}


        list_node *prediction_node = create_new_list_node(prediction);
        prediction_node->P = bayes_final;
        insert_node_at_end(curr_decisioned_list, prediction_node);

	 
	insert_list_at_end(table_out, curr_decisioned_list);
        curr_list = curr_list->next;
        //printf("*d ------------------------\n\n");
    } /* End row processing while() */
    return table_out;
}

/* --- End of decisioning functions --- */

/* --- Begin file processing functions --- */

/* Tokenize a line to strings and put them into a list consisting of nodes */

void tokenize(list *list_in, char *str) {

    char *pt;
    list_node *node;
    pt = strtok (str,SEP);
    /* Read the next token into a list */
    while (pt != NULL) {
        node = create_new_list_node(pt);
        insert_node_at_end (list_in, node);         /* insert name */
        pt = strtok (NULL, SEP);
    }   
} 

/* Read file line by line and store it into a table consiting dynamic lists e */
void readfile(table *table_in, char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    //table * table_out  = new_table();

    fp = fopen(filename, "r");

    if (fp == NULL)
        exit(EXIT_FAILURE);

    /* Step to the end of the file */
    while ((read = getline(&line, &len, fp)) != -1) {
        list *mylist;
        mylist = new_list();


        line[strlen(line)-1] = '\0';  // Remove the last character of line \n

        /* Get comma separated values into mylist */ 
	tokenize(mylist, line);

        /* insert the list into the table */
        insert_list_at_end(table_in, mylist);
        	
    }

    fclose(fp);
    if (line)
        free(line);

}

/* --- End file processing functions --- */


/* --- Main function --- */
int main (int argc, char *argv[]) {
    /* File name for the training data */
    char *training_file_name=argv[1]; 
    //training_file_name="training.txt";
    /* File name for the data that we have decision/classify */
    char *decisioning_file_name=argv[2]; 
    //decisioning_file_name="decisioning.txt";

    /* --- Begin of reading in the training data --- */ 

    table *training_table = new_table();
    readfile(training_table, training_file_name);

    /* --- End of reading in the training data --- */ 

    /* --- Begin of reading in the decisioning data --- */ 
    table *decisioning_table = new_table();
    /* Read the decisioning table file */
    readfile(decisioning_table, decisioning_file_name);
    add_label_to_table(decisioning_table, "decisioning_table");
    /* End of reading in the training data */ 
    
    /* Determine the class variable name we need to decision for */
    char *C_variable_name = get_class_variable(decisioning_table); 
    //printf("*d main(): C_variable_name=%s\n", C_variable_name);
    

    /* ----------------------------------------------- */

    /* --- Begin model training calculations --- */ 

    /* Cross link nodes in the training table so we can walk it vertically as well*/
    cross_link_nodes(training_table); 

    /* Create, calculate and populate the Pxi table */
    /* Identify unique values of each column, xi */
    /* Count unique values of each column, count(xi) */
    /* Compute probability values of each column's unique values, P(xi) */
    table *Pxi_table = new_table();
    create_Pxi_table(Pxi_table, training_table);
    add_label_to_table(Pxi_table, "Pxi_table");

    /* Create, calculate and populate the Pxi table */
    folder *PxiCi_folder = new_folder();
  
    /* Specify the class variable header value */ 
    /* then for each header's each unique value */
    /* calculate probability values of P(xi|Ci) */
    PxiCi_folder = create_PxiCi_folder(training_table, C_variable_name);
    add_label_to_folder(PxiCi_folder, C_variable_name);
    
    /* --- End of model training calculations --- */ 

    /* ----------------------------------------------- */

    /* --- Begin of decisioning/classification --- */

    /* Begin of decisioning/classification */
    table *decisioned_table = new_table();
    decisioned_table = get_decisioned_table(Pxi_table, PxiCi_folder, decisioning_table);
    add_label_to_table(decisioned_table, "decisioned_table");
    print_table(decisioned_table);

    /* --- End of decisioning/classification ---*/


    /* ----------------------------------------------- */

    /* --- Begin of cleanup --- */
    free_table (training_table);
    free_table (Pxi_table);     
    free_folder (PxiCi_folder);    
    free_table (decisioning_table); 
    free_table (decisioned_table); 
    /* --- End of cleanup --- */

    return 0;
}

