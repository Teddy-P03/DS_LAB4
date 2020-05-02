#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
	void * element ;
	struct node * next ; 
} ;

struct linkedlist {
	int unit ;
	struct node * last ;
} ;


typedef struct node node_t ;
typedef struct linkedlist linkedlist_t ;

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

typedef struct{
    char str[128];
    int numofp;
} token;

int main(){
    char pattern[128];
    char string[5][128];
    scanf("%s", pattern);
    for(int i=0;i<5;i++){
        scanf("%s", string[i]);
    }
    linkedlist_t * p_queue ;
    linkedlist_t * s_queue ;
    p_queue = linkedlist_alloc(sizeof(char)) ;
    s_queue = linkedlist_alloc(sizeof(token));
    for(int i=0;i<5;i++){
        for(int j=0;j<strlen(pattern);j++){
        	linkedlist_insert_last(p_queue, &pattern[j]) ;
        }
        token first;
        strcpy(first.str,string[i]);		
        first.numofp = 0;      
        linkedlist_insert_last(s_queue, &first);
		//int false =0;
        while(linkedlist_length(p_queue) != 0){
            char p;
            linkedlist_remove_first(p_queue, &p);
				if(p=='*'){
				int size = linkedlist_length(s_queue);
				for(int k=0;k<size;k++){
                    token check;
                    linkedlist_remove_first(s_queue,&check);
					if(strlen(check.str) == 0) {
						linkedlist_insert_last(s_queue, &check);
					}
					else{
						int max = strlen(check.str);	
						int min = 0;
						while(min <= max){
							token next;
							strcpy(next.str,check.str);
							for(int l=0;l<max-min;l++){
								next.str[l] = next.str[l+min];
							}
							int erase =1;
							while(erase <= min){
								next.str[strlen(check.str)-erase] = '\0';
								erase += 1;
							}   
							linkedlist_insert_last(s_queue, &next);
							min += 1;
						}
					}
                }
			}
			else if(p=='!'){
			int size = linkedlist_length(s_queue);
				for(int k=0;k<size;k++){
                    token check;
                    linkedlist_remove_first(s_queue,&check);
					linkedlist_insert_last(s_queue, &check);
					if(strlen(check.str) != 0){
						token next;
						strcpy(next.str,check.str);
						for(int l=0;l<strlen(check.str)-1;l++){
                            next.str[l] = next.str[l+1];
                        }
                    	next.str[strlen(check.str)-1] = '\0';    
                        linkedlist_insert_last(s_queue, &next);	
					}
                }
			}
		    else if(p=='?'){
				int size = linkedlist_length(s_queue);
				for(int k=0;k<size;k++){
                    token check;
                    linkedlist_remove_first(s_queue,&check);
					if(strlen(check.str) == 0){
					}
					else{
						token next;
						strcpy(next.str,check.str);
						for(int l=0;l<strlen(check.str)-1;l++){
                            next.str[l] = next.str[l+1];
                        }
                        next.str[strlen(check.str)-1] = '\0';    
                        linkedlist_insert_last(s_queue, &next);		
					}
                }
			}
			else if(p=='['){
				linkedlist_t * random_queue;
				random_queue = linkedlist_alloc(sizeof(token));
				int size = linkedlist_length(s_queue);
				for(int k=0;k<size;k++){
					token check;
					linkedlist_remove_first(s_queue,&check);
					linkedlist_insert_last(random_queue, &check);
				}
				while(1){
					linkedlist_remove_first(p_queue, &p);
					if(p == ']') break;
					int size1 = linkedlist_length(random_queue);
						for(int k=0;k<size1;k++){
						token check;
						linkedlist_remove_first(random_queue,&check);
						if(check.str[0] == p){
							token next;
							strcpy(next.str,check.str);
							for(int l=0;l<strlen(check.str)-1;l++){
								next.str[l] = next.str[l+1];
							}
							next.str[strlen(check.str)-1] = '\0';   
							linkedlist_insert_last(s_queue, &next);
						}
						else linkedlist_insert_last(random_queue, &check);
					}
				}
				linkedlist_free(random_queue);
			}
			else{
                int size = linkedlist_length(s_queue);
				for(int k=0;k<size;k++){
                    token check;
                    linkedlist_remove_first(s_queue,&check);
                    if(check.str[0] == p){
                        token next;
						strcpy(next.str,check.str);
						for(int l=0;l<strlen(check.str)-1;l++){
							next.str[l] = next.str[l+1];
						}
						next.str[strlen(check.str)-1] = '\0';   
						linkedlist_insert_last(s_queue, &next);
                    }
                }
			}
			/*if(linkedlist_length(s_queue) == 0){
				false +=1;
                break;
            }*/
        }
        token fin;
		int true=0;
		while(linkedlist_length(s_queue) != 0){
			linkedlist_remove_first(s_queue,&fin);
			if(strlen(fin.str) == 0){
				true += 1;
			}
		}
        //if(false != 0) printf("false\n");
		//else 
		if(true != 0){
            printf("true\n");
        }
        else printf("false\n");
    }
	linkedlist_free(p_queue);
	linkedlist_free(s_queue);
    return 0;
}

