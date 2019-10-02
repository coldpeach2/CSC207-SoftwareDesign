#include <stdio.h>
    
/* Reads the image from the open file fp into the two-dimensional array arr 
 * num_rows and num_cols specify the dimensions of arr
 */
void read_image(int num_rows, int num_cols, 
                int arr[num_rows][num_cols], FILE *fp) {
	for (int y = 0; y < num_rows; y++) {
		for (int x = 0; x < num_cols; x++) {
			if (!fscanf(fp, "%d", &arr[y][x])) arr[y][x] = 0;
		}
	}
}

/* Print to standard output the contents of the array arr */
void print_image(int num_rows, int num_cols, int arr[num_rows][num_cols]) {
	for (int y = 0; y < num_rows; y++) {
		for (int x = 0; x < num_cols - 1; x++) {
			printf("%d ", arr[y][x]);
		}
		printf("%d\n", arr[y][num_cols - 1]);
	}
}

void mark_cell(int num_rows, int num_cols, int arr[num_rows][num_cols], int y, int x) {
	arr[y][x] = 256;
	if (y > 0 && arr[y - 1][x] == 255) mark_cell(num_rows, num_cols, arr, y - 1, x);
	if (y < num_rows - 1 && arr[y + 1][x] == 255) mark_cell(num_rows, num_cols, arr, y + 1, x);
	if (x > 0 && arr[y][x - 1] == 255) mark_cell(num_rows, num_cols, arr, y, x - 1);
	if (x < num_cols - 1 && arr[y][x + 1] == 255) mark_cell(num_rows, num_cols, arr, y, x + 1);
}

int count_cells(int num_rows, int num_cols, int arr[num_rows][num_cols]) {
	int count = 0;
	for (int y = 0; y < num_rows; y++) {
		for (int x = 0; x < num_cols; x++) {
			if (arr[y][x] == 255) {
				mark_cell(num_rows, num_cols, arr, y, x);
				count++;
			}
		}
	}
	return count;
}