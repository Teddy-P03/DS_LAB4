#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 128

struct node {
    void * element ;
    struct node * left ; 
    struct node * right ;
} ;

typedef struct node node_t ;
typedef struct node linkedlist_t ;

linkedlist_t * 
linkedlist_alloc (int unit)  ;

void
linkedlist_free (linkedlist_t * l) ;

int 
linkedlist_length (linkedlist_t * l) ;

void
linkedlist_insert_first (linkedlist_t * l, void * e) ;

void
linkedlist_insert_last (linkedlist_t * l, void * e) ;

int
linkedlist_remove (linkedlist_t * l, node_t * n) ;

int
linkedlist_remove_first (linkedlist_t * l, void * e) ;

int
linkedlist_remove_last (linkedlist_t * l, void * e) ;

int
linkedlist_get (linkedlist_t * l, int pos, void * e) ;



linkedlist_t * 
linkedlist_alloc (int unit) 
{
    linkedlist_t * l = (linkedlist_t *) malloc(sizeof(linkedlist_t)) ;
    l->left = l  ;  
    l->right = l ;
    l->element = malloc(sizeof(int)) ;
    int * u = (int *) l->element ;   // <-- int * u = l->element ; 이렇게 하는 것과 다른 점이 뭔가?
    *u = unit ;

    return l ;
}

void
linkedlist_free (linkedlist_t * l)  // <-- 원리가 이해가 안됨
{
    node_t * curr ;
    node_t * next ;

    curr = l->right ;
    while (curr != l) {
        next = curr->right ;
        free(curr->element) ;
        free(curr) ;
        curr = next ;
    }
    free(l->element) ;
    free(l) ;
}

int 
linkedlist_length (linkedlist_t * l)
{
    int len = 0 ;

    node_t * curr = l->right ;
    while (curr != l) {
        len += 1 ;
        curr = curr->right ; 
    }
    return len ; 
}

void 
linkedlist_insert (node_t * left, void * e, int unit)
{
    node_t * _new = (node_t *) malloc(sizeof(node_t)) ;
    _new->element = malloc(unit) ;
    memcpy(_new->element, e, unit) ;

    node_t * right = left->right ;

    _new->left = left ;
    _new->right = right ;

    left->right = _new ;
    right->left = _new ;
}

void
linkedlist_insert_first (linkedlist_t * l, void * e)
{
    linkedlist_insert(l, e, *((int *)(l->element))) ;
}

void
linkedlist_insert_last (linkedlist_t * l, void * e)
{
    linkedlist_insert(l->left, e, *((int *)(l->element))) ;
}

int
linkedlist_remove (linkedlist_t * l, node_t * n)
{
    if (l->left == l)
        return 1 ;

    n->left->right = n->right ;
    n->right->left = n->left ;

    free(n->element) ;
    free(n) ;
    return 0 ;
}

int 
linkedlist_remove_first (linkedlist_t * l, void * e)
{
    if (l->right == l)
        return 1 ;

    memcpy(e, l->right->element, *((int *)(l->element))) ;
    linkedlist_remove(l, l->right) ;
    return 0 ;
}

int
linkedlist_remove_last (linkedlist_t * l, void * e)
{
    if (l->left == l)
        return 1 ;

    memcpy(e, l->left->element, *((int *)l->element)) ;
    linkedlist_remove(l, l->left) ;
    return 0 ;
}

int 
linkedlist_get (linkedlist_t * l, int pos, void * e)
{
    if (pos < 0)
        return 1 ;

    int unit = *((int *) l->element) ;
    int i = 0 ;
    node_t * curr = l->right ;
    while (i < pos && curr != l) {
        curr = curr->right ;
        i += 1 ;
    }
    if (i != pos)
        return 1 ;

    memcpy(e, curr->element, unit) ;
    return 0 ;
}







typedef enum { no, yes } bool ;

typedef struct {
    char string[MAX];
    int label;
} struct_t ;


int 
check_label(linkedlist_t * next, int curr_label) {
    struct_t temp;
    linkedlist_get(next, 0, &temp);
    if(temp.label != curr_label)
        return 1;
    else 
        return 0;
}


void
make_curr_word(linkedlist_t * curr_word, linkedlist_t * next) {
    struct_t temp;
    linkedlist_remove_first(next, &temp) ;
    for(int i = 0; i < strlen(temp.string); i++) {                               
        linkedlist_insert_last (curr_word, &(temp.string[i])) ;
    }
}

int
check_overlap(linkedlist_t * next, struct_t q) {

    for(int i= 0; i < linkedlist_length(next); i++) {
        struct_t check;
        linkedlist_get(next, i, &check);
        int a = check.label;
        int b = q.label;
        if( !(strcmp(check.string, q.string)) && a == b)
            return 0;
    }
    return 1;
}

void
make_next(linkedlist_t * curr_word, linkedlist_t * next, int curr_label) {
    char st[MAX];
    struct_t q;
    if(linkedlist_length(curr_word) == 0){ return;}        
    else {
        int i = 0 ;                                 
        while(i < linkedlist_length(curr_word)) {           
            linkedlist_get(curr_word, i, &(st[i]));
            i++;
        }
        st[i] = 0x0 ;
        strcpy(q.string, st);
        q.label = curr_label+1;
        q.string[i] = 0x0 ;

        if( check_overlap(next, q) )
            linkedlist_insert_last(next, &q);

    }
}




