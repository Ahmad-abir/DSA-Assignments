#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int element;
    struct node* next;
    struct node* prev;
} node;

typedef struct 
{
    // declare head, tail, cur and other variables you need
    node*head;
    node*tail;
    int size;
    int cur_idx;

} linkedList;

void checker(linkedList*list);

void init(linkedList* list)
{
    // implement initialization
    node* node1 = (node*)malloc(sizeof(node));
    node* node2 = (node*)malloc(sizeof(node));
    node* node3 = (node*)malloc(sizeof(node));

    node1->element = 3 ;
    node2->element = 4 ; 
    node3->element = 5 ;

    node1->next = node2;
    node2->next = node3;
    node3->next = NULL;

    node1->prev = NULL;
    node2->prev = node1;
    node3->prev = node2;

    list->head = node1;
    list->tail = node3;
    list->size = 3;
    list->cur_idx = 1;

}

void free_list(linkedList* list)
{
    // implement destruction of list
    node*current = list->head;
    while(current!=NULL){
        node*temp = current;
        current = current->next;
        free(temp);
    }
    list->head=NULL;
    list->tail=NULL;
    list->cur_idx=0;
    list->size=0;
}

void print(linkedList* list)
{
    // implement list printing
    node*current = list->head;
    int counter = 0 ;
    printf("[");
    for(int i = 0 ; current!=NULL ; i++){
        printf(" %d" , current->element);
        current = current->next;
        if(i==list->cur_idx) printf("|");
    }
    if(list->size == 0) printf(".");
    printf(" ]\n");
}

void insert(int item, linkedList* list)
{
    // implement insert function
    node * newnode = (node*)malloc(sizeof(node));
    newnode->element = item;
    int i =0;
    node*current = list->head;
    while(i!=list->cur_idx){
        current = current->next;
        i++;
    }
    newnode->prev = current;
    newnode->next = current->next;
    if(current->next != NULL){
        current->next->prev = newnode;
    } 
    else list->tail=newnode;
    current->next = newnode;
    list->size++;
    list->cur_idx++;
    checker(list);
}

int delete_cur(linkedList* list)
{
    // implement deletion of current index function
    if (list == NULL) return -1;
    node*current = list->head;
    for(int i = 0 ; i < list->cur_idx ;i++){
        current = current->next;
    }
    int l = current->element;
    if(current->next != NULL) current->next->prev = current->prev;
    else list->tail = current->prev;

    if(current->prev != NULL) current->prev->next = current->next;
    else list->head = current->next;
    list->size--;
    list->cur_idx--;
    free(current);
    checker(list);
    return l;
}

void append(int item, linkedList* list)
{
    // implement append function
    if (list == NULL) return;
    node * newnode = (node*) malloc(sizeof(node));
    newnode->element=item;
    newnode->next=NULL;

    if(list->size == 0){
        newnode->prev=NULL;
        list->head=newnode;
        list->tail=newnode;
    }
    else{
        newnode->prev=list->tail;
        list->tail->next=newnode;
        list->tail=newnode;
    }
    list->size++;
    //list->cur_idx++;
    checker(list);
}

int size(linkedList* list)
{
    // implement size function
    return list->size;
}

void prev(int n, linkedList* list)
{
    // implement prev function
    if (list == NULL) return;
    if(n<0) return;
    list->cur_idx=list->cur_idx-n;
    checker(list);
}

void next(int n, linkedList* list)
{
    // implement next function
    if (list == NULL) return;
    if(n<0) return ;
    list->cur_idx +=n;
    checker(list);

}

int is_present(int n, linkedList* list)
{
    // implement presence checking function
    if (list == NULL) return 0;
    node*current = list->head;
    while(current !=NULL){
        if(current->element == n) return 1;
        current = current->next;
    }
    return 0;
}

void clear(linkedList* list)
{
    // implement list clearing function
    free_list(list);
}

int delete_item(int item, linkedList* list)
{
    // implement item deletion function
    if (list == NULL) return 0;
    node*current= list->head;
    int is_found =0;
    int idx=0;
    while(current!=NULL){
        if(current->element == item){
            is_found =1;
            break;
        }
        idx++;
        current = current->next;
    }
    if(is_found){
        int temp = list->cur_idx;
        list->cur_idx=idx;
        delete_cur(list);
        list->cur_idx = temp -1;
        checker(list);
        return 1;
    }
    else return 0;

}

void swap_ind(int ind1, int ind2, linkedList* list)
{
    // implement swap function
    if (list == NULL) return;
    if(ind1==ind2) return;

    node* node1 = list->head;
    for (int i = 0; i < ind1; i++)
        node1 = node1->next;

    node* node2 = list->head;
    for (int i = 0; i < ind2; i++)
        node2 = node2->next;

    int temp = node1->element;
    node1->element = node2->element;
    node2->element = temp;
}


int search(int item, linkedList* list)
{
    // implement search function
    int idx = 0;
    node*current = list->head;
    while(current!= NULL){
        if(current->element == item) {
            return idx;
        }
        idx++;
        current= current->next;
    }
    return -1;

}

int find(int ind, linkedList* list)
{
    // implement find function
    if(ind > list->size -1 || ind<0) return -1;
    node*current = list->head ;
    for( int i = 0 ; i< ind ; i++ ){
        current = current->next;
    }
    return current->element;
}

int update(int ind, int value, linkedList* list)
{
    // implement update function at metioned index position
    if(ind > list->size -1 || ind<0 || list->size==0) return -1;
    node*current = list->head;
    for(int i = 0 ; i < ind ; i++){
        current=current->next;
    }
    int l = current->element;
    current->element=value;
    list->cur_idx=ind;
    return l;

}

int trim(linkedList* list)
{
    // implement trim function
    if(list->size==0) return -1;
    int l = list->tail->element;
    node*temp = list->tail;
    if(temp->prev!=NULL){
        list->tail=temp->prev;
        list->tail->next = NULL; 
        list->size--;
        free(temp);
        checker(list);
        return l;
    } 
    else{
        free_list(list);
        return l;
    } 
}

void reverse(linkedList* list)
{
    // implement reverse function
    if(list->size==0) return;
    node*node1=list->head;
    node*node2=list->tail;
    for(int i = 0 ; i < list->size/2 ; i++){
        int temp = node1->element;
        node1->element = node2->element;
        node2->element = temp;
        node1 = node1->next;
        node2 = node2->prev;
    }
}

// you can define helper functions you need

void checker(linkedList*list){
    if(list->cur_idx<0) list->cur_idx = 0;
    if(list->cur_idx+1>list->size) list->cur_idx=list->size-1;
}