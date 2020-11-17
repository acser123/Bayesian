#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEP ","
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

    /* set next_v pointer NULL */
    node_out->next_v = NULL;         
 
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
    //printf ("*debug*: print_list(): ** Entering **\n" );
if (list_in != NULL ) {
    list_node *mycurrent = list_in->head;
    int i=0;
    while (mycurrent != NULL)
    {      
            i++;
     //       printf ("\n*debug*: print_list(): entering while(), i=%d\n",i );
    //    if (mycurrent == list_in->head)
    //        printf ("%s count=%d P=%f", mycurrent->label, mycurrent->count, mycurrent->P);
    //    else
   //         printf (",%s count=%d P=%f", mycurrent->label, mycurrent->count, mycurrent->P);
        printf("\n");
        print_node(mycurrent);
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
    printf("*d print_table(): table_in->label=%s\n", table_in->label);
   // printf("*d print_table(): *** Entering ***\n");
    int i=0;
if (table_in != NULL) {
    list *current = table_in->head;
    while (current != NULL)
    {
    	print_list (current);
    //    printf("*d print_table: print_list finished\n");  
        current = current->next;
        i++;
    }
}
     //   printf("*d print_table: print_table finished\n");  
}

/* Create a dynamically allocated table */
table *new_table(void) {
   table *table_out = malloc (sizeof *table_out); /* create storage for list */
   if (!table_out) {                         /* validate all allocations */
      perror("malloc-table_out");
      return NULL;
   }
   table_out->head = NULL;
   //table_out->label = ""; 
   table_out->size = 0;
   return table_out;
}

void add_label_to_table(table *table_in, const char *label){
    /* allocate/validate storage for label */

    if (!(table_in->label = malloc (strlen (label) + 1))) {
       perror ("malloc-table_in->label");
       free (table_in);
       return;
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
            mylist_in->next = NULL;
        }
    }
}



/* you are responsible for freeing any memory you allocate */
void free_table (table *table_in)
{ list *current = table_in->head; while (current) {
        list *victim = current;
        current = current->next;
        free_list (victim);
    }
    //free (table_in->label);
    free (table_in);
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


/* you are responsible for freeing any memory you allocate */
void free_folder (folder *folder_in) { 
    table *current = folder_in->head; 
    while (current) {
        table *victim = current;
        current = current->next;
        free_table (victim);
    }
    free (folder_in);
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
       /* terminate the rows' next_v's with NULLs */
       curr_node_ptr->next_v = NULL; 
       curr_node_ptr  = curr_node_ptr->next;
    }
}

/* retrieve_column retrieve the column that has header_in */
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
            //printf("*d retrieve_column(): strcmp header_in=%s\n", header_in);
            list_node *current_node = column_node_ptr;
            while (current_node != NULL) {
                //printf("*d retrieve_column(): strcmp current_node->label=%s\n", current_node->label);
                //print_node(current_node);
                
  
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

    //list_out = list_tmp; 
   //printf("\n\n*d retrieve_column(): about to return with list_tmp="); print_list(list_tmp); printf("\n");
   //printf("\n\n*d retrieve_column(): about to return with list_out="); print_list(list_out); printf("\n");
   //printf("*d retrieve_column(): done\n\n");

    //free_list(list_tmp);
    return list_out; 
}

/* Get unique values with counts and probabilities */
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

        /* Calculate probability for each unique value, subtract one from number of rows since also counted the header row */
        current->P = (current->count) / ((double) numrows - 1); 

        /* Step to calculate P for the next node */
        current = current->next;
    }

    return unique_list_out;
}



/* Return a list (column) of uniques with probabilities and counts where header_name_in=header_criterion_in  and where the value is field_value_in*/

