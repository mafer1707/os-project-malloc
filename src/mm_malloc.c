#include <unistd.h> // Para sbrk
#include <string.h> // Para memset y memcpy
#include "../include/mm_malloc.h"

// Inicio de la lista enlazada del heap
block_meta *list = NULL;

void *my_malloc(size_t size) {

    if (size == 0) {
        return NULL;
    }

    //Alineación (Código tomado de Gemini)

    //Explicación: esta alineación se usa para redondear el size que no sea multiplo de 8
    //al múltiplo de 8 más cercano. 
    //Ejemplo: un size de 5 lo convertiría a 8. Un size de 13 a 16.

    //Esto es necesario para que la información se guarde en bloques de a 8 bytes cada uno y sea más
    //fácil para el computador leer los datos.

    //size + (alignment - 1) hace que el size cruce/supere al primer multiplo de 8 superior a él.
    //~(alignment - 1) le aplica una negación al binario del número que se forma en la resta. En este caso ~7 

    //(size + (alignment - 1)) & ~(alignment - 1) aplica un AND a estos dos números a nivel binario.
    //De este modo logra que el size sea múltiplo de 8 ya que esta operación hace que el resultado siempre termine en tres ceros
    //lo cual es lo que queremos porque cualquier múltiplo de 8 (8, 16, 24, 32...) en binario siempre termina en tres ceros (000).

    size_t alignment = 8;
    size = (size + (alignment - 1)) & ~(alignment - 1);

    //

    if(list != NULL){

        block_meta *current = list; 

        while (current != NULL)
        {        
            if(current->free == 1 && current->size >= size){

                 current->free = 0;
                 return current + 1; 
            }

            current = current->next;
        }      
    }

    block_meta *ptr = (block_meta*) sbrk(size + sizeof(block_meta));

    if (ptr == (void *)-1) {
        return NULL; 
    }

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

    block_meta *ptr_aux = (block_meta*)ptr - 1;
    block_meta *neighbor = ptr_aux->next;

    if(neighbor != NULL && neighbor->free && (((char*)ptr_aux + sizeof(block_meta) + ptr_aux->size) == (char*)neighbor)){

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

    size_t alignment = 8;
    size = (size + (alignment - 1)) & ~(alignment - 1);

    block_meta *ptr_aux = (block_meta*)ptr - 1;
    void *new_ptr;

    if(size <= ptr_aux->size){
        return ptr;
    }
    else {

        new_ptr = my_malloc(size);

        if(new_ptr == NULL) return NULL;

        memcpy(new_ptr, ptr, ptr_aux->size);
        my_free(ptr);
    }

    return new_ptr;
}