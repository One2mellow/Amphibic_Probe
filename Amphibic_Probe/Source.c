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
	union { int width, w; };
	union { int height, h; };
	union { int* pixels, * p; };
} map_t;

map_t frame;

typedef struct { //pixel color
	int r;
	int g;
	int b;
} color_t;

typedef struct { //pixel's coordinates
	int x;
	int y;
}co_t;

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

bool LoadSprite(map_t* sprite, const char* filename);

co_t pool_middle(co_t arr[], int size);


int main() {

	static map_t sprite;
	
	if (!LoadSprite(&sprite, BMP)) {
		printf_s("Failed to load file: \" %s\"",BMP);
		return -1;
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
bool LoadSprite(map_t* sprite, const char* filename) {
	bool return_value = true;

	unsigned int image_data_address;
	int width;
	int height;
	unsigned int pixel_count;
	unsigned int bit_depth;
	unsigned int byte_depth;
	unsigned int* pixels;

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
			fread(&bit_depth, 2, 1, file);
			if (image_data_address != 138) { // Potential version mismatch or corrupt file
				//PRINT_ERROR("(%s) Image data address expected %d is %d", filename, 138, image_data_address);
				return_value = false;
			}
			else if (bit_depth != 32) {
				//PRINT_ERROR("(%s) Bit depth expected %d is %d", filename, 32, bit_depth);
				return_value = false;
			}
			else { // Image metadata correct
				printf("image data address:\t%d\nwidth:\t\t\t%d pix\nheight:\t\t\t%d pix\nbit depth:\t\t%d bpp\n", image_data_address, width, height, bit_depth);
				pixel_count = width * height;
				byte_depth = bit_depth / 8;
				pixels = malloc(pixel_count * byte_depth);
				if (pixels) {
					fseek(file, image_data_address, SEEK_SET);
					int pixels_read = fread(pixels, byte_depth, pixel_count, file);
					printf("Read %d pixels\n", pixels_read);
					if (pixels_read == pixel_count) {
						sprite->w = width;
						sprite->h = height;
						sprite->p = pixels;
					}
					else {
						//PRINT_ERROR("(%s) Read pixel count incorrect. Is %d expected %d", filename, pixels_read, pixel_count);
						free(pixels);
						return_value = false;
					}
				}
				else {
					printf("(%s) Failed to allocate %d pixels.\n", filename, pixel_count);
					return_value = false;
				}
			} // Done loading sprite
		}
		else {
			//PRINT_ERROR("(%s) First two bytes of file are not \"BM\"", filename);
			return_value = false;
		}

		fclose(file);
	}
	else {
		//PRINT_ERROR("(%s) Failed to open file", filename);
		return_value = false;
	}
	return return_value;
}