list * get_uniques_with_criterion(table *table_in, const char *examine_header_in, const char *header_name_in, const char *field_value_in) {

    /* We will advance this pointer */   
    list *curr_list = table_in->head;
    list_node *curr_node = curr_list->head;

    int curr_column_count = 0, crit_column_num = 0, examine_column_num = 0;

    while (curr_node != NULL) {
        curr_column_count++;
        if (strcmp(curr_node->label, header_name_in) == 0) {
            crit_column_num = curr_column_count;
        }
        curr_node = curr_node->next;
    }
 
    printf("*d guwd(): crit_column_num=%d\n", crit_column_num);
    list *unique_list_out = new_list();


    /* The first node label is the header value and it is unique for sure so we initialize the unique list */


    /* This is the columns where we want to find uniques, counts and P's for        only those rows where header_name_in=field_value_in */ 
    curr_node = curr_list->head;
    curr_column_count=0;
    while (curr_node != NULL) {
        curr_column_count++;
        if (strcmp(curr_node->label, examine_header_in) == 0) {
            examine_column_num = curr_column_count;
        }
        curr_node = curr_node->next;
    }
    /* Set curr_node to the examine_column_num'th column */
    curr_node = curr_list->head;
    int j=1;
    while (curr_node != NULL && j<examine_column_num ) {
        j++;
        curr_node = curr_node->next;
    }

    list_node *unique_list_node = create_new_list_node(curr_node->label);
    unique_list_node->count++;
    insert_node_at_end(unique_list_out, unique_list_node);
    
    /* Step to the second item on the current node */
    curr_list = curr_list->next;
    curr_node = curr_node->next_v; 

    /* We already looked at the first row */
    int numrows=1; 

    /* Step through nodes of the list_in */
    while (curr_node != NULL) {
        /* Only process the line if header_name_in column field matches the field_value_in */

        /* Step horizontally crit_column_num times to find the header_name_in vaalue */
        list_node *node_ptr = curr_list->head;
        int i=1;
        while (node_ptr != NULL && i<crit_column_num ) {
            i++;
            node_ptr = node_ptr->next;
        }

        if(strcmp(field_value_in, node_ptr->label)==0) { 
            
            numrows++;

            /* Go the beginning of the unique list */
            unique_list_node = unique_list_out->head;
            /* Clear flag */
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
             } /* while(unique_list_node != NULL) */

             /* If we have not found the current on the unique list, then add the current node to the unique list */
             if (found==0)  { 
                 add_node->count++;
                 insert_node_at_end(unique_list_out, add_node);
                 found=1;
             } 
          } /* strcmp() */
          /* Step to examine the next node */
          curr_node = curr_node->next_v; 
          curr_list = curr_list->next;
    }


    /* Calculate and populate probabilities */
    list_node *current = unique_list_out->head; 

    /*  Step through unique_list_out */
    while (current != NULL) {

        /* Calculate probability for each unique value, subtract one from number of rows since also counted the header row */
        current->P = (current->count) / ((double) numrows - 1); 

        /* Step to calculate P for the next node */
        current = current->next;
    }

    return unique_list_out;
}

/* --- End of column functions --- */

/* --- Begin of global training functions --- */

/* Create table of Pxi values */
void create_Pxi_table(table *table_out, table *table_in){
    //table *table_out; 
    //table *table_out2 = new_table();
    //table *table_out = new_table();
    //table_out->head=NULL; 
    //table_out2->head=NULL; 

    list *curr_list = table_in->head;
    
    list_node *curr_node = curr_list->head;
    


    //list *curr_column;
    
    while (curr_node != NULL) {

       // printf("*d create_Pxi_table(): before retrieve_column with curr_node->label=%s\n", curr_node->label);
        //printf("*d create_Pxi_table(): before retrieve_column, result curr_column=\n"); print_list(curr_column); printf("\n");
    list *curr_column =  new_list();
        curr_column = retrieve_column(table_in, curr_node->label);
        //printf("*d create_Pxi_table(): after retrieve_column, result curr_column=\n"); print_list(curr_column); printf("Done print_list\n");
        list *counts_list = new_list();
        counts_list = get_uniques(curr_column);
        //printf("*d create_Pxi_table(): counts_list=\n"); print_list(counts_list); printf("\n");
        //printf("*d create_Pxi_table(): before insert_list_at_end, table_out=\n"); print_table(table_out); printf("\n");
        //insert_list_at_end(table_out, curr_column);        
        insert_list_at_end(table_out, counts_list);        
        curr_node = curr_node->next;
        //free_list(counts_list);     
        //free_list(curr_column);     
    }

    //return table_out2;
    
}

