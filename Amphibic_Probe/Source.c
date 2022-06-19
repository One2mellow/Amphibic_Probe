#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BMP "Crysis.bmp"
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
	pix_t* pix; //list of pool pixels
	struct pool* next;
}poolList_t;


int menu(); //menu function

int LoadImage(image_t* image, const char* filename); //loading the BMP image and getting WxH values

co_t pool_middle(pix_t *root, int size); //returning the pool's center coordinates from given coordinate array

void imgtrx(pixmat** mtrx, image_t image, char* filename); //converting the BMP image to 2d array of type pixmat

poolList_t* Pools(pixmat** mtrx, image_t image, poolList_t* pools); //Creating list of pools which contain size and center co. for each pool

void CreateBMP(char* filename, color_t** matrix, int height, int width); //UNFINISHED! creating BMP of best route

int segment(pix_t* root, pixmat** mtrx, int** temp, image_t image, int i, int j, int* size); //using region base image segmentation to detect pools

void pix_insert(pix_t** root, co_t coordinate); //appending new pixel to the end of pixels list

void pool_insert(poolList_t** root, int size, co_t center, pix_t* pix); //appending new pool element to the end of pools list

void deallocpix(pix_t** root); //deallocating memory of the pixel list

void deallocpool(poolList_t** root); //deallocating memory of the pools list

co_t InputCheck(image_t image); //checking the validity of the starting coordinates

int SpaceMod(int x, int y); //making sure that the correct number of spaces is printed between co. and size in pools.txt


///////////////////////////////////////////function for section 3- START///////////////////////////////////////////

//find distance between to cordinates
double distance(co_t a, co_t b) {
	double x1, x2, y1, y2;
	x1 = a.x, x2 = b.x, y1 = a.y, y2 = b.y;
	double d;
	d = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	return d;
}

//find the closest pool
/*The function receives an array of pools and a current coordinate and
returns the index in the array of the nearest pool.*/
int closest_pool(co_t current_pos, co_t middle_arr[], int size) {
	int i, j = -1;
	double smallest_d = 10000;
	for (i = 0; i < size; i++) {
		if (middle_arr[i].x != 2000)
			if (smallest_d > distance(middle_arr[i], current_pos)) {
				smallest_d = distance(middle_arr[i], current_pos);
				j = i;
			}
	}
	return j;
}


