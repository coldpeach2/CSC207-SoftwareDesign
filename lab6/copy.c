#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Write the copy function to perform exactly as strncpy does, with one
   exception: your copy function will guarantee that dest is always
   null-terminated.
   You shoud read the man page to learn how strncpy works.

  NOTE: You must write this function without using any string functions.
  The only function that should depend on string.h is memset.
 */

/*   The stpncpy() and strncpy() functions copy at most len characters from
     src into dst.  
     If src is less than len characters long, the remainder of
     dst is filled with `\0' characters.  Otherwise, dst is not terminated.*/


char *copy(char *dest, const char *src, int capacity) {
  if (dest == NULL){
    return NULL;
  }
  for (int i = 0; i < capacity && src[i] != '\0'; i++){
    dest[i] = src[i];

    for (; i < capacity; i++){
      dest[i] = '\0';
    }
  }
  return dest;
}


int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: copy size src\n");
        exit(1);
    }
    int size = strtol(argv[1], NULL, 10);
    char *src = argv[2];

    char dest[size];
    memset(dest, 'x', size);

    copy(dest, src, size);

    printf("%s\n", dest);
    return 0;
}
