#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// From https://stackoverflow.com/questions/20339761/list-of-lists-c/20340740

// ------------ data structures ------------
typedef union s_datatype {
    int t_int;
    char* t_char;
    double t_double;
    bool t_bool;
} t_datatype;

typedef struct s_token {
    int y;
    int type;
    t_datatype value;
    struct s_token *next;
} t_token;

typedef struct s_line {
    int x;
    int depth;
    int type;
    int number_of_tokens;
    t_token*head;
    struct s_line *next;
} t_line;

typedef struct s_tokenized_code {
    int number_of_lines;
    t_line*head;
} t_tokenized_code;

// ------------ functions ------------

t_token * init_token (int y, int type, t_datatype value) {
    t_token *token = malloc(sizeof(struct s_token));
    if (token == NULL) {
        return NULL;
    }
    token->y = y;
    token->type = type;
    token->value = value;
    token->next = NULL;
    return token;
}

t_line * init_line (int x, int depth, int type) {
    t_line *line = malloc(sizeof(struct s_line));
    if (line == NULL) {
        return NULL;
    }
    line->x = x;
    line->depth = depth;
    line->type = type;
    line->number_of_tokens = 0;
    line->head = NULL;
    line->next = NULL;
    return line;
}

t_tokenized_code * init_code (void) {
    t_tokenized_code *code = malloc(sizeof(struct s_tokenized_code));
    if (code == NULL) {
        return NULL;
    }
    code->number_of_lines = 0;
    code->head = NULL;
    return code;
}

void insert_token (t_line *line, t_token *token) {
    if (token != NULL) {
        if (line->head == NULL){
            line->head = token;
            line->number_of_tokens += 1;
            return;
        }
        t_token*tmp = line->head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = token;
        line->number_of_tokens += 1;
    }
}

void insert_line (t_tokenized_code *code, t_line *line) {
    if (line != NULL) {
        if (code->head == NULL) {
            code->head = line;
            code->number_of_lines += 1;
            return;
        }
        t_line*tmp = code->head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = line;
        code->number_of_lines += 1;
    }
}

void free_code (t_tokenized_code *source) {
    if (source == NULL) {
        return;
    }
    t_line*line;
    t_token*token;
    while ((line = source->head) != NULL) {
        while ((token = line->head) != NULL) {
            line->head = token->next;
            free(token);
        }
        source->head = line->next;
        free(line);
    }
    free(source);
}


// ------------ main ------------
int main (void) {
    t_tokenized_code *source = init_code();
    t_line *line = init_line(0,0,3);
    if (line == NULL) {
        return 1;
    }
    insert_line(source, line);

    t_datatype tt;
    tt.t_char = "first";
    t_token *token = init_token(0,6, tt);
    if (token == NULL) {
        return 1;
    }
    insert_token(line, token);

    tt.t_char = "second";
    token = init_token(1,6, tt);
    insert_token(line, token);

    line = init_line(1,0,3);
    insert_line(source, line);

    tt.t_char = "third";
    token = init_token(0,6, tt);
    insert_token(line, token);

    tt.t_char = "fourth";
    token = init_token(1,6, tt);
    insert_token(line, token);

    tt.t_char = "fifth";
    token = init_token(2,6, tt);
    insert_token(line, token);

    line = source->head;
    while (line != NULL) {
        printf("****\n");
        token = line->head;
        while (token != NULL) {
            printf("%s\n", token->value.t_char);
            token = token->next;
        }
        line = line->next;
    }
    free_code (source);
    return 0;
}


