#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_image(int num_rows, int num_cols, 
                int arr[num_rows][num_cols], FILE *fp);
                
void print_image(int num_rows, int num_cols, int arr[num_rows][num_cols]);

int count_cells(int num_rows, int num_cols, int arr[num_rows][num_cols]);

int main(int argc, char **argv) {
	// > count_cells ./img.txt
	// ----> argc is 2, argv is ["count_cells", "./img.txt"]
	// > count_cells ./img.txt -p
	// ----> argc is 3, argv is ["count_cells", "./img.txt", "-p"]
	if (argc <= 1 || argc >= 4 || (argc == 3 && strcmp(argv[2], "-p") != 0)) {
		fprintf(stderr, "Usage: count_cells <imagefile.txt> [-p]\n");
		return 1;
	}
	FILE *fp;
	fp = fopen(argv[1], "r");
	int num_rows, num_cols;
	fscanf(fp, "%d %d", &num_rows, &num_cols);
	int arr[num_rows][num_cols];
	read_image(num_rows, num_cols, arr, fp);
	fclose(fp);
	if (argc == 3) print_image(num_rows, num_cols, arr);
	printf("Number of Cells is %d\n", count_cells(num_rows, num_cols, arr));
    return 0;
}