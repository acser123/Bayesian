#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEP ","
#define MAXN 512    /* if you need a constant, #define one (or more) */

/* Structue for a node in the list that stores the entire file we will read in */
typedef struct list_node {
   char *label;
   struct list_node *next;
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
//   struct list *next;
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

    new_node = malloc (sizeof *new_node);   /* allocate/validate node */
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
list_node *insert_node_at_front (list *mylist_in, const char *data)
{
    list_node *new_node = create_new_list_node(data);

    //printf("*debug* insert_node_at_front(): data=%s\n", data);
    if (!new_node)
        return NULL;

    new_node->next = mylist_in->head;
    mylist_in->head = new_node;
    mylist_in->size++;

    return mylist_in->head;
}

/* print_list - tweaked for formatted output */
void print_list (list *self)
{
    list_node *current = self->head;

    while (current != NULL)
    {
        if (current == self->head)
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
//list *create_new_list (list *list_node_h)
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
    if (!(new_list->head = malloc (sizeof (list_node_h) + 1))) {
        perror ("malloc-new_list->list");
        free (new_list);
        return NULL;
    }
    new_list->head = list_node_h;  /* copy data to new_node->data */
    new_list->next = NULL;          /* set next pointer NULL */

    return new_list;    /* return pointer to new_node */
}

/* insert new list at front returning pointer to head
 * to guage success/failure of addition.
 */
list *insert_list_at_front (table *mytable_in, list *mylist_in)
{
    list *new_list = create_new_list(mylist_in);

    if (!new_list) {
       return NULL;
    }


    new_list->next = mytable_in->head;
    mytable_in->head = new_list;
    mytable_in->size++;

    return mytable_in->head;
}

/* get_list from table */ 
/*
list get_list (table *self) {
   return (self->head);
}
*/
/* print_list - tweaked for formatted output */
void print_table (table *self)
{
    list *current = self->head;
    int i;
    while (current != NULL)
    {
	i++;

        if (current == self->head) {
        printf("*debug* print_list(): i=%d\n", i);
    	    print_list (current);
        }
        else {
        printf("*debug* print_list(): i=%d\n", i);
    	    print_list (current);
        }

        current = current->next;
   
    }

    putchar ('\n');
}


/* you are responsible for freeing any memory you allocate */
void free_table (table *mytable)
{
    list *current = mytable->head;
    while (current) {
        list *victim = current;
        current = current->next;
//        free (victim->list);
        free (victim);
    }
    free (mytable);
}
/* --- End of table functions --- */

/* you are responsible for the state of stdin when doing user input */
void empty_stdin (void)
{
    int c = getchar();

    while (c != '\n' && c != EOF)
        c = getchar();
}


char * tokenize(list *mylist, char *str) {
   //char str[] ="1,2,3,4,5";
   char *pt;
   pt = strtok (str,SEP);
   while (pt != NULL) {
    printf("*debug* tokenize(): insert_node_at_front() called with %s\n", pt);
    insert_node_at_front (mylist, pt);         /* insert name */
     pt = strtok (NULL, SEP);
   }   
   printf("*debug* tokenize(): print_list():"); print_list (mylist);
   return pt;
} 

int readfile(table *mytable, char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

 //   list *mylist;
 //   mylist = new_list();
 //   insert_list_at_front(mytable, mylist);

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        list *mylist;
        mylist = new_list();
        insert_list_at_front(mytable, mylist);

        printf("*debug* readfile() before tokenization: print_list():"); print_list (mylist);

        printf("*debug* readfile(): Retrieved line of length %zu:\n", read);
        printf("%s", line);
        line[strlen(line)-1] = '\0';  // Remove the last character of line \n


        
	tokenize(mytable->head, line);
        printf("*debug* readfile(): line done\n\n");
        	
    }
        printf("*debug* readfile(): tokenize() done\n\n\n");

    fclose(fp);
    if (line)
        free(line);
//    exit(EXIT_SUCCESS);
    return 0;
}


int main (int argc, char *argv[]) {
    char *fn=argv[1]; 
    fn="training.txt";

//list *mylist;
table *mytable;

    mytable = new_table();
    
    //list input_list =  get_list(mytable);
    //readfile(mytable->head, fn);
    readfile(mytable, fn);

//    printf ("\n\nfinal list (%zu nodes):", mytable->head->size);
//    print_list (mytable->head);
    print_table(mytable);

    //free_list (mytable->head);     /* don't forget to free memory you allocate */
    //free_table (mytable);     /* don't forget to free memory you allocate */

    return 0;
}

