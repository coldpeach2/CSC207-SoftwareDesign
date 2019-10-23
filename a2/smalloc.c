#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"



void *mem;
struct block *freelist;
struct block *allocated_list;

int allignment(unsigned int nbytes){
    int temp = 0;
    int i = 1;
    while(temp < nbytes){
        temp = 8 * i;
        i++;
    }
    return nbytes = temp;
}

void *smalloc(unsigned int nbytes) {
    nbytes = allignment(nbytes);

	struct block *curr = freelist;
    struct block *prev = NULL;
    while (curr != NULL){
        if(curr->size >= nbytes){

            if(curr->size == nbytes){
                // remove from freelist
                if(prev == NULL){
                    freelist = freelist->next;
                }else{
                    prev->next = curr->next;
                }
                // place in allocated list
                curr->next = allocated_list;
                allocated_list = curr;
                return curr->addr;

            }else{

                struct block *new_block = malloc(nbytes * sizeof(struct block));
                new_block->addr = curr->addr;
                new_block->size = nbytes;
                curr->size = curr->size - nbytes;
                curr->addr = curr->addr + nbytes;
                new_block->next = allocated_list;
                allocated_list = new_block;
                
                return new_block->addr;

            }
        }
        prev = curr;
        curr = curr->next;
    }
 
    return NULL;
}


int sfree(void *addr) {
	struct block *curr = allocated_list;
    struct block *prev = NULL;
    while(curr != NULL){
        if(curr->addr == addr){
            // removes from allocated list
            if(prev == NULL){
                allocated_list = allocated_list->next;
            }else{
                prev->next = curr->next;
            }
            struct block *curr_free = freelist;
            struct block *prev_free = NULL;
            while(curr_free != NULL){
                // handles adding to front of freelist
                if (prev_free == NULL && (curr->addr) < (curr_free->addr)){
                    curr->next = freelist;
                    freelist = curr;
                    break;
                }
                // handles adding curr to freelist in between two nodes
                else if((prev_free->addr) < (curr->addr) && (curr->addr) < (curr_free->addr)){

                    curr->next = curr_free;
                    prev_free->next = curr;
                    break;
                }
                prev_free = curr_free;
                curr_free = curr_free->next;
            }

            // handles edge case to place curr at the front of freelist
            if(prev_free != NULL && prev_free->addr < curr->addr){
                prev_free->next = curr;
                curr->next = NULL;
            }
            // handles edge case if freelist is empty
            if(freelist == NULL){
                freelist = curr;
                curr->next = NULL;
            }
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return -1;
}


void mem_init(int size) {
    // mem stores the starting address of the memory region that is reserved by mem_init.
    // mmap returns a pointer to a block of memory
    mem = mmap(NULL, size,  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if(mem == MAP_FAILED) {
         perror("mmap");
         exit(1);
    }

    freelist = malloc(size * sizeof(struct block));
    freelist->addr = mem;
    freelist->size = size;
    return;

}


void mem_clean(){
    struct block *curr = allocated_list;
    struct block *prev = NULL;
    while(curr != NULL){
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    struct block *curr_free = freelist;
    struct block *prev_free = NULL;
   while(curr_free != NULL){
        prev_free = curr_free;
        curr_free = curr_free->next;
        free(prev_free);
    }
}