linkedlist_t * 
linkedlist_alloc (int unit) 
{
	linkedlist_t * l = (linkedlist_t *) malloc(sizeof(linkedlist_t)) ;
	l->unit = unit ;
	l->last = 0x0 ;	
	return l ;
}

void
linkedlist_free (linkedlist_t * l)
{
	node_t * curr ;
	node_t * next ;

	if (l->last) {
		curr = l->last->next ; 
		do {
			node_t * next = curr->next ;
			free(curr->element) ;
			free(curr) ;
			curr = next ;
		} while (curr != l->last) ;
	}
	free(l) ;
}

int 
linkedlist_length (linkedlist_t * l)
{
	int len = 0 ;
	if (l->last) {
		node_t * curr = l->last ;
		do {
			len += 1 ;
			curr = curr->next ;
		} while (curr != l->last) ;
	}
	return len ; 
}

void
linkedlist_insert_first (linkedlist_t * l, void * e)
{
	node_t * n = (node_t *) malloc(sizeof(node_t)) ;
	n->element = malloc(l->unit) ;
	memcpy(n->element, e, l->unit) ;

	if (l->last) {
		node_t * first ;
		first = l->last->next ;
		n->next = first ;
		l->last->next = n ;
	}
	else {
		l->last = n ;
		l->last->next = n ;
	}
}

void
linkedlist_insert_last (linkedlist_t * l, void * e)
{
	node_t * n = (node_t *) malloc(sizeof(node_t)) ;
	n->element = malloc(l->unit) ;
	memcpy(n->element, e, l->unit) ;

	if (l->last) {
		node_t * first ;
		first = l->last->next ;
		n->next = first ;
		l->last->next = n ;
		l->last = n ;
	}
	else {
		l->last = n ;
		l->last->next = n ;
	}
}

int
linkedlist_remove (linkedlist_t * l, node_t * n)
{
	if (l->last == 0x0)
		return 1 ;

	node_t * prev = l->last ;
	node_t * curr = l->last->next ;
	while (curr != n && curr != l->last) {
		prev = curr ;
		curr = curr->next ;		
	}
	if (curr != n) 
		return 1 ;

	if (prev != curr) 
		prev->next = curr->next ;
	else 
		l->last = 0x0 ;
	free(curr->element) ;
	free(curr) ;
	return 0 ;
}

int 
linkedlist_remove_first (linkedlist_t * l, void * e)
{
	if (l->last == 0x0)
		return 1 ;

	node_t * first = l->last->next ;
	memcpy(e, first->element, l->unit) ;
	linkedlist_remove(l, first) ;
	return 0 ;
}

int
linkedlist_remove_last (linkedlist_t * l, void * e)
{
	if (l->last == 0x0)
		return 1 ;

	node_t * last = l->last ;


	if (l->last == l->last->next) {
		l->last = 0x0 ;
	} 
	else {
		node_t * n = l->last ;
		while (n->next != l->last) {
			n = n->next ;
		}
		n->next = l->last->next ;
		l->last = n ;
	}

	memcpy(e, last->element, l->unit) ;
	free(last->element) ;
	free(last) ;
	return 0 ;
}

int
linkedlist_get (linkedlist_t * l, int pos, void * e)
{
	if (pos < 0 || l->last == 0x0)
		return 1 ;

	int i = 0 ;
	node_t * n = l->last->next ;
	while (i < pos && n != l->last) {
		n = n->next ;
		i++ ;
	}
	if (i != pos)
		return 1 ;

	memcpy(e, n->element, l->unit) ;
	return 0 ;
}
