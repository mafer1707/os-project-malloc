#include <unistd.h> // Para sbrk
#include <string.h> // Para memset y memcpy
#include "../include/mm_malloc.h"

// Inicio de la lista enlazada del heap
block_meta *list = NULL;

void *my_malloc(size_t size) {

    if(list != NULL){

        block_meta *current = list; 

        while (current != NULL)
        {        
            if(current != NULL && current->free == 1 && current->size >= size){

                 return current + 1; 
            }

            current = current->next;
        }      
    }

    block_meta *ptr = sbrk(size + sizeof(block_meta));
    ptr->free = 0;
    ptr->next = NULL;
    ptr->size = size;
    
    if(list == NULL){
         list = ptr;
    }        
    else{

        block_meta *ptr_aux = list;

        while (ptr_aux != NULL)
        {
            if(ptr_aux->next == NULL){
                ptr_aux->next = ptr;
                break;
            }

            ptr_aux = ptr_aux->next;
        }     
    }

    void *result = ptr + 1;

    return result; 
}

void my_free(void *ptr) {

    if(ptr == NULL) return;

    block_meta *ptr_aux = ptr - sizeof(block_meta);
    block_meta *neighbor = ptr_aux->next;

    if((((char*)ptr_aux + sizeof(block_meta) + ptr_aux->size) == (char*)neighbor) && neighbor->free){

        ptr_aux->size = ptr_aux->size + neighbor->size + sizeof(block_meta);
        ptr_aux->next = neighbor->next;
    }

    ptr_aux->free = 1;
}

void *my_calloc(size_t nmemb, size_t size) {

    void *ptr = my_malloc(nmemb * size);

    if(ptr == NULL) return NULL;

    memset(ptr, 0, nmemb * size);

    return ptr;
}

void *my_realloc(void *ptr, size_t size) {
   
    if(ptr == NULL)
        return my_malloc(size);
    
    if(size == 0){
        my_free(ptr);
        return NULL;
    }

    block_meta *ptr_aux = ptr - sizeof(block_meta);
    void *new_ptr;

    if(size < ptr_aux->size){
        return ptr;
    }
    else if(size > ptr_aux->size){

        new_ptr = my_malloc(size);

        if(new_ptr == NULL) return NULL;

        memcpy(ptr,new_ptr,ptr_aux->size);
        my_free(ptr);
    }

    return new_ptr;
}