/*  Create folder of tables of PxiCi values, i.e. probabilities of Pxi when Ci is of a value for each value of C */
folder * create_PxiCi_folder(table *training_table_in, char *Cname_in) {
    folder *folder_out = new_folder();


/* 
  Find out unique values for Cname_in
  For each unique value of Ci of Cname_in 
    Generate a table of Ps for each Xi value for each X
    Add table to the folder
  Return the folder 
*/

    //list *Ci_list = new_list();
    
    list *curr_list = training_table_in->head;
    list_node *curr_node = curr_list->head;
    list *Ci_list = new_list();

    while (curr_node != NULL) {
        /* When we find Cname in in the headers, perform the whole sequence of creating uniques and then creating a table of Ps */
        if(!strcmp(curr_node->label, Cname_in)) {
            //printf("*d curr_node->label=%s\n", curr_node->label);

            /* Get the column for Cname_in */
            list *C_list = new_list();
            C_list = retrieve_column(training_table_in, Cname_in);

            /* Get unique values on C_list */
            //list *Ci_list = new_list();
            Ci_list = get_uniques(C_list);

        }
        curr_node = curr_node->next;
    }

    
//printf("*d Ci_list: "); print_list(Ci_list);

    /* For each non-header value on the Ci_list (e.g. "Y", or "N"), generate Ps for all headers' all values and store them into individual tables which you then link into a folder */

    /* Skip the header */
   
    list_node *curr2_node = (Ci_list->head)->next;
//    list_node *curr2_node = (Ci_list->head);

    while (curr2_node != NULL) {
        /* Table to hold the values for all Ci */
        table *Ci_table =  new_table();   
        
        add_label_to_table(Ci_table, curr2_node->label);

        list *curr2_list = training_table_in->head;
        list_node *header_ptr_node = curr2_list->head;

 //       list *counts_list = new_list();

        /* Get all headers */       
        while (header_ptr_node != NULL) {
             

            list *counts_list = new_list();

            printf("*d get_uniques_with_criterion: header_ptr_node->label=%s, Cname_in=%s, curr2_node->label=%s\n", header_ptr_node->label, Cname_in, curr2_node->label);
            counts_list = get_uniques_with_criterion(training_table_in, header_ptr_node->label, Cname_in, curr2_node->label);

            printf("*d before insert_list_at_end Ci_table="); print_table(Ci_table);             
            printf("\n*d before insert_list_at_end Ci_table printed\n");             
            printf("*d counts_list="); print_list(counts_list);             
           
            insert_list_at_end(Ci_table, counts_list);
            printf("*d insert_list_at_end done, Ci_table="); //print_table(Ci_table);             
            printf("\n*d insert_list_at_end done, Ci_table printed\n");             
            printf("*d Done with header=%s\n\n", header_ptr_node->label);
            //free_list(counts_list);
            header_ptr_node = header_ptr_node->next; 
        }

        insert_table_at_end(folder_out, Ci_table);
        printf("*d Done with Ci=%s\n\n", curr2_node->label);
        curr2_node = curr2_node->next;
    } 
      
  
///////////////////////
    //list *curr_list = training_table_in->head;
    //list *counts_list = new_list(); 
    //list_node *curr_node = curr_list->head;
    //table *counts_table = new_table(); 
    //while (curr_node != NULL) {
        //counts_list = get_uniques_with_criterion(table_in, curr_node->label, Cname_in, "Y");
     //   insert_table_at_end(folder_out, counts_table);        
        
      //  curr_node = curr_node->next;
   // }

    return folder_out;

}


/* --- End of global training functions --- */

/* --- Begin file processing functions --- */

/* Tokenize a line to strings and put them into a list consisting of nodes */

void tokenize(list *list_in, char *str) {

    //list *list_out = new_list();
    char *pt;
    list_node *node;
    pt = strtok (str,SEP);
    while (pt != NULL) {
        node = create_new_list_node(pt);
        insert_node_at_end (list_in, node);         /* insert name */
        //printf("*d tokenize(): list_in="); print_list(list_in); printf("\n");
        pt = strtok (NULL, SEP);
    }   
   // return list_out; 
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

   //return table_in;
}

/* --- End file processing functions --- */

int main (int argc, char *argv[]) {
    char *fn=argv[1]; 
    fn="training.txt";

    /* --- Begin of reading in the training data --- */ 
    table *training_table = new_table();

    /* Table to hold the P(xi|Ci) probabilities */
    //table *table_PxiCi = new_table();
    
    readfile(training_table, fn);

    //printf("*d main(): training_table="); print_table(training_table);

    /* End of reading in the training data */ 
 
    /* --- Begin training --- */ 

    /* Cross link nodes in the training table so we can walk it vertically as well*/
    cross_link_nodes(training_table); 
    //printf("\n\n\n*******d main(): training_table="); print_table(training_table);
    //printf("\n\n\n*******d main(): training_table print_table done.\n\n");


    /* Create, calculate and populate the Pxi table */
    table *Pxi_table = new_table();
    create_Pxi_table(Pxi_table, training_table);
    add_label_to_table(Pxi_table, "Pxi_table");


    list *counts_list = new_list();

    counts_list = get_uniques_with_criterion(training_table, "Outlook", "Play", "Y");
    print_list(counts_list);


    //printf("print_table(Pxi_table)=\n"); print_table(Pxi_table); printf("\n");
 //   printf("*d main(): Pxi_table="); print_table(Pxi_table);
  //  printf("\n");

    folder *PxiCi_folder = new_folder();
   
    PxiCi_folder = create_PxiCi_folder(training_table, "Play");
    printf("*d main(): PxiCi_folder="); print_folder(PxiCi_folder);
    
  //  list *testcolumn = new_list();
 //   testcolumn = retrieve_column(training_table, "Outlook");
//    printf("*d main(): testcolumnr=\n"); print_list(testcolumn); printf("\n");
    
    //print_table(table_PxiCi);


 

    /* Read and analyze all training table values */
   // table *parameter_table = new_table();
   // list *Ci = new_list();
   // insert_list_at_end(parameter_table, Ci);


    /* Identify unique values of the last column, C  or Class */
    /* Count unique values of the last column, count(Ci) */
    /* Compute probability values of each of the last column's unique values, P(Ci) */

    /* Identify unique values of each column, xi */
    /* Count unique values of each column, count(xi) */
    /* Compute probability values of each column's unique values, P(xi) */

    /* Calculate probability values of P(xi|Ci) */

    /* End of training */ 
    //free_list(testcolumn);
    free_table (training_table);     /* don't forget to free memory you allocate */
    free_table (Pxi_table);     /* don't forget to free memory you allocate */
    printf("*d main() end.\n");
    return 0;
}

