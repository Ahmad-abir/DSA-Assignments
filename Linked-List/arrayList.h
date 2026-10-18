#include <stdio.h>
#include <stdlib.h>
typedef struct 
{
    int *array;
    // declare variables you need
    int capacity;
    int size;
    int cur_idx;

} arrayList;

void arrange(int f,int l,arrayList*list);
void decreaser(arrayList*list);
void checker(arrayList*list);

void init(arrayList* list)
{
    // implement initialization
    list -> capacity = 8;
    list -> size = 3 ;
    list -> cur_idx = 1;
    list -> array = (int*) malloc((list->capacity)*sizeof(int));
    list->array[0] = 3;
    list->array[1] = 4;
    list->array[2] = 5;
}

void free_list(arrayList* list)
{
    // implement destruction of list
    free(list -> array);
    //list -> array = (int*)malloc(2*sizeof(int));
    list -> capacity = 2;
    list -> size = 0;
    list -> cur_idx = 0;
}

void increase_capacity(arrayList* list)
{
    // implement capacity increase
    int new_cap = list -> capacity * 2;
    list -> array = (int*) realloc(list -> array, (new_cap)*sizeof(int));
    list -> capacity = new_cap;

}

void decrease_capacity(arrayList* list)
{
    // implement capacity decrease
    int new_cap = list -> capacity / 2;
    list -> array = (int*) realloc(list -> array, (new_cap)*sizeof(int));
    list -> capacity = new_cap;

}

void print(arrayList* list)
{
    // implement list printing
    printf("[");
    for(int i = 0 ; i < list -> size ; i++){
        printf(" %d" , list->array[i]);
        if(list->cur_idx==i){
            printf("|");
        }
    }
    if(list->size == list->cur_idx && list->size != 0) printf("|");
    if(list->size == 0) printf(" .");
    printf(" ]\n");
}

void insert(int item, arrayList* list)
{
    // implement insert function
    if(list->size+1>list->capacity/2){
        increase_capacity(list);
    }
    arrange(list->cur_idx,list->size-1,list);
    list->array[list->cur_idx+1] = item;
    list->size++;
    list->cur_idx++;
    checker(list);
}

int delete_cur(arrayList* list)
{
    // implement deletion of element at current index position
    if(list->size==0){
        return -1;
    }
    int temp = list->array[list->cur_idx];
    for(int i  = list->cur_idx ; i < list->size ; i++){
        list->array[i] = list->array[i+1];
    }
    list->size--;
    decreaser(list);
    return temp; 
}

void append(int item, arrayList* list)
{
    // implement append function
    if(list->size+1>list->capacity/2){
        increase_capacity(list);
    }
    list->array[list->size] = item;
    list->size++;
}

int size(arrayList* list)
{
    // implement size function
    return list->size;
}

void prev(int n, arrayList* list)
{
    // implement prev function
    if(n > list->cur_idx) list->cur_idx = 0;
    else list->cur_idx = list->cur_idx - n;
}

void next(int n, arrayList* list)
{
    // implement next function
    if(n > (list->size-1 - list->cur_idx)) list->cur_idx = list->size-1;
    else list->cur_idx = list->cur_idx + n;
}

int is_present(int n, arrayList* list)
{
    // implement presence checking function
    for(int i = 0 ; i < list->size ; i++){
        if(list->array[i] == n) return 1;
    }
    return 0;
}

void clear(arrayList* list)
{
    // implement list clearing function
    free(list->array);
    list->size=0;
    list->capacity=2;
    list->cur_idx=0;
}

int delete_item(int item, arrayList* list)
{
    // implement item deletion function
    int idx = -1;
    for(int i = 0 ; i < list->size ; i++){
        if(list->array[i] == item){
            idx = i;
            break;
        }
    }
    if(idx == -1 ) return 0;
    else{
        for(int i  = idx ; i < list->size ; i++){
            list->array[i] = list->array[i+1];
        }
        list->size--;
        list->cur_idx--;
        decreaser(list);
        return 1;
    }
}

void swap_ind(int ind1, int ind2, arrayList* list)
{
    // implement swap function at metioned index position
    int temp = list->array[ind1];
    list->array[ind1]=list->array[ind2];
    list->array[ind2]=temp;

}

int search(int item, arrayList* list)
{
    // implement search function
    int idx = -1;
    for(int i = 0 ; i < list->size ; i++){
        if(list->array[i] == item){
            idx = i ; 
            break;
        }
    }
    if(idx!=-1) list->cur_idx=idx;
    return idx;
}

int find(int ind, arrayList* list)
{
    // implement find function
    if(ind >= list->size) return -1;
    else{
        list->cur_idx=ind;
        return list->array[ind];
    } 
}

int update(int ind, int value, arrayList* list)
{
    // implement update function at metioned index position
    if(ind >= list->size ) return -1;
    else{
        int prev = list->array[ind];
        list->array[ind] = value;
        list->cur_idx=ind;
        return prev;
    }
}

int trim(arrayList* list)
{
    // implement trim function
    int l = list->array[list->size-1] ;
    list->size--;
    decreaser(list);
    return l;
}

void reverse(arrayList* list)
{
    // implement reverse function
    for(int i = 0 ; i*i < list->size;i++){
        int temp = list->array[i];
        list->array[i] = list->array[list->size-i-1];
        list->array[list->size-i-1] = temp;
    }

}

// you can define helper functions you need

void arrange(int f,int l,arrayList*list){
    if(l+1>list->capacity/2){
        increase_capacity(list);
    }
    for(int i = l ; i >= f+1 ; i--){
        list->array[i+1] = list->array[i];
    }
}

void decreaser(arrayList*list){
    if(list->cur_idx>list->size) list->cur_idx--;
    if(list->size <= list->capacity/4) decrease_capacity(list);
    if(list->size == 0 ) list->capacity=2;
}

void checker(arrayList*list){
    if(list->cur_idx > list->size-1) list->cur_idx = list->size-1;
    if(list->cur_idx < 0 ) list->cur_idx = 0 ;
}