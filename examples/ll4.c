#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 512    /* if you need a constant, #define one (or more) */

typedef struct lnode {  /* don't typedef pointers -- it will confuse you */
    char *data;
    struct lnode *next;
} lnode;

typedef struct list {   /* you pass this list to insert_at_front */
    lnode *head;
    size_t size;        /* you can track any list stats you like */
} list;

/* create a dynamically allocated list struct */
list *new_list (void) 
{
    list *temp = malloc (sizeof *temp);     /* create storage for list  */
    if (!temp) {                            /* validate ALL allocations */
        perror ("malloc-new_list");
        return NULL;
    }
    temp->head = NULL;  /* initialize head NULL */
    temp->size = 0;

    return temp;    /* return pointer to new list */
}

/* create new dynamically allocated node, initialize all values */
lnode *create_new_node (const char *data)
{
    lnode *new_node = NULL;

    if (!data) {    /* validate data not NULL */
        fputs ("error: data is NULL in create_new_node.\n", stderr);
        return NULL;
    }

    new_node = malloc (sizeof *new_node);   /* allocate/validate node */
    if (!new_node) {
        perror ("malloc-new_node");
        return NULL;
    }
    /* allocate/validate storage for data */
    if (!(new_node->data = malloc (strlen (data) + 1))) {
        perror ("malloc-new_node->data");
        free (new_node);
        return NULL;
    }
    strcpy (new_node->data, data);  /* copy data to new_node->data */
    new_node->next = NULL;          /* set next pointer NULL */

    return new_node;    /* return pointer to new_node */
}

/* insert new node at front, returning pointer to head
 * to guage success/failure of addition.
 */
lnode *insert_at_front (list *mylist, const char *data)
{ 
    lnode *new_node = create_new_node(data);

    if (!new_node) 
        return NULL;

    new_node->next = mylist->head;
    mylist->head = new_node;
    mylist->size++;

    return mylist->head;
}

/* print_list - tweaked for formatted output */
void print_list (list *self)
{
    lnode *current = self->head;

    while (current != NULL)
    {
        if (current == self->head)
            printf (" %s", current->data);
        else
            printf (", %s", current->data);
        current = current->next;
    }

    putchar ('\n');
}

/* you are responsible for freeing any memory you allocate */
void free_list (list *mylist)
{
    lnode *current = mylist->head;
    while (current) {
        lnode *victim = current;
        current = current->next;
        free (victim->data);
        free (victim);
    }
    free (mylist);
}

/* you are responsible for the state of stdin when doing user input */
void empty_stdin (void)
{
    int c = getchar();

    while (c != '\n' && c != EOF)
        c = getchar();
}

int main (void) {

    char i = 'y';
    char name[MAXN] = "";   /* fixed size buf for reading name input */

    list *mylist = new_list();

    while (i == 'y' || i == '\n') {         /* 'y' or (default '\n') */
        fputs ("\nenter name: ", stdout);
        if (scanf ("%511[^\n]", name) != 1) {   /* ALWAYS CHECK RETURN! */
            fputs ("error: invalid input or user canceled.", stderr);
            return 1;
        }
        empty_stdin();      /* empty any chars that remain in stdin */

        insert_at_front (mylist, name);         /* insert name */
        fputs ("continue (y)/n: ", stdout);     /* prompt to continue */
        scanf ("%c", &i);   /* read answer (or '\n' from pressing Enter) */
    }

    printf ("\nfinal list (%zu nodes):", mylist->size);
    print_list (mylist);

    free_list (mylist);     /* don't forget to free memory you allocate */

    return 0;
}
