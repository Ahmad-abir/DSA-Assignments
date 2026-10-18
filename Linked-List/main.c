#include <stdio.h>
//#include "arrayList.h"
#include "linkedList.h"

int main()
{
    FILE* file = fopen("in.txt", "r");
    if(file == NULL){
        return 1;
    }

    //arrayList list;
    linkedList list;
    init(&list);

    int func, param, param2;
    while(fscanf(file, "%d", &func) == 1 && func != 0)
    {
        if(func == 1 && fscanf(file, "%d", &param) == 1){
            printf("Insert %d\n", param);
            insert(param, &list);
            print(&list);
        }
        else if(func == 2){
            printf("Delete current item\n");
            int ret = delete_cur(&list);
            if(ret>=0){
                print(&list);
                printf("%d is deleted\n", ret);
            }
            else
                printf("List is empty\n");
        }
        else if(func == 3 && fscanf(file, "%d", &param) == 1){
            printf("Append %d\n", param);
            append(param, &list);
            print(&list);
        }
        else if(func == 4){
            int ret = size(&list);
            printf("Size of the list is %d\n", ret);
        }
        else if(func == 5 && fscanf(file, "%d", &param) == 1){
            printf("Prev %d\n", param);
            prev(param, &list);
            print(&list);
        }
        else if(func == 6 && fscanf(file, "%d", &param) == 1){
            printf("Next %d\n", param);
            next(param, &list);
            print(&list);
        }
        else if(func == 7 && fscanf(file, "%d", &param) == 1){
            int ret = is_present(param, &list);
            if(ret){
                printf("%d is present\n", param);
            }
            else{
                printf("%d is not present\n", param);
            }
        }
        else if(func == 8){
            clear(&list);
            print(&list);
            printf("The list was cleared.\n");
        }
        else if(func == 9 && fscanf(file, "%d", &param) == 1){
            int i = delete_item(param,&list);
            print(&list);
            if(i) printf("%d is deleted\n" , param);
            else printf("The item was not found!\n");
        }
        else if(func == 10 && fscanf(file, "%d", &param) == 1 && fscanf(file,"%d",&param2)==1){
            
            swap_ind(param,param2,&list);
            print(&list);
            printf("The index %d and %d was swapped.\n" , param,param2);
        }
        else if(func == 11 && fscanf(file, "%d", &param) == 1){
            int i = search(param,&list);
            printf("%d Found at : %d\n" ,param, i);
        }
        else if(func == 12 && fscanf(file, "%d", &param) == 1){
            int i = find(param,&list);
            printf("The element at index %d is: %d\n" , param,i);
        }
        else if(func == 13 && fscanf(file, "%d", &param) == 1){
            fscanf(file,"%d",&param2);
            int l = update(param,param2,&list);
            print(&list);
            printf("The num at index %d was updated from %d to %d\n " , param,l,param2);
        }
        else if(func == 14){
            int i = trim(&list);
            print(&list);
            printf("%d was trimmed.\n" , i);
        }
        else if(func == 15 ){
            reverse(&list);
            print(&list);
            printf("The list was reversed.\n");
        }
        else if(func == 16){
            print(&list);
            printf("List Size: %d \n" , list.size );
            //printf("List Capacity : %d \n" , list.capacity);
            printf("List Current Index : %d \n" , list.cur_idx);
        }
        
        // implement the remaining functionality here

    }

    return 0;
}