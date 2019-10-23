
/* Remember that header files must not contain code or declaration of 
 * variables that would require memory to be allocated.
 */


struct block {
    void *addr; /*start address of memory for this block */
    int size;
    struct block *next;
};

/****************************************************************************/
/* Implemented in smalloc.c */

/* Allocates size bytes of memory for the dynamically allocated memory 
 * algorithm to use */
void mem_init(int size);

/* Reserves nbytes of space from the memory region created by mem_init.  Returns
 * a pointer to the reserved memory. Returns NULL if memory cannot be allocated */    
void *smalloc(unsigned int nbytes);

/* Free the reserved space starting at addr.  Returns 0 if successful 
 * -1 if the address cannot be found in the list of allocated blocks */

/*(30%) Reserves nbytes bytes of space from the memory region created by mem_init. 
If the memory is reserved (allocated) successfully, Returns a pointer to the 
reserved memory. If the memory cannot be reserved (i.e. there is no block that is
large enough to hold nbytes bytes), returns NULL.*/

int sfree(void *addr);

/* Free any dynamically used memory in the allocated and free list */

/*(30%) Returns memory allocated by smalloc to the list of free
blocks so that it might be reused later.*/

void mem_clean();

/*(10%) Uses free (that's the C function free, not your sfree!) to 
free all the dynamically allocated memory (allocated_list and freelist) 
used by the program before exiting. (The valgrind program must show all h
eap memory freed, otherwise you have a memory leak. See below.)*/

/****************************************************************************/
/* Implemented in testhelpers.c */
/* The remaining functions are for testing purposes*/

/* prints the list of allocated blocks */
void print_allocated(void);

/* prints the list of free blocks */
void print_free(void);

/* A function that writes to the memory allocated by smalloc */
void write_to_mem(int size, char *ptr, char value);

/* prints the contents of allocated memory */
void print_mem(void);
