#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Write random integers (in binary) to a file with the name given by the command-line
 * argument.  This program creates a data file for use by the time_reads program.
 */

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: write_test_file filename\n");
        exit(1);
    }

    FILE *fp;
    if ((fp = fopen(argv[1], "w")) == NULL) {
        perror("fopen");
        exit(1);
    }
    int numbers[100];
    //write 100 random integers to the file
    /* from 0 to 99 and use that number as an index */
    for (int i = 0; i < 99; ++i) {
        numbers[i] = rand() % 100;
        printf("%d\n", numbers[i]);
        fwrite(numbers, sizeof(int), 100, fp);
}
    fclose(fp);
    return 0;
}
