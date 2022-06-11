#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define BMP "centers.bmp"
#define BMPCPY "fishpool-copy.bmp"
#define TXT "pools.txt"
#define BEST_TXT "best-route.txt"

const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

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
	pix_t* pix; //list of pool pixels
	struct pool* next;
}poolList_t;


bool LoadSprite(image_t* image, const char* filename);

co_t pool_middle(pix_t *root, int size);

void imgtrx(pixmat** mtrx, image_t image, char* filename);

void Pools(pixmat** mtrx, image_t image, poolList_t* pools);

void CreateBMP(char* filename, color_t** matrix, int height, int width);

int segment(pix_t* root, pixmat** mtrx, int** temp, image_t image, int i, int j, int* size);

void pix_insert(pix_t** root, co_t coordinate);

void pool_insert(poolList_t** root, int size, co_t center, pix_t* pix);

int main() {
	int i, j;
	poolList_t* pools = NULL;
	pixmat** matrix;
	static image_t image;

	if ((LoadSprite(&image, BMP)) != 0) {
		printf_s("Failed to load file: \" %s\"", BMP);
		return -1;
	}


	matrix = malloc(sizeof(pixmat*) * image.width);
	if (matrix)
	{
		for (i = 0;i < image.width;i++) {
			matrix[i] = malloc(sizeof(pixmat) * image.height);
		}
	}// allocate memory to image pixel matrix



	imgtrx(matrix, image, BMP);
	Pools(matrix, image, pools);
	//CreateBMP(BMPCPY, matrix, image.height, image.width);
	for (i = 0;i < image.width;i++) {
		free(matrix[i]);
	}	
	free(matrix);
	return 0;
}


