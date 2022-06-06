#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define BMP "fishpool-another-ex1.bmp"
#define BMPCPY "fishpool-copy.bmp"
#define TXT "pools.txt"
#define BEST_TXT "best-route.txt"

typedef struct { //bmp file values struct
	int width;
	int height;
} image_t;

typedef struct { //pixel color
	unsigned char r;
	unsigned char g;
	unsigned char b;
} color_t;

typedef struct { //pixel's coordinates
	int x;
	int y;
}co_t;

typedef struct { // struct to contain each pixel color & coordinates
	color_t color;
	co_t cordinate;
} pixmat;

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

co_t pool_middle(pixmat arr[], int size);

void imgtrx(pixmat* mtrx, image_t image, char* filename);

void CreateBMP(char* filename, color_t** matrix, int height, int width);

int main() {
	int i, j, t;

	pixmat* matrix;
	static image_t image;

	if ((LoadSprite(&image, BMP)) != 0) {
		printf_s("Failed to load file: \" %s\"", BMP);
		return -1;
	}


	matrix = malloc(sizeof(pixmat) * image.height * image.width); // allocate memory to image pixel matrix


	imgtrx(matrix, image, BMP);
	//	CreateBMP(BMPCPY, matrix, image.height, image.width);

	free(matrix);
	return 0;
}


co_t pool_middle(pixmat arr[], int size) {
	int x_max, x_min, y_max, y_min, i;
	co_t middle;
	x_max = x_min = arr[0].cordinate.x;
	y_max = y_min = arr[0].cordinate.y;
	for (i = 0; i < size; i++) {
		if (x_min > arr[i].cordinate.x)
			x_min = arr[i].cordinate.x;
		if (x_max < arr[i].cordinate.x)
			x_max = arr[i].cordinate.x;
		if (y_min > arr[i].cordinate.y)
			y_min = arr[i].cordinate.y;
		if (y_max < arr[i].cordinate.y)
			y_max = arr[i].cordinate.y;
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
	int bpp;

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
			fread(&bpp, 4, 1, file);

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

void imgtrx(pixmat* mtrx, image_t image, char* filename) {
	int val, i = 0, j, k = 0;
	FILE* file;

	k = image.height * image.width;

	val = fopen_s(&file, filename, "rb");

	if (file != 0)
	{

		fseek(file, 54, SEEK_SET);
		for (i = 0; i < k; i++)
		{
			mtrx[i].color.b = fgetc(file);
			mtrx[i].color.g = fgetc(file);
			mtrx[i].color.r = fgetc(file);
			mtrx[i].cordinate.x = i % image.width;
			mtrx[i].cordinate.y = i / image.width;
			if (mtrx[i].color.b == 0 && mtrx[i].color.g == 0 && mtrx[i].color.r == 0) {
				i--;
			}
		}

	}

	if (val != 0)
		fclose(file);

	return 0;
}



void CreateBMP(char* filename, pixmat* matrix, int height, int width) {


	int i, j;
	int padding, bitmap_size;
	color_t* wrmat;
	int t = width * height;
	wrmat = malloc(sizeof(color_t) * height * width);



	if (((width * 3) % 4) != 0) {
		padding = (width * 3) + 1;
	}
	else
	{
		padding = width * 3;
	}

	bitmap_size = height * padding * 3;

	char tag[] = { 'B', 'M' };
	int header[] = {
		0x3a, 0x00, 0x36,
		0x28,                // Header Size
		width, height,       // Image dimensions in pixels
		0x180001,            // 24 bits/pixel, 1 color plane
		0,                   // BI_RGB no compression
		0,                   // Pixel data size in bytes
		0x002e23, 0x002e23,  // Print resolution
		0, 0,                // No color palette
	};
	header[0] = sizeof(tag) + sizeof(header) + bitmap_size;

	FILE* fp;
	fopen_s(&fp, filename, "w+");
	fwrite(&tag, sizeof(tag), 1, fp);
	fwrite(&header, sizeof(header), 1, fp); //write header to disk
	fwrite(wrmat, bitmap_size * sizeof(char), 1, fp);
	fclose(fp);

	fclose(fp);
	free(wrmat);
}