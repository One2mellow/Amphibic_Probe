#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define BMP "fishpool.bmp"
#define BMPCPY "fishpool-copy.bmp"
#define TXT "pools.txt"
#define BEST_TXT "best-route.txt"

typedef struct { //bmp file values struct
	int width;
	int height;
} image_t;

typedef struct { //pixel color
	int r;
	int g;
	int b;
} color_t;

typedef struct { //pixel's coordinates
	int x;
	int y;
}co_t;

typedef struct pxgroup {
	color_t color;
	co_t position;
}pxgroup;

typedef struct pixel { //list of pixels
	co_t p;
	struct pixel* next;
}pix_t;

typedef struct pool { //pools' list extructed of bmp
	int size; //the number of pixels that combine the pool
	co_t poolCenter;
	pix_t* pArr; //list of pool pixels
	struct pool* next;
}poolList_t;

bool LoadSprite(image_t* image, const char* filename);

co_t pool_middle(co_t arr[], int size);


int main() {
	int i, j;

	pxgroup** imgtrx;
	static image_t image;

	if ((LoadSprite(&image, BMP)) != 0) {
		printf_s("Failed to load file: \" %s\"", BMP);
		return -1;
	}

	imgtrx = malloc(sizeof(pxgroup*) * image.height); // allocate memory to image pixel matrix
	for (i = 0;i < image.height;i++) {
		imgtrx[i] = malloc(sizeof(pxgroup) * image.width);
	}


	return 0;
}


co_t pool_middle(co_t arr[], int size) {
	int x_max, x_min, y_max, y_min, i;
	co_t middle;
	x_max = x_min = arr[0].x;
	y_max = y_min = arr[0].y;
	for (i = 0; i < size; i++) {
		if (x_min > arr[i].x)
			x_min = arr[i].x;
		if (x_max < arr[i].x)
			x_max = arr[i].x;
		if (y_min > arr[i].y)
			y_min = arr[i].y;
		if (y_max < arr[i].y)
			y_max = arr[i].y;
	}
	middle.x = (x_max + x_min) / 2;
	middle.y = (y_max + y_min) / 2;
	return middle;
}
//try it in the main function
/*co_t test[] = {{5,2}, {15,1}, {2,3}, {300,6}, {11,100}, {2,1}, {50,5}};
	co_t middle_cot = pool_middle(test,sizeof test/8);
	printf_s("The pool middle cordinate is (%d,%d)", middle_cot.x, middle_cot.y);*/


	/* Bitmap file format
	 *
	 * SECTION
	 * Address:Bytes	Name
	 *
	 * HEADER:
	 *	  0:	2		"BM" magic number
	 *	  2:	4		file size
	 *	  6:	4		junk
	 *	 10:	4		Starting address of image data
	 * BITMAP HEADER:
	 *	 14:	4		header size
	 *	 18:	4		width  (signed)
	 *	 22:	4		height (signed)
	 *	 26:	2		Number of color planes
	 *	 28:	2		Bits per pixel
	 *	[...]
	 * [OPTIONAL COLOR PALETTE, NOT PRESENT IN 32 BIT BITMAPS]
	 * BITMAP DATA:
	 *	138:	X	Pixels
	 */
bool LoadSprite(image_t* image, const char* filename) {
	int return_value = 0;

	unsigned int image_data_address;
	int width;
	int height;

	printf("Loading bitmap file: %s\n", filename);

	FILE* file;
	return_value = fopen_s(&file, filename, "rb");
	if (file) {
		if (fgetc(file) == 'B' && fgetc(file) == 'M') {
			printf("BM read; bitmap file confirmed.\n");
			fseek(file, 8, SEEK_CUR);
			fread(&image_data_address, 4, 1, file);
			fseek(file, 4, SEEK_CUR);
			fread(&width, 4, 1, file);
			fread(&height, 4, 1, file);
			fseek(file, 2, SEEK_CUR);

			image->width = width;
			image->height = height;
		}
		fclose(file);
	}
	else {
		//PRINT_ERROR("(%s) Failed to open file", filename);
		return_value = false;
	}
	return return_value;
}
