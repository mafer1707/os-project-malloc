#include <unistd.h> // Para sbrk
#include "../include/mm_malloc.h"

// Inicio de la lista enlazada del heap
block_meta *list = NULL;

void *my_malloc(size_t size) {

    if(list != NULL){

        block_meta *current = list; 

        while (current != NULL)
        {
            current = current->next;

            if(current != NULL && current->free && current->size >= size)
                return current + sizeof(block_meta);
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
    // TODO: Marcar el bloque como libre.
    // TODO: Fusionar bloques adyacentes (Coalescing).
    (void)ptr;
}

void *my_calloc(size_t nmemb, size_t size) {
    // TODO: Usar my_malloc y luego memset a 0.
    (void)nmemb;
    (void)size;
    return NULL;
}

void *my_realloc(void *ptr, size_t size) {
    // TODO: Redimensionar el bloque o moverlo a uno nuevo.
    (void)ptr;
    (void)size;
    return NULL;
}

int main(){

    my_malloc(50);
    my_malloc(100);
    my_malloc(70);
    my_malloc(20);

    return 0;
}