int route_finder(co_t tracker_coordinate, co_t end_coordinate, float oil, float time, int pool_size_arr[], co_t middle_arr[], int size_of_pool, int r) {
	int x = 1;
	r++;
	if (oil >= 0) {
		if ((distance(tracker_coordinate, end_coordinate)) * 0.2 - 0.009 <= oil) {
			float ab = (oil - distance(tracker_coordinate, end_coordinate) * 0.2);
			if (ab < 0)
				ab = ab * -1;
			FILE* best_route;
			fopen_s(&best_route, "best-route.txt", "at");
			if (!best_route) return x;
			fprintf_s(best_route, "(%d) %.2f %.2f %d %d %d\n", r, time + distance(tracker_coordinate, end_coordinate) / 0.2, ab, size_of_pool, end_coordinate.x, end_coordinate.y);
			fclose(best_route);
			char test = 0;
			int i = 0;
			int j = 0;

			return 0;
		}
		else {
			int p1 = closest_pool(tracker_coordinate, middle_arr, sizeof middle_arr);
			if (p1 == -1)return 1;
			co_t temp1 = middle_arr[p1];
			float ab = oil - distance(tracker_coordinate, temp1) * 0.2 + pool_size_arr[p1] * 0.2;
			float bb = time + pool_size_arr[p1] + distance(tracker_coordinate, temp1) / 0.2;

			if ((oil - distance(tracker_coordinate, temp1) * 0.2) > 0) {
				if (middle_arr[p1].x != 2000) {
					FILE* best_route;
					fopen_s(&best_route, "best-route.txt", "at");
					if (!best_route) return x;
					fprintf_s(best_route, "(%d) %.2f %.2f %d %d %d	", r, bb, ab, pool_size_arr[p1], temp1.x, temp1.y);
					fclose(best_route);


				}

				middle_arr[p1].x = 2000;
				middle_arr[p1].y = 2000;
				co_t middle_arr2[5];
				for (int i = 0; i < 5; i++)
					middle_arr2[i] = middle_arr[i];

				x *= route_finder(temp1, end_coordinate, ab, time + pool_size_arr[p1] + distance(tracker_coordinate, temp1) / 0.2, pool_size_arr, middle_arr2, pool_size_arr[p1], r);
			}
			int p2 = closest_pool(tracker_coordinate, middle_arr, sizeof middle_arr);
			if (p2 == -1)return 1;
			co_t temp2 = middle_arr[p2];
			float ac = oil - distance(tracker_coordinate, temp2) * 0.2 + pool_size_arr[p2] * 0.2;
			float aa = time + pool_size_arr[p2] + distance(tracker_coordinate, temp2) / 0.2;

			if ((oil - distance(tracker_coordinate, temp1) * 0.2) > 0) {

				if (middle_arr[p2].x != 2000) {
					FILE* best_route;
					fopen_s(&best_route, "best-route.txt", "at");
					if (!best_route) return x;
					fprintf_s(best_route, "(%d) %.2f %.2f %d %d %d	", r, aa, ac, pool_size_arr[p2], temp2.x, temp2.y);
					fclose(best_route);


				}

				middle_arr[p2].x = 2000;
				middle_arr[p2].y = 2000;
				co_t middle_arr3[5];
				for (int i = 0; i < 5; i++)
					middle_arr3[i] = middle_arr[i];
				x *= route_finder(temp2, end_coordinate, ac, time + pool_size_arr[p2] + distance(tracker_coordinate, temp2) / 0.2, pool_size_arr, middle_arr3, pool_size_arr[p2], r);
			}
		}
	}
	FILE* best_route;
	fopen_s(&best_route, "best-route.txt", "at");
	if (!best_route) return x;
	fprintf_s(best_route, "X\n");
	fclose(best_route);
	return x;
}
///////////////////////////////////////////function for section 3- END///////////////////////////////////////////