co_t pool_middle(pix_t* root, int size) {

	int x_max, x_min, y_max, y_min, i;
	co_t middle;
	pix_t* curr;
	co_t* pixels;

	pixels = malloc(sizeof(co_t) * size+1);
	
	i = 0;

	curr = root;

	while (curr != NULL)
	{
		pixels[i].x = curr->p.x + 1;
		pixels[i].y = curr->p.y + 1;
		//printf("%d, %d\n", pixels[i].x, pixels[i].y);
		curr = curr->next;
		i++;
	}


	x_max = pixels[0].x;
	x_min = pixels[0].x;
	y_max = pixels[0].y;
	y_min = pixels[0].y;
	for (i = 0; i < size; i++) {
		if (x_min > pixels[i].x)
			x_min = pixels[i].x;
		if (x_max < pixels[i].x)
			x_max = pixels[i].x;
		if (y_min > pixels[i].y)
			y_min = pixels[i].y;
		if (y_max < pixels[i].y)
			y_max = pixels[i].y;
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

void imgtrx(pixmat** mtrx, image_t image, char* filename) {
	int val, i = 0, j, k = 0;
	FILE* file;

	k = image.height * image.width;

	val = fopen_s(&file, filename, "rb");

	if (file != 0)
	{

		fseek(file, 54, SEEK_SET);
		for (i = 0; i < image.height; i++)
		{
			for ( j = 0; j < image.width; j++)
			{
				mtrx[j][i].color.b = fgetc(file);
				mtrx[j][i].color.g = fgetc(file);
				mtrx[j][i].color.r = fgetc(file);
				mtrx[j][i].cordinate.x = j;
				mtrx[j][i].cordinate.y = i;
				if (mtrx[j][i].color.b == 0 || mtrx[j][i].color.g == 0 || mtrx[j][i].color.r == 0) {
					j--;
				}
			}
		}

	}

	//for ( i = 0; i < k; i++)
	//{
	//	printf("(%d, %d) : ", mtrx[i].cordinate.x, mtrx[i].cordinate.y);
	//	printf("{ %d, %d , %d }\n", mtrx[i].color.r, mtrx[i].color.g, mtrx[i].color.b);

	//}

	if (val != 0)
		fclose(file);

	return 0;
}





/*void CreateBMP(char* filename, pixmat* matrix, int height, int width) {

	int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (height * width);
	int val,i;

	static unsigned char fileHeader[] = {
	  'B','M',     /// signature
	  0,0,0,0, /// image file size in bytes
	  0,0,0,0, /// reserved
	  0,0,0,0, /// start of pixel array
	};

	fileHeader[2] = (unsigned char)(fileSize);
	fileHeader[3] = (unsigned char)(fileSize >> 8);
	fileHeader[4] = (unsigned char)(fileSize >> 16);
	fileHeader[5] = (unsigned char)(fileSize >> 24);
	fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

	static unsigned char infoHeader[] = {
	   0,0,0,0, /// header size
	   0,0,0,0, /// image width
	   0,0,0,0, /// image height
	   0,0,     /// number of color planes
	   0,0,     /// bits per pixel
	   0,0,0,0, /// compression
	   0,0,0,0, /// image size
	   0,0,0,0, /// horizontal resolution
	   0,0,0,0, /// vertical resolution
	   0,0,0,0, /// colors in color table
	   0,0,0,0, /// important color count
	};

	infoHeader[0] = (unsigned char)(INFO_HEADER_SIZE);
	infoHeader[4] = (unsigned char)(width);
	infoHeader[5] = (unsigned char)(width >> 8);
	infoHeader[6] = (unsigned char)(width >> 16);
	infoHeader[7] = (unsigned char)(width >> 24);
	infoHeader[8] = (unsigned char)(height);
	infoHeader[9] = (unsigned char)(height >> 8);
	infoHeader[10] = (unsigned char)(height >> 16);
	infoHeader[11] = (unsigned char)(height >> 24);
	infoHeader[12] = (unsigned char)(1);
	infoHeader[14] = (unsigned char)(3 * 8);

	FILE* image;

	val = fopen_s(&image, BMPCPY, "wb");

	if (val != 0)
	{
		fwrite(fileHeader, FILE_HEADER_SIZE, 1, image);
		fwrite(infoHeader, INFO_HEADER_SIZE, 1, image);

		for ( i = 0; i < height*width; i++)
		{
			fwrite(matrix[i].color.b, 1, 1, image);
			fwrite(matrix[i].color.g, 1, 1, image);
			fwrite(matrix[i].color.r, 1, 1, image);
		}
		fclose(image);
	}


/*
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
	free(wrmat);*/
//}*/


void Pools(pixmat** mtrx, image_t image, poolList_t* pools){
	int i, j, val;
	int** temp;
	int size;
	pix_t* root = NULL;
	co_t center;
	FILE* file;

	temp = malloc(sizeof(int*) * image.width);
	if (temp)
	{
		for (i = 0;i < image.width;i++) {
			temp[i] = malloc(sizeof(int) * image.height);
		}
	}// allocate memory to temp color signed matrix

	for ( i = 0; i < image.height; i++)
	{
		for (j = 0;j < image.width;j++) {
			if (mtrx[j][i].color.r == 155 && mtrx[j][i].color.g == 190 && mtrx[j][i].color.b == 245)
			{
				temp[j][i] = 1;
			}
			else {
				temp[j][i] = 0;
			}
		}
	}


	for (i = 0;i < image.height;i++) {
		for (j = 0;j < image.width;j++) {
			if (temp[j][i] == 1)
			{
				size = 1;
				temp[j][i] = 0;
				pix_insert(&root, mtrx[j][i].cordinate);
				segment(&root, mtrx, temp, image, i, j, &size);
				//pool_insert(&pools, size, center, &root);//insert segmention function


				if (size > 10)
				{
					printf("NEW POOL\n\n");
					printf("--%d--\n", size);
					center = pool_middle(root, size);
		//			printf("%d,%d,%d\n", mtrx[center.x][center.y].color.r, mtrx[center.x][center.y].color.g, mtrx[center.x][center.y].color.b);
					printf("%d, %d\n", center.x, center.y);
				}
				else
				{
					printf("NOT POOL\n\n");
					printf("--%d--\n", size);
				}
			}
		}
	}

	for (i = 0;i < image.width;i++) {
			free(temp[i]);
	}
	free(temp);
}


int segment(pix_t* root, pixmat** mtrx, int** temp, image_t image, int i, int j, int* size) {

	//pix_insert(root, mtrx[j][i].cordinate);

	if (j > 0) {
		if (temp[j - 1][i] == 1) {
			pix_insert(root, mtrx[j-1][i].cordinate);
			*size += 1;
			temp[j-1][i] = 0;
			segment(root, mtrx, temp, image, i, j - 1, size);
		}
	}

	if (j < image.width - 1) {
		if (temp[j + 1][i] == 1) {
			pix_insert(root, mtrx[j + 1][i].cordinate);
			*size += 1;
			temp[j + 1][i] = 0;
			segment(root, mtrx, temp, image, i, j + 1, size);

		}
	}

	if (i > 0)
	{
		if (temp[j][i - 1] == 1) {
			pix_insert(root, mtrx[j][i - 1].cordinate);
			*size += 1;
			temp[j][i-1] = 0;
			segment(root, mtrx, temp, image, i - 1, j, size);
		}
	}

	if (i < image.height - 1)
	{
		if (temp[j][i + 1] == 1) {
			pix_insert(root, mtrx[j][i + 1].cordinate);
			*size += 1;
			temp[j][i + 1] = 0;
			segment(root, mtrx, temp, image, i + 1, j, size);
		}

	}



	
}


void pix_insert(pix_t** root, co_t coordinate) {
	pix_t* new_pix = malloc(sizeof(pix_t));
	if (new_pix == NULL) {
		exit(1);
	}
	new_pix->next = NULL;
	new_pix->p.x = coordinate.x;
	new_pix->p.y = coordinate.y;

	if (*root == NULL) {
		*root = new_pix;
		return;
	}

	pix_t* curr = *root;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = new_pix;
}

void pool_insert(poolList_t** root, int size, co_t center, pix_t* pix){
	
	poolList_t* new_pool = malloc(sizeof(poolList_t));
	if (new_pool == NULL) {
		exit(1);
	}
	new_pool->next = NULL;
	new_pool->poolCenter.x = 0;
	new_pool->poolCenter.y = 0; // Adapt pool center function to this
	new_pool->size = size;
	new_pool->pix = pix;

	if (*root == NULL) {
		*root = new_pool;
		return;
	}

	poolList_t* curr = *root;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = new_pool;
	}