int main() {

    char RE[MAX];
    scanf("%s", &RE);

    char words[5][MAX];
    for(int i=0; i < 5; i++) {
        scanf("%s", &(words[i]));
    }

    
    int i = 0 ;
    while(i < 5) {                                                              
        
        linkedlist_t * next ;
        next = linkedlist_alloc (sizeof(struct_t));

        struct_t init;
        init.label= 0;
        int curr_label = 0;

        for(int j = 0; words[i][j] != 0x0; j++) {
            init.string[j] = words[i][j];  
            init.string[j+1] = 0x0;
        }                                           

        linkedlist_insert_first(next, &init); 

        bool error = no;
        bool success;
        for(int j = 0; RE[j] != 0x0; j++) {
            linkedlist_t * curr_word;
            curr_word = linkedlist_alloc (sizeof(char));

            success = no;


            if(islower( RE[j] )) {
                error = yes;
                do {
                    if(check_label(next, curr_label))
                        break;

                    linkedlist_t * curr_word;
                    curr_word = linkedlist_alloc (sizeof(char)); 

                    make_curr_word(curr_word, next);

                    char c;
                    linkedlist_get(curr_word, 0, &c);
                    if( RE[j] == c ){ 
                        char trash;
                        linkedlist_remove_first (curr_word, &trash);
                        if(linkedlist_length(curr_word) == 0)
                            success = yes;
                        make_next(curr_word, next, curr_label);
                        error = no;
                    }
                
                    linkedlist_free (curr_word) ;

                } while( 0 < linkedlist_length(next) ); 
                curr_label += 1;
            }
            

            if(RE[j] == '*') {
                do {
                    if(check_label(next, curr_label))
                        break;

                    linkedlist_t * curr_word;
                    curr_word = linkedlist_alloc (sizeof(char)); 

                    make_curr_word(curr_word, next);
                    bool init = yes ;

                    while( 0 < linkedlist_length(curr_word)) {

                        if( init == yes ) {
                            make_next(curr_word, next, curr_label); 
                        }
                        init = no ;

                        char trash;
                        linkedlist_remove_first (curr_word, &trash);
                        if(linkedlist_length(curr_word) == 0)
                            success = yes;
                        make_next(curr_word, next, curr_label);
                    }
                
                    linkedlist_free (curr_word) ;
                } while( 0 < linkedlist_length(next) ); 
                curr_label += 1;
            }

            if(RE[j] == '?') {
                error = yes;
                do {
                    if(check_label(next, curr_label))
                        break;

                    linkedlist_t * curr_word;
                    curr_word = linkedlist_alloc (sizeof(char)); 

                    make_curr_word(curr_word, next);

                    if( 0 < linkedlist_length(curr_word) ) {
                        char trash;
                        linkedlist_remove_first (curr_word, &trash);
                        if(linkedlist_length(curr_word) == 0)
                            success = yes;
                        make_next(curr_word, next, curr_label);
                        error = no;                     
                    }
                    linkedlist_free (curr_word) ;

                } while( 0 < linkedlist_length(next) );
                curr_label += 1;
            }

            if(RE[j] == '!') {
                do {
                    if(check_label(next, curr_label))
                        break;

                    linkedlist_t * curr_word;
                    curr_word = linkedlist_alloc (sizeof(char)); 

                    make_curr_word(curr_word, next);

                    make_next(curr_word, next, curr_label); 

                    if(linkedlist_length(curr_word) != 0) {
                        char trash;
                        linkedlist_remove_first (curr_word, &trash);
                        if(linkedlist_length(curr_word) == 0)
                            success = yes;
                        make_next(curr_word, next, curr_label);     
                    }               
                    
                    linkedlist_free (curr_word) ;
                } while( 0 < linkedlist_length(next) ); 
                curr_label += 1;
            }



            if(RE[j] == '[') {
                char option[MAX];
                j++;
                for(int i = 0 ; RE[j] != ']'; i++) {
                    option[i] = RE[j];
                    option[i+1] = 0x0;
                    j++;
                }

                bool init = yes ;
                error = yes;
                do {
                    if(check_label(next, curr_label))
                        break;

                    linkedlist_t * curr_word;
                    curr_word = linkedlist_alloc (sizeof(char)); 

                    make_curr_word(curr_word, next);

                    char c;
                    linkedlist_get(curr_word, 0, &c);
                    for(int i = 0 ; i < strlen(option); i++) {
                        if( ( option[i] == c ) && ( init = yes ) ){ 
                            char trash;
                            linkedlist_remove_first (curr_word, &trash);
                            if(linkedlist_length(curr_word) == 0) {
                                success = yes;
                            }           
                            make_next(curr_word, next, curr_label);
                            init = no;
                            error = no;
                            break;
                        }
                    }

                    linkedlist_free (curr_word) ;
                } while( 0 < linkedlist_length(next) ); 
                curr_label += 1;
            }
        }

        if( ( (linkedlist_length(next) == 0) || (success == yes) ) && error == no)
            printf("true\n") ;
        else
            printf("false\n") ;

        linkedlist_free(next) ;
        i++;
    }
    

    return 0;
}
