#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BMP "fishpool-another-ex1.bmp"
#define BMPCPY "fishpool-copy.bmp"
#define TXT "pools.txt"
#define BEST_TXT "best-route.txt"
#define Special "for-nitai.txt"


typedef struct { //bmp file values struct
	int width;
	int height;
	unsigned char header[54];
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

typedef struct lists {//route data
	double time;
	double oil;
	int size;
	int x;
	int y;
	struct lists* next;
} list_t;

typedef struct cot_list { //list of coordinate sequence (moving path)
	co_t coordinate;
	struct cot_list* next;
} cot_list;

typedef struct pools_CoordinatesAndSize {
	int centerX;
	int centerY;
	int poolsize; //the number of pixels that combine the pool
	struct pools_CoordinatesAndSize* next;
}printing_t;

int menu(); //menu function

int LoadImage(image_t* image, const char* filename, unsigned char* head); //loading the BMP image and getting WxH values

co_t pool_middle(pix_t* root, int size); //returning the pool's center coordinates from given coordinate array

void imgtrx(pixmat** mtrx, image_t image, char* filename, int width_flag); //converting the BMP image to 2d array of type pixmat

poolList_t* Pools(pixmat** mtrx, image_t image, poolList_t* pools); //Creating list of pools which contain size and center co. for each pool

void CreateBMP(char* filename,char* txt, pixmat** matrix, int height, int width, unsigned char* header, int width_flag); // Print the route on the bmp copy

void segment(pix_t* root, pixmat** mtrx, int** temp, image_t image, int i, int j, int* size); //using region base image segmentation to detect pools

void pix_insert(pix_t** root, co_t coordinate); //appending new pixel to the end of pixels list

void pool_insert(poolList_t** root, int size, co_t center, pix_t* pix); //appending new pool element to the end of pools list

void coordinat_insert(cot_list** root, co_t coordinate); //appending new coordinate element to the end of coordinate list

void deallocpix(pix_t** root); //deallocating memory of the pixel list

void deallocpool(poolList_t** root); //deallocating memory of the pools list

void dealloccoordinate(cot_list** root);

int SpaceMod(int x, int y); //making sure that the correct number of spaces is printed between co. and size in pools.txt

void RoutePainter(pixmat** matrix, int x, int y, int x_final, int y_final, int height, int width, int width_flag);

co_t best_co(FILE* route); //extrcats coordinates from best route file

void printNsortpools();

printing_t* pools_sortingNinsert(printing_t* head, int coordinate_x, int coordinate_y, int poooolsize);

void time2glow(char* filename, pixmat** matrix, image_t image);

void section_3();

co_t InputCheck(co_t image); //checking the validity of the starting coordinates

void there_a_route(double oil, co_t current_pos, co_t end_coordinate); //All the functions that print to screen and file together

void max_oil_file_creation(int i, int counter, double data[], double kk, int j, char test, int p, int counter2, int c, double timeb, double oilb, int sizeb, int xb, int yb, co_t current_pos, co_t end_coordinate, double oil);
//Create a data file of the route that has the most oil left

void best_route_file_creation(int i, int counter, double data[], double kk, int j, char test, int p, int counter2, int c, double timeb, double oilb, int sizeb, int xb, int yb, co_t current_pos, co_t end_coordinate);
//Create a data file of the fastest route

void printing_to_screen(char test, int counter2, int j, int i, int p, int counter, double kk, double data[], double oil, co_t current_pos, double timeb, double oilb, int garbi, int xb, int yb);
//Print data to the screen from the linked list

void freeList(list_t* head); //Release memory in the linked list

list_t* add(double time, double oil, int size, int x, int y, list_t* head); //Add a node to the linked list

int print(list_t* head, int i, int counter2); //Print the data to the screen

double* set_finel_oil_arr(char test, int i, int j, int k, double* data); //Find final oil data from the file

double* set_finel_time_arr(char test, int i, int j, int k, double* data); //Allocation of memory to data from a file

double* malloc_data(int counter); //Allocation of memory to data from a file

int set_counter(); //Count lines in file

void correct_data3(char test, int i, int j, int k, int r, int n); //Corrects the data files

void reducing_correct_data3(FILE* best_route, FILE* best_route2, char test, int i, int j, int k, int r, int n); //Corrects the data files

int reducing3_correct_data3(FILE* best_route, FILE* best_route2, char test, int r, int k, int j); //Corrects the data files

int reducing2_correct_data3(FILE* best_route, FILE* best_route2, char test, int r, int n); //Corrects the data files

void correct_data2(int i, int j, int k, char test); //Corrects the data files

void correct_data1(int i, int j, int k, char test); //Corrects the data files

void reducing_correct_data1(FILE* best_route, FILE* best_route2, int i, int j, int k, char test); //Corrects the data files

void file_name_changer(); //Renames a file

double oil_input(); //Obtaining oil data

void reset_files(); //Files boot

co_t* middle_arr_malloc(int num_of_pool); //Allocation of memory to the set of pool centers

int* pool_size_arr_malloc(int num_of_pool); //Allocation of memory to the array of pool sizes

int pool_counter(); //Counting pools from a file

int route_finder(co_t tracker_coordinate, co_t end_coordinate, double oil, double time, int pool_size_arr[], co_t middle_arr[], int size_of_pool, int r, int num_of_pool, int x); //Finding all the tracker routes

co_t* reducing_route_finder6(int num_of_pool); //Allocation of memory to a variable

double reducing_route_finder5(double oil, co_t tracker_coordinate, co_t end_coordinate); //Fixing a math problem when the oil is almost exactly right

int reducing_route_finder4(int x); //Print x when the tracker runs out of oil

int reducing_route_finder2(co_t middle_arr[], int p1, int x, int r, double bb, double ab, int pool_size_arr[], co_t temp1); //Print tracker data to a file

int reducing_route_finder(int x, int r, double time, co_t tracker_coordinate, co_t end_coordinate, double ab, int size_of_pool); //Print tracker data to a file

int closest_pool(co_t current_pos, co_t middle_arr[], int size); //find the closest pool
/*The function receives an array of pools and a current coordinate and
returns the index in the array of the nearest pool.*/

double distance(co_t a, co_t b); //find distance between to cordinates




int main() {
	int i, val, count = 0, choice = 0, width_flag = 0;
	poolList_t* pools = NULL;
	pixmat** matrix;
	static image_t image;
	FILE* tx;

	val = fopen_s(&tx, TXT, "w");
	if (!tx) return 0;

	if ((LoadImage(&image, BMP, image.header)) != 0) {
		printf_s("Failed to load file: \" %s\"", BMP);
		return -1;
	}

	if (image.width % 4 != 0) // making sure width is divsible by 4 due to bmp regulations
	{
		width_flag = image.width;
		image.width = image.width + 4 - (image.width % 4);
	}

	matrix = malloc(sizeof(pixmat*) * image.width);
	if (matrix)
	{
		for (i = 0;i < image.width;i++) {
			matrix[i] = malloc(sizeof(pixmat) * image.height);
		}
	}// allocate memory to image pixel matrix

	imgtrx(matrix, image, BMP, width_flag);

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
			printf_s("\nSize %d", pools->size);
			if (val == 0) {
				fprintf_s(tx, "%s%dx%d%s", "Image size (", image.width, image.height, ")\nPool Center	Size\n===========	====");
				for (poolList_t* curr = pools; curr != NULL; curr = curr->next) {
					fprintf_s(tx, "\n(%d,%d)", curr->poolCenter.x, curr->poolCenter.y);
					for (i = 0; i < 9 - SpaceMod(curr->poolCenter.x, curr->poolCenter.y); i++)
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
			printNsortpools();
			choice = menu();
			break;
		case 3:
			putchar('\n');
			section_3();
			CreateBMP(BMPCPY, BEST_TXT , matrix, image.height, image.width, image.header, width_flag);
			choice = menu();
			break;
		case 4:
			//Naama
			break;
		case 5:
			time2glow(Special, matrix, image);
			choice = menu();
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

	return 0;
}

int menu() {
	int choice;
	int enter;
	printf_s("--------------------------\nME LAB services\n--------------------------");

	printf_s("\nMenu:\n1. Scan pools\n2. Print sorted pool list\n3. Select route\n4. Numeric report.\n5. Students addition\n9. Exit\nEnter choice: ");

	scanf_s("%d", &choice);
	enter = getchar();
	return choice;
}

co_t pool_middle(pix_t* root, int size) {

	int x_max, x_min, y_max, y_min, i;
	co_t middle = { 0 };
	pix_t* curr;
	co_t* pixels;

	pixels = malloc(sizeof(co_t) * size + 1);
	if (!pixels) return middle;

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

int LoadImage(image_t* image, const char* filename, unsigned char* head) {
	int return_value = 0;
	unsigned int image_data_address;
	int width;
	int height;
	int bpp;

	FILE* file;
	return_value = fopen_s(&file, filename, "rb");
	if (file) {
		if (fgetc(file) == 'B' && fgetc(file) == 'M') {
			fseek(file, 8, SEEK_CUR);
			fread(&image_data_address, 4, 1, file);;
			fseek(file, 4, SEEK_CUR);
			fread(&width, 4, 1, file);
			fread(&height, 4, 1, file);
			fseek(file, 2, SEEK_CUR);
			fread(&bpp, 4, 1, file);
			image->width = width;
			image->height = height;
			fseek(file, 0, SEEK_SET);
			for (int i = 0;i < 54;i++) {
				image->header[i] = fgetc(file);
			}
		}
		fclose(file);
	}
	else {
		printf_s("(%s) Failed to open file\n", filename);
		return_value = 0;
	}
	return return_value;
}

void imgtrx(pixmat** mtrx, image_t image, char* filename, int width_flag) {
	int val, i = 0, j;
	FILE* file;
	val = fopen_s(&file, filename, "rb");
	if (!file) {
		printf_s("Error open the fishpool.bmp\n");
		return;
	}

	if (file != 0)
	{
		fseek(file, 54, SEEK_SET);
		for (i = 0; i < image.height; i++)
		{
			for (j = 0; j < width_flag; j++)
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
	fclose(file);
	return;
}

void CreateBMP(char* filename, char* txt, pixmat** matrix, int height, int width, unsigned char* header, int width_flag) {
	FILE* image, * route;
	co_t start, end;
	char position;
	int i, j;
	fopen_s(&image, filename, "wb");
	fopen_s(&route, txt, "rt");

	if (image != 0 && route != 0) {

		position = fgetc(route);
		fseek(route, 15, SEEK_SET);
		start = best_co(route);
		do {
			position = fgetc(route);
			while (position != '(')
				position = fgetc(route);
			fseek(route, -1, SEEK_CUR);
			end = best_co(route);
			RoutePainter(matrix, start.x, start.y, end.x, end.y, height, width, width_flag);
			start = end;
		} while (end.x != width && end.y != height);
		for (int i = 0; i < 54; i++)
		{
			fputc(header[i], image);
		}
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width - 2; j++)
			{
				fputc(matrix[j][i].color.b, image);
				fputc(matrix[j][i].color.g, image);
				fputc(matrix[j][i].color.r, image);
			}
		}
	}

	else return;
	fclose(image);
	fclose(route);
}

poolList_t* Pools(pixmat** mtrx, image_t image, poolList_t* pools) {
	int i, j;
	int** temp;
	int size;
	pix_t* root = NULL;
	co_t center;

	temp = malloc(sizeof(int*) * image.width);
	if (temp)
	{
		for (i = 0;i < image.width;i++) {
			temp[i] = malloc(sizeof(int) * image.height);
		}
	}// allocate memory to temp color signed matrix

	for (i = 0; i < image.height; i++)
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
				segment(root, mtrx, temp, image, i, j, &size);

				if (size > 10)
				{
					center = pool_middle(root, size);
					pool_insert(&pools, size, center, root);
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

void segment(pix_t* root, pixmat** mtrx, int** temp, image_t image, int i, int j, int* size) {

	//pix_insert(root, mtrx[j][i].cordinate);

	if (j > 0) {
		if (temp[j - 1][i] == 1) {
			pix_insert(&root, mtrx[j - 1][i].cordinate);
			*size += 1;
			temp[j - 1][i] = 0;
			segment(root, mtrx, temp, image, i, j - 1, size);
		}
	}

	if (j < image.width - 1) {
		if (temp[j + 1][i] == 1) {
			pix_insert(&root, mtrx[j + 1][i].cordinate);
			*size += 1;
			temp[j + 1][i] = 0;
			segment(root, mtrx, temp, image, i, j + 1, size);

		}
	}

	if (i > 0)
	{
		if (temp[j][i - 1] == 1) {
			pix_insert(&root, mtrx[j][i - 1].cordinate);
			*size += 1;
			temp[j][i - 1] = 0;
			segment(root, mtrx, temp, image, i - 1, j, size);
		}
	}

	if (i < image.height - 1)
	{
		if (temp[j][i + 1] == 1) {
			pix_insert(&root, mtrx[j][i + 1].cordinate);
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

void pool_insert(poolList_t** root, int size, co_t center, pix_t* pix) {

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

void RoutePainter(pixmat** matrix, int x, int y, int x_final, int y_final, int height, int width, int width_flag) {
	int  j = 0, i = 0;
	float movratio, b;
	matrix[x][y].color.r = 250; matrix[x][y].color.g = 180; matrix[x][y].color.b = 30; //color pixel at the beggining
	movratio = ((float)y_final - (float)y) / ((float)x_final - (float)x);
	b = (float)y - (float)(movratio * x);
	x++;y++;

	if (width_flag != 0) {
		width = width_flag;
		x_final = width_flag;
	}

	for (x; x < x_final && x < width && y < y_final && y < height; x++)
	{
		y = (int)((x * movratio) + b);
		matrix[x][y].color.r = 100; matrix[x][y].color.g = 30; matrix[x][y].color.b = 232;
	}
	matrix[width - 1][height - 1].color.r = 250; matrix[width - 1][height - 1].color.g = 180; matrix[width - 1][height - 1].color.b = 30; //color pixel at the end
}

void coordinat_insert(cot_list** root, co_t coordinate) {
	cot_list* new_co = malloc(sizeof(cot_list));
	if (new_co == NULL) {
		exit(1);
	}
	new_co->next = NULL;
	new_co->coordinate.x = coordinate.x;
	new_co->coordinate.y = coordinate.y;

	if (*root == NULL) {
		*root = new_co;
		return;
	}

	cot_list* curr = *root;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = new_co;
}

void dealloccoordinate(cot_list** root) {
	cot_list* curr = *root;
	while (curr != NULL)
	{
		cot_list* aux = curr;
		curr = curr->next;
		free(aux);
	}
	*root = NULL;
}

co_t best_co(FILE* route) {
	int flag = 0, j = 0;
	char temp[10], tmpx[3] = { 0 }, tmpy[3] = { 0 };
	co_t coordinate = { 0 };

	if (route != 0) {

		fread(&temp, 1, 10, route);

		for (int i = 0; i < 9; i++)
		{
			if (temp[i] == ',') {
				flag = 1;
				j = 0;
			}
			if (flag == 1)
			{
				if (temp[i] >= '0' && temp[i] <= '9') {
					tmpy[j] = temp[i];
					j++;
				}

			}
			else
			{
				if (temp[i] >= '0' && temp[i] <= '9') {
					tmpx[j] = temp[i];
					j++;
				}
			}
		}
		coordinate.x = (int)atof(tmpx);
		coordinate.y = (int)atof(tmpy);
	}
	return coordinate;
}

void printNsortpools() {

	FILE* f;
	printing_t* head = malloc(sizeof(printing_t));
	//printing_t* head = NULL;
	int a = fopen_s(&f, "pools.txt", "rt");
	int i; int k = 0; int j = 0; int  count_pools = -3; ; int flag = 0; int l = 0;
	int coordinate_x, coordinate_y, poooolsize;
	char filechar[22]; char chr;
	if (f == NULL) {
		printf_s("Problem reading pools.txt file\nList is empty"); //if there is no such a file  //failed to open pools.txt
		return 0;
	}
	if (a == 0) // open successfully
	{
		chr = fgetc(f);
		while (chr != EOF)
		{
			if (chr == '\n')
			{
				count_pools = count_pools + 1; //counter pools
			}
			chr = fgetc(f);
		}
		if (count_pools == 0) { printf_s("List is empty"); } //if there are no pools
		if (count_pools > 0)
		{
			printf_s("\nSorted pools by size:\nCoordinate      Size\n==========  \t====\n");

			for (l = 0; l < count_pools; l++)
			{

				char str[21], x[3], y[3], size[21];
				for (i = 0; i <= strlen(str); i++)
				{
					str[i] = 0;
				}
				char* strpointer = str;
				/*fgets(str,1, f);
				fgets(str, 1, f);
				fgets(str, 1, f);*/
				fseek(f, 56 + l * 13, SEEK_SET);
				fgets(strpointer, 19, f);
				j = 0; k = 0;
				for (i = 0; i <= strlen(str); i++)
				{

					if (str[i] != ',' && flag == 0)
					{
						x[i] = str[i + 1];
					}
					if (str[i] == ',')
					{
						flag = 1;
					}
					if (str[i] != ')' && flag == 1)
					{
						y[j] = str[i + 1];
						j++;

					}
					if (str[i] == ')') { flag = 2; i++; }
					if (flag == 2)
					{
						size[k] = str[i + 1];
						k++;
					}
				}
				flag = 0;

				poooolsize = (int)atof(size);
				coordinate_x = (int)atof(x);
				coordinate_y = (int)atof(y);

				pools_sortingNinsert(head, coordinate_x, coordinate_y, poooolsize);
			}

		}
	}
	fclose(f);
	/*free(head);*/
}

printing_t* pools_sortingNinsert(printing_t* head, int coordinate_x, int coordinate_y, int poooolsize) {

	printing_t* temp = head;
	printing_t* root = malloc(sizeof(printing_t));
	printing_t* newNode = malloc(sizeof(printing_t));
	newNode->centerX = coordinate_x;
	newNode->centerY = coordinate_y;
	newNode->poolsize = poooolsize;

	if (!head) // empty list_t
		return newNode;
	if (poooolsize < root->poolsize) {
		while (root->next && poooolsize < root->next->poolsize)
			root = root->next;
		newNode->next = root->next;
		root->next = newNode;
	}
	else { // Place at beginning of list_t
		newNode->next = head;
		head = newNode;
	}
	printf_s("(%3d,%3d)  \t%d \n", head->centerX, head->centerY, head->poolsize);

	return head;
}

void time2glow(char* filename, pixmat** matrix, image_t image) {
	FILE* file;
	list_t* root = NULL;
	char pos;
	float time, fuel;
	int size, x, y, i = 0;
	fopen_s(&file, Special, "rt");

	if (file != 0)
	{
		pos = fgetc(file);
		fseek(file, 0, SEEK_SET);
		while (pos != EOF) {
			fscanf_s(file, "%f %f %d %d %d", &time, &fuel, &size, &x, &y);
			add((double)time, (double)fuel, size, x, y, root);
			pos = fgetc(file);
			for (i; pos != '>' && pos != EOF; i++)
				pos = fgetc(file);
		}

		//iterating through the list
		for (list_t* curr = root; curr != NULL; curr = curr->next) {
			//CreateBMP("Most_fuel.bmp","Most_fuel.txt", matrix, image.height, image.width, image.header);
		}

		freeList(root);
	}
}

///////////////////////////////////////////function for section 3- START///////////////////////////////////////////

double distance(co_t a, co_t b) {
	double x1, x2, y1, y2;
	x1 = a.x, x2 = b.x, y1 = a.y, y2 = b.y;
	double d;
	d = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	return d;
}

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

int reducing_route_finder(int x, int r, double time, co_t tracker_coordinate, co_t end_coordinate, double ab, int size_of_pool) {
	FILE* best_route;
	fopen_s(&best_route, "best-route.txt", "at");
	if (!best_route) return x;
	fprintf_s(best_route, "(%d) %.2lf %.2lf %d %d %d\n", r, time + distance(tracker_coordinate, end_coordinate) / 0.2, ab, size_of_pool, end_coordinate.x, end_coordinate.y);
	fclose(best_route);
	return x;
}

int reducing_route_finder2(co_t middle_arr[], int p1, int x, int r, double bb, double ab, int pool_size_arr[], co_t temp1) {
	if (middle_arr[p1].x != 2000) {
		FILE* best_route;
		fopen_s(&best_route, "best-route.txt", "at");
		if (!best_route) return x;
		fprintf_s(best_route, "(%d) %.2lf %.2lf %d %d %d	", r, bb, ab, pool_size_arr[p1], temp1.x, temp1.y);
		fclose(best_route);
	}
	return x;
}

int reducing_route_finder4(int x) {
	FILE* best_route;
	fopen_s(&best_route, "best-route.txt", "at");
	if (!best_route) return x;
	fprintf_s(best_route, "X\n");
	fclose(best_route);
	return x;
}

double reducing_route_finder5(double oil, co_t tracker_coordinate, co_t end_coordinate) {
	double ab = (oil - distance(tracker_coordinate, end_coordinate) * 0.2);
	if (ab < 0)
		ab = ab * -1;
	return ab;
}

co_t* reducing_route_finder6(int num_of_pool) {
	co_t* middle_arr2 = NULL;
	middle_arr2 = (co_t*)malloc(num_of_pool * sizeof(co_t));
	if (middle_arr2 == NULL) {
		exit(0);
	}
	return middle_arr2;
}

int route_finder(co_t tracker_coordinate, co_t end_coordinate, double oil, double time, int pool_size_arr[], co_t middle_arr[], int size_of_pool, int r, int num_of_pool, int x) {
	r++;
	if (oil >= 0) {
		if ((distance(tracker_coordinate, end_coordinate)) * 0.2 - 0.009 <= oil) {
			double ab = reducing_route_finder5(oil, tracker_coordinate, end_coordinate);
			reducing_route_finder(x, r, time, tracker_coordinate, end_coordinate, ab, size_of_pool);
			return 0;
		}
		else {
			int p1 = closest_pool(tracker_coordinate, middle_arr, num_of_pool);
			if (p1 == -1)return 1;
			co_t temp1 = middle_arr[p1];
			double ab = oil - distance(tracker_coordinate, temp1) * 0.2 + pool_size_arr[p1] * 0.2;
			double bb = time + pool_size_arr[p1] + distance(tracker_coordinate, temp1) / 0.2;
			if ((oil - distance(tracker_coordinate, temp1) * 0.2) > 0) {
				reducing_route_finder2(middle_arr, p1, x, r, bb, ab, pool_size_arr, temp1);
				middle_arr[p1].x = 2000;
				co_t* middle_arr2 = reducing_route_finder6(num_of_pool);
				for (int i = 0; i < num_of_pool; i++)
					middle_arr2[i] = middle_arr[i];
				x *= route_finder(temp1, end_coordinate, ab, time + pool_size_arr[p1] + distance(tracker_coordinate, temp1) / 0.2, pool_size_arr, middle_arr2, pool_size_arr[p1], r, num_of_pool, 1);
			}
			int p2 = closest_pool(tracker_coordinate, middle_arr, num_of_pool);
			if (p2 == -1)return 1;
			co_t temp2 = middle_arr[p2];
			double ac = oil - distance(tracker_coordinate, temp2) * 0.2 + pool_size_arr[p2] * 0.2;
			double aa = time + pool_size_arr[p2] + distance(tracker_coordinate, temp2) / 0.2;
			if ((oil - distance(tracker_coordinate, temp2) * 0.2) > 0) {
				reducing_route_finder2(middle_arr, p2, x, r, aa, ac, pool_size_arr, temp2);
				middle_arr[p2].x = 2000;
				co_t* middle_arr3 = reducing_route_finder6(num_of_pool);
				for (int i = 0; i < num_of_pool; i++)
					middle_arr3[i] = middle_arr[i];
				x *= route_finder(temp2, end_coordinate, ac, aa, pool_size_arr, middle_arr3, pool_size_arr[p2], r, num_of_pool, 1);
			}
		}
	}
	reducing_route_finder4(x);
	return x;
}

int pool_counter() {
	char pointer = 0;
	int num_of_pool = 0;
	FILE* pools;
	fopen_s(&pools, "pools.txt", "rt");
	if (!pools) return 0;
	fseek(pools, 40, SEEK_CUR);
	for (pointer = getc(pools); pointer != EOF; pointer = getc(pools)) {
		if (pointer == '(') {
			num_of_pool++;
		}
	}
	fclose(pools);
	return num_of_pool;
}

int* pool_size_arr_malloc(int num_of_pool) {
	int* pool_size_arr = NULL;
	pool_size_arr = (int*)malloc(num_of_pool * sizeof(int));
	if (pool_size_arr == NULL) {
		exit(0);
	}
	return pool_size_arr;
}

co_t* middle_arr_malloc(int num_of_pool) {
	co_t* middle_arr = NULL;
	middle_arr = (co_t*)malloc(num_of_pool * sizeof(co_t));
	if (middle_arr == NULL) {
		exit(0);
	}
	return middle_arr;
}

void reset_files() {
	FILE* best_route;
	fopen_s(&best_route, "best-route.txt", "wt");
	if (!best_route) return;
	fprintf_s(best_route, "");
	fclose(best_route);

	fopen_s(&best_route, "best-route2.txt", "wt");
	if (!best_route) return;
	fprintf_s(best_route, "");
	fclose(best_route);
}

double oil_input() {
	double oil;
	do {
		printf_s("Please enter valid oil supply in range 1-1000\n");
		scanf_s("%lf", &oil);
	} while ((oil < 1) || (oil > 1000));
	return oil;
}

void file_name_changer() {
	remove("best-route2.txt");
	char old_name[] = "temp.txt";
	char new_name[] = "best-route2.txt";
	int d = rename(old_name, new_name);
}

void reducing_correct_data1(FILE* best_route, FILE* best_route2, int i, int j, int k, char test) {
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
}

void correct_data1(int i, int j, int k, char test) {
	FILE* best_route;
	fopen_s(&best_route, "best-route.txt", "rt");
	if (!best_route) return;
	FILE* best_route2;
	fopen_s(&best_route2, "temp.txt", "a");
	if (!best_route2) return;
	reducing_correct_data1(best_route, best_route2, i, j, k, test);
	fclose(best_route2);
	fclose(best_route);
	file_name_changer();
}

void correct_data2(int i, int j, int k, char test) {
	FILE* best_route;
	FILE* best_route2;
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
	file_name_changer();
}

int reducing2_correct_data3(FILE* best_route, FILE* best_route2, char test, int r, int n) {
	fseek(best_route, -r - 3, SEEK_CUR);
	for (test = getc(best_route); n < r + 1; test = getc(best_route)) {
		fprintf_s(best_route2, "%c", test);
		n++;
	}
	return n;
}

int reducing3_correct_data3(FILE* best_route, FILE* best_route2, char test, int r, int k, int j) {
	fseek(best_route, k - r - 1, SEEK_CUR);
	for (test = getc(best_route); k < j + 1; test = getc(best_route)) {
		fprintf_s(best_route2, "%c", test);
		k++;
	}
	fseek(best_route, -2, SEEK_CUR);
	return k;
}

void reducing_correct_data3(FILE* best_route, FILE* best_route2, char test, int i, int j, int k, int r, int n) {
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		if (test == '\n') {
			j = i;
			fseek(best_route, k - j - 1, SEEK_CUR);
			test = getc(best_route);
			if (test == 49) {
				fseek(best_route, -2, SEEK_CUR);
				for (test = getc(best_route); k < j + 1; test = getc(best_route)) {
					fprintf_s(best_route2, "%c", test);
					k++;
				}
				fseek(best_route, -2, SEEK_CUR);
				test = getc(best_route);
			}
			else {
				if (test != 49) {
					char temp = test;
					fseek(best_route, -k - 3, SEEK_CUR);
					for (test = getc(best_route); test != '\n'; test = getc(best_route)) {
						if (test == '(') {
							test = getc(best_route);
							if (test == temp) {
								n = reducing2_correct_data3(best_route, best_route2, test, r, n);
								k = reducing3_correct_data3(best_route, best_route2, test, r, k, j);
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
}

void correct_data3(char test, int i, int j, int k, int r, int n) {
	FILE* best_route;
	FILE* best_route2;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return;
	fopen_s(&best_route2, "temp.txt", "a");
	if (!best_route2) return;
	reducing_correct_data3(best_route, best_route2, test, i, j, k, r, n);
	fclose(best_route2);
	fclose(best_route);
	file_name_changer();
}

int set_counter() {
	char test = 0;
	int counter = 0;
	FILE* best_route;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return 0;
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		if (test == '\n')
			counter++;
	}
	fclose(best_route);
	return counter;
}

double* malloc_data(int counter) {
	double* data = NULL;
	data = (double*)malloc(counter * sizeof(double));
	if (data == NULL) {
		exit(0);
	}
	return data;
}

double* set_finel_time_arr(char test, int i, int j, int k, double* data) {
	FILE* best_route;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return 0;
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		if (test == ')')
			j = i;
		if (test == '\n') {
			fseek(best_route, j - i, SEEK_CUR);
			fscanf_s(best_route, "%lf", &data[k]);
			k++;
			for (test = getc(best_route); test != '\n'; test = getc(best_route));
		}
		i++;
	}
	fclose(best_route);
	return data;
}

double* set_finel_oil_arr(char test, int i, int j, int k, double* data) {
	double trash;
	FILE* best_route;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return 0;
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		if (test == ')')
			j = i;
		if (test == '\n') {
			fseek(best_route, j - i, SEEK_CUR);
			fscanf_s(best_route, "%lf %lf", &trash, &data[k]);
			k++;
			for (test = getc(best_route); test != '\n'; test = getc(best_route));
		}
		i++;
	}
	fclose(best_route);
	return data;
}

int print(list_t* head, int i, int counter2) {
	if (head == NULL)
		return i;
	i = print(head->next, i, counter2);
	if (i == 0) {
		printf_s("Time=%.2lf (%d,%d) oil=%.2lf", head->time, head->x, head->y, head->oil);
		i++;
	}
	else {
		printf_s(" -> Time=%.2lf (%d,%d) oil=%.2lf", head->time, head->x, head->y, head->oil);
		i++;
	}
	return i;
}

list_t* add(double time, double oil, int size, int x, int y, list_t* head) {
	list_t* new_node = NULL;
	new_node = (list_t*)malloc(sizeof(list_t));
	if (new_node == NULL) {
		exit(0);
	}
	new_node->time = time;
	new_node->oil = oil;
	new_node->size = size;
	new_node->x = x;
	new_node->y = y;
	new_node->next = head;
	head = new_node;
	return head;
}

void freeList(list_t* head) {
	list_t* tmp;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

void printing_to_screen(char test, int counter2, int j, int i, int p, int counter, double kk, double data[], double oil, co_t current_pos, double timeb, double oilb, int garbi, int xb, int yb) {
	FILE* best_route;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return;
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		for (test = getc(best_route); (test != '\n') && (test != EOF); test = getc(best_route)) {
			if (test == ')')
				counter2++;
			j++;
		}
		if (test == EOF)
			break;
		if (i == counter)
			i--;
		if (kk >= data[i]) {
			kk = data[i];
			fseek(best_route, -j + 1, SEEK_CUR);
			list_t* head = NULL;
			head = add(0, oil, 0, current_pos.x, current_pos.y, head);
			for (p = 0; p < counter2; p++) {
				fscanf_s(best_route, "%lf %lf %d %d %d ", &timeb, &oilb, &garbi, &xb, &yb);
				head = add(timeb, oilb, garbi, xb, yb, head);
				fseek(best_route, 3, SEEK_CUR);
				if (p == (counter2 - 1)) {
					fseek(best_route, -4, SEEK_CUR);
					counter2 = 0;
					j = 0;
				}
			}
			print(head, 0, counter2);
			printf_s("\n");
			freeList(head);
		}
		i++;
	}
	fclose(best_route);
}

void best_route_file_creation(int i, int counter, double data[], double kk, int j, char test, int p, int counter2, int c, double timeb, double oilb, int sizeb, int xb, int yb, co_t current_pos, co_t end_coordinate) {
	FILE* best_route;
	FILE* best_route3;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return;
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
				fscanf_s(best_route, "%lf %lf %d %d %d ", &timeb, &oilb, &sizeb, &xb, &yb);
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
}

void max_oil_file_creation(int i, int counter, double data[], double kk, int j, char test, int p, int counter2, int c, double timeb, double oilb, int sizeb, int xb, int yb, co_t current_pos, co_t end_coordinate, double oil) {
	remove("for-nitai.txt");
	FILE* best_route;
	FILE* best_route3;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return;
	fopen_s(&best_route3, "for-nitai.txt", "at");
	if (!best_route3) return;
	for (i = 0; i < counter; i++)
		if (kk <= data[i]) {
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
			fseek(best_route, -c - 2, SEEK_CUR);
			for (p = 0; p < counter2 + 1; p++) {
				fscanf_s(best_route, "%lf %lf %d %d %d ", &timeb, &oilb, &sizeb, &xb, &yb);
				fseek(best_route, 3, SEEK_CUR);
				if (p == 0)
					fprintf_s(best_route3, "0.00 %.2lf 0 %d %d", oil, current_pos.x, current_pos.y);
				else {
					if (p == counter2)
						sizeb = 0;
					fprintf_s(best_route3, " => %.2lf %.2lf %d %d %d", timeb, oilb, sizeb, xb, yb);
				}
			}
		}
	}
	fclose(best_route3);
	fclose(best_route);
}

void there_a_route(double oil, co_t current_pos, co_t end_coordinate) {
	correct_data1(0, 0, 0, 0);
	correct_data2(0, 0, 0, 0);
	correct_data3(0, 0, 0, 0, 0, 0);
	int counter = set_counter();
	double* data = malloc_data(counter);
	data = set_finel_time_arr(0, 0, 0, 0, data);
	printing_to_screen(0, 0, 0, 0, 0, counter, data[0], data, oil, current_pos, 0, 0, 0, 0, 0);
	remove("best-route.txt");
	best_route_file_creation(0, counter, data, data[0], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, current_pos, end_coordinate);
	data = set_finel_oil_arr(0, 0, 0, 0, data);
	max_oil_file_creation(0, counter, data, data[0], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, current_pos, end_coordinate, oil);
	remove("best-route2.txt");
	printf_s("New best-route.txt file was created\n");
	free(data);
	//need to add section 3c!!!
}

co_t InputCheck(co_t image) {
	co_t coordinate;
	do {
		char input[81], * dex;
		char x[80] = { 0 }, y[80] = { 0 };
		int i, j = 0, flag = 0;
		printf_s("Please Enter valid x,y start coordinate, bmp width is %d and height is %d\n", image.x, image.y);
		gets_s(input, 81);
		dex = strchr(input, ',');
		while (dex == NULL) {
			printf_s("Please Enter valid x,y start coordinate, bmp width is %d and height is %d\n", image.x, image.y);
			gets_s(input, 81);
			dex = strchr(input, ',');
		}
		for (i = 0; input[i] != '\0'; i++) {
			if (input[i] < '0' || input[i] > '9')
				if (input[i] != ',' && input[i] != ' ') {
					printf_s("Please Enter valid x,y start coordinate, bmp width is %d and height is %d\n", image.x, image.y);
					gets_s(input, 81);
					i = 0;
				}
		}
		for (i = 0; input[i] != '\0'; i++) {
			if (input[i] == ',') {
				flag = 1;
				i++;
			}
			if (flag == 0) {
				x[i] = input[i];
			}
			else {
				y[j] = input[i];
				j++;
			}
		}
		coordinate.x = (int)atof(x);
		coordinate.y = (int)atof(y);
	} while (coordinate.x > image.x || coordinate.y > image.y);
	return coordinate;
}

/*Section 3 -
Shows the fastest route from a certain point on the map to the end point considering the amount of oil*/
void section_3() {
	FILE* pools;
	char trash;
	co_t end_coordinate = { 0 };
	int num_of_pool = pool_counter();
	int* pool_size_arr = pool_size_arr_malloc(num_of_pool);
	co_t* middle_arr = middle_arr_malloc(num_of_pool);
	fopen_s(&pools, "pools.txt", "rt");
	if (!pools) {
		printf_s("Error open the pools.txt");
		return;
	}
	else {
		fseek(pools, 12, SEEK_SET);
		fscanf_s(pools, "%d %c %d", &end_coordinate.x, &trash, 1, &end_coordinate.y);
		fseek(pools, 40, SEEK_CUR);
		for (int i = 0; i < num_of_pool; i++) {
			fscanf_s(pools, "%d %c %d", &middle_arr[i].x, &trash, 1, &middle_arr[i].y);
			fseek(pools, num_of_pool, SEEK_CUR);
			fscanf_s(pools, "%d %c ", &pool_size_arr[i], &trash, 1);
		}
		fclose(pools);
		reset_files();
		co_t current_pos = InputCheck(end_coordinate);
		double oil = oil_input();
		if (route_finder(current_pos, end_coordinate, oil, 0, pool_size_arr, middle_arr, 0, 0, num_of_pool, 1) != 0) {
			printf_s("Sorry, could not reach destination with these inputs\n");
			remove("best-route.txt");
			remove("best-route2.txt");
		}
		else {
			there_a_route(oil, current_pos, end_coordinate);
		}
	}
	free(pool_size_arr);
	free(middle_arr);
}