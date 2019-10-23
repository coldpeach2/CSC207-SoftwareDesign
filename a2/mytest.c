#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"

/* this test will be testing if the address returned by smalloc is always a multiple of 8.*/ 

int main(void) {
	int i;
    char *ptrs[4];
    ptrs[0] = smalloc(sizeof(char) * (strlen("Hello World") + 1));
    ptrs[1] = smalloc(30);
    ptrs[2] = smalloc(sizeof(int) * 16735);
    ptrs[3] = smalloc(0);

    for (i = 0; i < 4; i++){
    	int address = (int)(&ptrs[i]);
    	int isAlligned = (address % 8);
   		printf("%d\n", isAlligned);
   		printf("Address is a multiple of 8 with remainder:");
    	sfree(ptrs[i]);
    }
    return 0;
}

