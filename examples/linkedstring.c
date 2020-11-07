#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LARGE 10

struct node *add_to_list(struct node *list, char *n);
struct node{
    char value[LARGE+1];
    struct node *next;
};

struct node *first = NULL;
void print(void);

int main(void) {
    
    char job[LARGE],*p;
    
    printf("Please enter printing jobs\n");
    scanf ("%s", job);
    p=&job[0];
    
    first = add_to_list(first, p);
    print();
    return 0;
}




struct node *add_to_list(struct node *list, char *n)
{
    struct node *new_node;
    new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        printf("Error: malloc failed in add_to_list\n");
        exit(EXIT_FAILURE);
    }
//    new_node->value = *n;
strcpy(new_node->value, n);

    new_node->next = list;
    return new_node;
}

void print(void){
    struct node *new_node;
    for(new_node=first;new_node!= NULL; new_node=new_node->next){
        printf("%s\n",new_node->value);
    }
}