int main() {
	int i, j, val, count = 0, choice = 0;
	poolList_t* pools = NULL;
	pixmat** matrix;
	static image_t image;
	FILE* tx;

	val = fopen_s(&tx, TXT, "w");

	if ((LoadImage(&image, BMP)) != 0) {
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

	choice = menu();

	while (choice != 9)
	{
		switch (choice)
		{
		case 1:
			pools = Pools(matrix, image, pools);
			if (pools == NULL) {
				printf_s("\nTotal of 0 pools.\n");
				menu();
				break;
			}
			printf_s("\nCoordinate x1,y1 of the first discoverd pool (%d,%d)", pools->poolCenter.x, pools->poolCenter.y);
			printf_s("\nSize %d",pools->size);
				if (val == 0) {
//					fwrite("Image size (", 1, 12, tx); //initializing pools.txt
					fprintf_s(tx, "%s%dx%d%s", "Image size (", image.width, image.height, ")\nPool Center	Size\n===========	====");
					for (poolList_t* curr = pools; curr != NULL; curr = curr->next) {
						fprintf_s(tx, "\n(%d,%d)", curr->poolCenter.x, curr->poolCenter.y);
						for (i = 0; i < 9 - SpaceMod(curr->poolCenter.x,curr->poolCenter.y); i++)
							fputc(' ', tx);
						fprintf_s(tx, "%d", curr->size);
						count++; //iterating through the pool list, printing size and center
					}
				}
				else
				{
					printf_s("ERROR! couldn't open %s", TXT);
				}

			printf_s("\nTotal of %d pools.\n", count);
			fclose(tx);
			choice = menu();
			break;
		case 2:
			printf("Sorted lists\n");
			choice = menu();
			break;
		case 3:
			putchar('\n');
			InputCheck(image);
			choice = menu();
			break;
		case 4:
			//Naama
			break;
		case 9:
			return 0;
			break;
		default:
			printf("\nBad input, try again\n\n");
			choice = menu();
			break;
		}
	}



	deallocpool(&pools);
	for (i = 0;i < image.width;i++) {
		free(matrix[i]);
	}	
	free(matrix);

	///////////////////////////////////////////section 3 - START///////////////////////////////////////////

	pools;
	char trash;
	co_t end_coordinate = { 0 };
	i = 0;

	char pointer = 0;
	int num_of_pool = 0;
	fopen_s(&pools, "pools.txt", "rt");
	if (!pools) return;
	fseek(pools, 40, SEEK_CUR);
	for (pointer = getc(pools); pointer != EOF; pointer = getc(pools)) {
		if (pointer == '(') {
			num_of_pool++;
		}
	}
	fclose(pools);

	int* pool_size_arr = NULL;
	pool_size_arr = (int*)malloc(num_of_pool * sizeof(int));
	if (pool_size_arr == NULL) {
		exit(0);
	}
	for (i = 0; i < num_of_pool; i++) {
		pool_size_arr[i] = 0;
	}
	i = 0;

	co_t* middle_arr = NULL;
	middle_arr = (co_t*)malloc(num_of_pool * sizeof(co_t));
	if (middle_arr == NULL) {
		exit(0);
	}
	for (i = 0; i < num_of_pool; i++) {
		middle_arr[i].x = 0;
		middle_arr[i].y = 0;
	}
	i = 0;

	fopen_s(&pools, "pools.txt", "rt");
	if (!pools) return;
	fseek(pools, 12, SEEK_SET);
	fscanf_s(pools, "%d %c %d", &end_coordinate.x, &trash, 1, &end_coordinate.y);
	fseek(pools, 40, SEEK_CUR);
	for (i = 0; i < num_of_pool; i++) {
		fscanf_s(pools, "%d %c %d", &middle_arr[i].x, &trash, 1, &middle_arr[i].y);
		fseek(pools, num_of_pool, SEEK_CUR);
		fscanf_s(pools, "%d %c ", &pool_size_arr[i], &trash, 1);
	}
	fclose(pools);

	FILE* best_route;
	fopen_s(&best_route, "best-route.txt", "wt");
	if (!best_route) return;
	fprintf_s(best_route, "");
	fclose(best_route);

	fopen_s(&best_route, "best-route2.txt", "wt");
	if (!best_route) return;
	fprintf_s(best_route, "");
	fclose(best_route);

	co_t current_pos = { 0 };
	float oil;
	printf_s("Please Enter valid x,y start coordinate, bmp width is %d and height is %d\n", end_coordinate.x, end_coordinate.y);
	scanf_s("%d,%d", &current_pos.x, &current_pos.y);

	do {
		printf_s("Please enter valid oil supply in range 1-1000\n");
		scanf_s("%f", &oil);
	} while ((oil < 1) || (oil > 1000));

	int r = 0;
	float time = 0;
	int size_of_pool = 0;

	if (route_finder(current_pos, end_coordinate, oil, time, pool_size_arr, middle_arr, size_of_pool, r) != 0) {
		printf_s("Sorry, could not reach destination with these inputs\n");
		remove("best-route.txt");
		remove("best-route2.txt");
	}
	else {
		char test = 0;
		int k = 0;
		int j = 0;
		i = 0;
		fopen_s(&best_route, "best-route.txt", "rt");
		if (!best_route) return;
		FILE* best_route2;
		fopen_s(&best_route2, "temp.txt", "a");
		if (!best_route2) return;
		for (test = getc(best_route); test != EOF; test = getc(best_route)) {
			if (test != 'X') {
				if (test == '\n') {
					j = i;
					fseek(best_route, -j + k - 2, SEEK_CUR);
					for (test = getc(best_route); k < j + 1; test = getc(best_route)) {
						fprintf_s(best_route2, "%c", test);
						k++;
					}
					k = j + 1;
					fseek(best_route, -2, SEEK_CUR);
					test = getc(best_route);
				}
			}
			else {
				fseek(best_route, -2, SEEK_CUR);
				test = getc(best_route);
				if (test == '\n') {
					k = j + 2;
					fseek(best_route, 1, SEEK_CUR);
					test = getc(best_route);
				}
				else {
					j = i;
					k += 5;
					fseek(best_route, -j + k - 2, SEEK_CUR);
					for (test = getc(best_route); k < j + 3; test = getc(best_route)) {
						fprintf_s(best_route2, "%c", test);
						k++;
					}
					fprintf_s(best_route2, "\n");
					k = j + 1;
				}
			}
			i++;
		}
		fclose(best_route2);
		fclose(best_route);
		remove("best-route2.txt");
		char old_name[] = "temp.txt";
		char new_name[] = "best-route2.txt";
		int d = rename(old_name, new_name);

		k = 0;
		j = 0;
		i = 0;

		fopen_s(&best_route, "best-route2.txt", "rt");
		if (!best_route) return;
		fopen_s(&best_route2, "temp.txt", "a");
		if (!best_route2) return;
		for (test = getc(best_route); test != EOF; test = getc(best_route)) {
			if (test == '\n') {
				j = i;
				fseek(best_route, -3, SEEK_CUR);
				test = getc(best_route);
				if (test != 'X') {
					fseek(best_route, -j + k, SEEK_CUR);
					for (test = getc(best_route); k < j + 1; test = getc(best_route)) {
						fprintf_s(best_route2, "%c", test);
						k++;
					}
					k = j + 1;
					fseek(best_route, -2, SEEK_CUR);
					test = getc(best_route);
				}
				else {
					test = getc(best_route);
					k = j + 1;
				}
			}
			i++;
		}
		fclose(best_route2);
		fclose(best_route);
		remove("best-route2.txt");
		d = rename(old_name, new_name);

		k = 0;
		j = 0;
		i = 0;
		fopen_s(&best_route, "best-route2.txt", "rt");
		if (!best_route) return;
		fopen_s(&best_route2, "temp.txt", "a");
		if (!best_route2) return;
		for (test = getc(best_route); test != EOF; test = getc(best_route)) {
			if (test == '\n') {
				j = i;
				fseek(best_route, k - j - 1, SEEK_CUR);
				test = getc(best_route);
				if (test == '1') {
					fseek(best_route, -2, SEEK_CUR);
					for (test = getc(best_route); k < j + 1; test = getc(best_route)) {
						fprintf_s(best_route2, "%c", test);
						k++;
					}
					fseek(best_route, -2, SEEK_CUR);
					test = getc(best_route);
				}
				else {
					int r = 0;
					int n = 0;
					if (test != "1") {
						char temp = test;
						fseek(best_route, -k - 3, SEEK_CUR);
						for (test = getc(best_route); test != '\n'; test = getc(best_route)) {
							if (test == '(') {
								test = getc(best_route);
								if (test == temp) {
									fseek(best_route, -r - 3, SEEK_CUR);
									for (test = getc(best_route); n < r + 1; test = getc(best_route)) {
										fprintf_s(best_route2, "%c", test);
										n++;
									}
									fseek(best_route, k - r - 1, SEEK_CUR);
									for (test = getc(best_route); k < j + 1; test = getc(best_route)) {
										fprintf_s(best_route2, "%c", test);
										k++;
									}
									fseek(best_route, -2, SEEK_CUR);
								}
							}
							r++;
						}
					}
					r = 0;
					n = 0;
				}
			}
			i++;
		}
		fclose(best_route2);
		fclose(best_route);
		remove("best-route2.txt");
		d = rename(old_name, new_name);

		int counter = 0;
		fopen_s(&best_route, "best-route2.txt", "rt");
		if (!best_route) return;
		for (test = getc(best_route); test != EOF; test = getc(best_route)) {
			if (test == '\n')
				counter++;
		}
		fclose(best_route);

		float* data = NULL;
		data = (float*)malloc(counter * sizeof(float));
		if (data == NULL) {
			exit(0);
		}
		for (i = 0; i < counter; i++) {
			data[i] = 0;
		}
		i = 0;
		j = 0;
		k = 0;
		fopen_s(&best_route, "best-route2.txt", "rt");
		if (!best_route) return;
		for (test = getc(best_route); test != EOF; test = getc(best_route)) {
			if (test == ')')
				j = i;
			if (test == '\n') {

				fseek(best_route, j - i, SEEK_CUR);
				fscanf_s(best_route, "%f", &data[k]);
				k++;
				for (test = getc(best_route); test != '\n'; test = getc(best_route));
			}
			i++;
		}
		fclose(best_route);

		float timeb = 0;
		float oilb = 0;
		int sizeb = 0;
		int xb = 0;
		int yb = 0;
		i = 0;
		j = 0;
		int p = 0;
		float kk = data[0];
		int counter2 = 0;
		int garbi = 0;
		fopen_s(&best_route, "best-route2.txt", "rt");
		if (!best_route) return;
		for (test = getc(best_route); test != EOF; test = getc(best_route)) {
			for (test = getc(best_route); (test != '\n') && (test != EOF); test = getc(best_route)) {
				if (test == ')')
					counter2++;
				j++;
			}
			if (i == counter)
				i--;
			if (kk >= data[i]) {
				kk = data[i];
				fseek(best_route, -j + 1, SEEK_CUR);
				for (p = 0; p < counter2; p++) {

					fscanf_s(best_route, "%f %f %d %d %d ", &timeb, &oilb, &garbi, &xb, &yb);
					fseek(best_route, 3, SEEK_CUR);
					if (p == 0)
						printf_s("Time=0.00 (%d,%d) oil=%.2f", current_pos.x, current_pos.y, oil);
					printf_s(" -> Time=%.2f (%d,%d) oil=%.2f", timeb, xb, yb, oilb);
					if (p == (counter2 - 1)) {
						printf_s("\n");
						fseek(best_route, -4, SEEK_CUR);
						counter2 = 0;
						j = 0;
					}
				}
			}
			i++;
		}
		fclose(best_route);

		kk = data[0];
		i = 0;
		j = 0;
		p = 0;
		int c = 0;
		counter2 = 0;

		remove("best-route.txt");
		fopen_s(&best_route, "best-route2.txt", "rt");
		if (!best_route) return;
		FILE* best_route3;
		fopen_s(&best_route3, "best-route.txt", "at");
		if (!best_route3) return;
		for (i = 0; i < counter; i++)
			if (kk >= data[i]) {
				kk = data[i];
				j++;
			}
		j--;
		for (test = getc(best_route); test != EOF; test = getc(best_route)) {
			if (test == '\n')
				p++;
			if (p == j) {
				for (test = getc(best_route); test != '\n'; test = getc(best_route)) {
					if (test == ')')
						counter2++;
					c++;
				}
				fseek(best_route, -c, SEEK_CUR);
				for (p = 0; p < counter2; p++) {

					fscanf_s(best_route, "%f %f %d %d %d ", &timeb, &oilb, &sizeb, &xb, &yb);
					fseek(best_route, 3, SEEK_CUR);
					if (p == 0) {
						fprintf_s(best_route3, "Best Route	Size\n");
						fprintf_s(best_route3, "(%3d,%3d)	0\n", current_pos.x, current_pos.y);
					}
					else
						fprintf_s(best_route3, "(%3d,%3d)	%d\n", xb, yb, sizeb);
				}
			}
		}
		fprintf_s(best_route3, "(%3d,%3d)	0\n", end_coordinate.x, end_coordinate.y);
		fclose(best_route3);
		fclose(best_route);
		remove("best-route2.txt");
		printf_s("New best-route.txt file was created\n");
		free(data);
	}
	free(pool_size_arr);
	free(middle_arr);
	return 0;

	///////////////////////////////////////////section 3 - END///////////////////////////////////////////








	return 0;
}


int menu() {
	int choice;
	printf_s("--------------------------\nME LAB services\n--------------------------");

	printf_s("\nMenu:\n1. Scan pools\n2. Print sorted pool list\n3. Select route\n4. Numeric report.\n5. Students addition\n9. Exit\nEnter choice: ");

	scanf_s("%d", &choice);
	getchar();
	return choice;
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
int LoadImage(image_t* image, const char* filename) {
	int return_value = 0;

	unsigned int image_data_address;
	int width;
	int height;
	int bpp;

	printf_s("Loading bitmap file: %s\n", filename);

	FILE* file;
	return_value = fopen_s(&file, filename, "rb");
	if (file) {
		if (fgetc(file) == 'B' && fgetc(file) == 'M') {
			printf_s("BM read; bitmap file confirmed.\n");
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
		printf_s("(%s) Failed to open file\n", filename);
		return_value = NULL;
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


poolList_t* Pools(pixmat** mtrx, image_t image, poolList_t* pools){
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

				if (size > 10)
				{
					center = pool_middle(root, size);
					pool_insert(&pools, size, center, &root);
				}
			}
			deallocpix(&root);
		}
	}




	for (i = 0;i < image.width;i++) {
			free(temp[i]);
	}
	free(temp);

	return pools;
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
	new_pool->poolCenter.x = center.x;
	new_pool->poolCenter.y = center.y;
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

void deallocpix(pix_t** root) {
	pix_t* curr = *root;
	while (curr != NULL) {
		pix_t* aux = curr;
		curr = curr->next;
		free(aux);
	}
	*root = NULL;
}

void deallocpool(poolList_t** root) {
	poolList_t* curr = *root;
	while (curr != NULL)
	{
		poolList_t* aux = curr;
		curr = curr->next;
		free(aux);
	}
	*root = NULL;
}


co_t InputCheck(image_t image) {
	co_t coordinate;
	char *input;

	do
	{
		char x[4], y[4];
		int i, j = 0;
		int flag = 0;

		input = malloc(sizeof(char) * image.width);
		printf_s("Please Enter valid x,y start coordinate, bmp width is %d and height is %d\n", image.width, image.height);
		gets_s(input, image.width);


		for (i = 0; input[i] != '\0'; i++)
		{
			if (input[i] < '0' || input[i] > '9')
				if (input[i] != ',' && input[i] != ' ')
				{
					free(input);
					input = malloc(sizeof(char) * image.width);
					printf_s("Please Enter valid x,y start coordinate, bmp width is %d and height is %d\n", image.width, image.height);
					gets_s(input, image.width);
					i = 0;
				}
		}

		for (i = 0; input[i] != '\0'; i++)
		{
			if (input[i] == ',') {
				flag = 1;
				i++;
			}
			if (flag == 0)
			{
				x[i] = input[i];
			}
			else {
				y[j] = input[i];
				j++;
			}
		}

		coordinate.x = (int)atof(x);
		coordinate.y = (int)atof(y);

	} while (coordinate.x > image.width || coordinate.y > image.height);

	free(input);

	return coordinate;
}

int SpaceMod(int x, int y) {
	int n;
	int space = 0;
	n = x;
	while (n != 0)
	{
		n = n / 10;
		space++;
	}
	n = y;
	while (n != 0)
	{
		n = n / 10;
		space++;
	}

	return space;
}