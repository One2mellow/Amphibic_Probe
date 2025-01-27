



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h> //lecture #6

#define BMP "fishpool.bmp"
#define BMPCPY "fishpool-copy.bmp"
#define TXT "pools.txt"
#define BEST_TXT "best-route.txt"
#define SPECIAL "for-nitai.txt"
#define FUEL_TXT "most_fuel.txt"
#define MOST_FUEL "MostFuel.bmp"

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
	co_t pool_center;
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

typedef struct pools_coordinates_and_size {
	int center_x;
	int center_y;
	int poolsize; //the number of pixels that combine the pool
	struct pools_coordinates_and_size* next;
}printing_t;

typedef struct country_link { // struct to store link to all store products in option 5
	char* a;
	char* b;
	char* c;
	char* d;
	char* e;
}link;

typedef struct data { //Struct to use with the numeric report function
	double distance;
	double cost;
	struct data* next;
}data_t;

void switcher(int choice, pixmat** matrix, int width_flag, image_t image); //Switch case for menu selection

int loadImage(image_t* image, const char* filename, unsigned char* head); //Loading the BMP image and getting WxH values

co_t pool_middle(pix_t* root, int size); //Returning the pool's center coordinates from given coordinate array

int imgtrx(pixmat** mtrx, image_t image, char* filename, int width_flag); //Converting the BMP image to 2d array of type pixmat

poolList_t* poolsFunction(pixmat** mtrx, image_t image, poolList_t* pools, int width_flag); //Creating list of pools which contain size and center co. for each pool

void bluePixRec(pixmat** mtrx, int** temp, image_t image); //Assigning values to 1/0 matrix for each pixel 1 for blue pixel, 0 for any other pixel

void createBMP(char* filename, char* origin, char* txt, pixmat** matrix, image_t pic, unsigned char* header, int width_flag); // Print the route on the BMP copy

void segment(pix_t* root, pixmat** mtrx, int** temp, image_t image, int i, int j, int* size); //Using region base image segmentation to detect pools

void pixInsert(pix_t** root, co_t coordinate); //Appending new pixel to the end of pixels list

void poolInsert(poolList_t** root, int size, co_t center, pix_t* pix); //Appending new pool element to the end of pools list

void deallocPix(pix_t** root); //Deallocating memory of the pixel list

void deallocPool(poolList_t** root); //Deallocating memory of the pools list

int spaceMod(int x, int y); //Making sure that the correct number of spaces is printed between co. and size in pools.txt

void routePainter(pixmat** matrix, int x, int y, int x_final, int y_final, int height, int width); //Painting the movement route in the map

void reachToEnd(pixmat**, int x, int y, int x_final, int y_final); //Compensating on extreme incline while painting the route

co_t bestCo(FILE* route); //Extrcats coordinates from best route file

void printnsortpools(); //the main function for section 2, takes the data from pools.txt, and puts it into a sorted connected list

void free_list_printing(printing_t* head); //deallocating after all the list was printed

void print_list(printing_t* head); //printing the connected list in section 2

printing_t* pools_sorting_ninsert(printing_t* head, int coordinate_x, int coordinate_y, int poooolsize); //take the data from 'printnsortpools' and puts it into new nodes in the connected list, sorts every new node into the list

void time2Glow(char* filename, pixmat** matrix, image_t image, int width_flag); //for setion 5

void fuelStore(double fuel); //store genertor for section 5

char storeMenu(int country, double fuel, int random); //Prints the store list for each country

void warehouse(link* root, char purchase, int country); //Generates link database for items of section 5 store

char cashier(char purchase, double fuel, int random); //Checking if you have enogh cash to complete the transaction

list_t* interReverseLL(list_t* root); //Revesrs the linked list order of type list_t

pix_t* route_coordinates(FILE* F); //Finding the route coordinates from fiile

void demalloc_data(data_t* head); //Freeing dat_t type linked list

void demalloc_route(pix_t* head); //Freeing pix_t linked list

void print_to_screen(data_t* head, double jump); //Printing the numeric calculations to screen

void numericReport(); //Main function for the numeric report

double distance2(pix_t* route, pix_t* temp); //Finding the distance between 2 points from linked list nodes

void node_adder(data_t** head, double d, double c); //Adding nodes to numeric calculations linked list

data_t* numericCostEquation(pix_t* route); //Calculating cost value using the given equation

int section_3(char trash);// section 3 find the best route that get you to the ending point

co_t input_check(co_t image); //checking the validity of the starting coordinates

void stringfixer(char* input, int* commacount, co_t image);//making sure that the string for section 3 input is ok

void there_a_route(double oil, co_t current_pos, co_t end_coordinate); //All the functions that print to screen and file together

void max_oil_file_creation(int i, int counter, double data[], double kk, int j, char test, int p, int counter2, int c, double timeb, double oilb, int sizeb, int xb, int yb, co_t current_pos, co_t end_coordinate, double oil);
//Create a data file of the route that has the most oil left

void best_route_file_creation(int i, int counter, double data[], double kk, int j, char test, int p, int counter2, int c, double timeb, double oilb, int sizeb, int xb, int yb, co_t current_pos, co_t end_coordinate);
//Create a data file of the fastest route

void printing_to_screen(char test, int counter2, int j, int i, int p, int counter, double kk, double data[], double oil, co_t current_pos, double timeb, double oilb, int garbi, int xb, int yb);
//Print data to the screen from the linked list

void free_list(list_t* head); //Release memory in the linked list

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

int file_corrupt(int num_of_pool); //check if the file corrupt

void reducing_route_finder7(co_t tracker_coordinate, co_t* middle_arr, int num_of_pool, double oil, co_t end_coordinate, int pool_size_arr[], int r, int x, double time); //turn left in the binary tree

void reducing_section_3(co_t current_pos, co_t end_coordinate, double oil, int pool_size_arr[], co_t middle_arr[], int num_of_pool); //printing route data to the screen

int input_menu();// the menu of the program

void section_1(pixmat** matrix, image_t image, int width_flag, poolList_t* pools, int val, int count, int i); //section 1 get data from bmp and create a txt file

//Nitai
int main() {
	int i, choice = 0, width_flag = 0;
	pixmat** matrix;
	static image_t image;

	if ((loadImage(&image, BMP, image.header)) != 0) {
		return -1;
	}

	width_flag = image.width;
	for (i = 0; ((width_flag * 3) % 4) != 0; i++) // making sure width * BytesPerPixel is divsible by 4 due to BMP color pallate regulations for 24bit BMP format
		width_flag = image.width + i;

	matrix = malloc(sizeof(pixmat*) * image.width);
	if (matrix) {
		for (i = 0;i < image.width;i++) {
			matrix[i] = malloc(sizeof(pixmat) * image.height);
		} // allocate memory to image pixel matrix
	}
	else
		return 1;

	while (choice != 9) {
		choice = input_menu();
		switcher(choice, matrix, width_flag, image);
	}
	for (i = 0;i < image.width;i++) {
		free(matrix[i]);
	}
	free(matrix);
	printf_s("\nGood bye!\n");
	return 0;
}

//Nitai
void switcher(int choice, pixmat** matrix, int width_flag, image_t image) {
	poolList_t* pools = NULL;
	switch (choice) {
	case 1:
		section_1(matrix, image, width_flag, pools, 0, 0, 0);
		break;
	case 2:
		printnsortpools();
		break;
	case 3:
		putchar('\n');
		if (section_3(0) != -1)
			createBMP(BMPCPY, BMP, BEST_TXT, matrix, image, image.header, width_flag);
		break;
	case 4:
		numericReport();
		break;
	case 5:
		time2Glow(SPECIAL, matrix, image, width_flag);
		break;
	}
}

//Nadav
co_t pool_middle(pix_t* root, int size) {
	int x_max, x_min, y_max, y_min, i;
	co_t middle = { 0 };
	pix_t* curr;
	co_t* pixels = { 0 };
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
	free(pixels);
	return middle;
}

//Nitai
int loadImage(image_t* image, const char* filename, unsigned char* head) {
	int width, height, return_value = 0; //Image width, image height, validating the the file can be opened
	FILE* file; //File pointer to store the image in
	return_value = fopen_s(&file, filename, "rb"); //Reading the BMP file as binary file
	if (file) {
		if (fgetc(file) == 'B' && fgetc(file) == 'M') {  //The first to bytes 'B' & 'M' are BMP header prifxes
			fseek(file, 16, SEEK_CUR);//Skipping unrelevent data (for our purposes)
			fread(&width, 4, 1, file); //Reading image width
			fread(&height, 4, 1, file); //Reading image height
			image->width = width;
			image->height = height;
			fseek(file, 0, SEEK_SET);
			for (int i = 0;i < 54;i++) {
				image->header[i] = fgetc(file); //Reading & storing header data information
			}
		}
		fclose(file);
	}
	else {
		return_value = 0; // if failed to open the file
	}
	return return_value;
}

//Nitai
int imgtrx(pixmat** mtrx, image_t image, char* filename, int width_flag) {
	int val, i, j, k; //val for checking if the file was opened successfully, i,j,k are counters
	FILE* file; //File pointer to store the image in
	val = fopen_s(&file, filename, "rb"); //Reading the BMP file as binary file
	if (!file) {
		printf_s("\nError open the %s\n", filename); //In case we couldn't open the file
		return -1;
	}
	if (file != 0) {
		fseek(file, 54, SEEK_SET); //Skipping the header bytes and getting to the color pallette 
		for (i = 0; i < image.height; i++) { //Our matrix is set from index '0' up to index that is one less than 'image height\width'
			for (j = 0; j < image.width; j++) {
				mtrx[j][i].color.b = fgetc(file); //Saving the RGB data to the correct x,y location on the color pallette
				mtrx[j][i].color.g = fgetc(file); //On the BMP the data is actually stored in order of BGR
				mtrx[j][i].color.r = fgetc(file);
				mtrx[j][i].cordinate.x = j;
				mtrx[j][i].cordinate.y = i;//assigning x,y values to each pixel (type of the pixmat struct)
			}
			if (width_flag - image.width > 1)
				for (k = 0; k < width_flag - image.width; k++)
					fseek(file, 1, SEEK_CUR); //Skips padding, width flag is set to the actual image width for cosidering the BPP*width/4 requirement
		}
	}
	fclose(file);
	return 0;
}

//Nitai
void createBMP(char* filename, char* origin, char* txt, pixmat** matrix, image_t pic, unsigned char* header, int width_flag) {
	FILE* image, * route; //File pointers to store the image we are going to create and the txt file we are reading from
	co_t start, end; //Coordinates for the start and finish of each section in our movement route
	char position; //The next charecter we are going to read from the txt file (for validating input reasons)

	imgtrx(matrix, pic, BMP, width_flag); //resetting the image matrix before painting a route
	fopen_s(&image, filename, "wb"); //opening the image file
	fopen_s(&route, txt, "rt"); //opening the best route file
	if (image != 0 && route != 0) { //making sure that the two files are open
		position = fgetc(route); //the current cursor location
		fseek(route, 15, SEEK_SET); //skiping the title of the text file
		start = bestCo(route); //fetching the coordinate from the file
		do {
			position = fgetc(route);  //checking what char will the cursor read
			while (position != '(' && position != EOF) //checking for brackets where new coordinate starts, making sure we haven't reach to the End Of File
				position = fgetc(route);
			fseek(route, -1, SEEK_CUR);
			end = bestCo(route); //fetching coordinate values from the file
			routePainter(matrix, start.x, start.y, end.x, end.y, pic.height, pic.width); //painting teh movemnt route on the map for given coordinates
			start = end; //next movement starts from the last end point
		} while (end.x != width_flag && end.y != pic.height); //making sure we haven't reach to the edge of the map (top-right corner) 
		for (int i = 0; i < 54; i++) {
			fputc(header[i], image); //printing header data to the new BMP file
		}
		for (int i = 0; i < pic.height; i++) {
			for (int j = 0; j < pic.width; j++) { //printing the modified color pallette to the new BMP (prnting the route)
				fputc(matrix[j][i].color.b, image);
				fputc(matrix[j][i].color.g, image);
				fputc(matrix[j][i].color.r, image);
			}
				for (int k = 0; k < width_flag - pic.width; k++) //width flag is set to the actual image width for cosidering the BPP* width / 4 requirement
					fputc(0, image); //Prints padding to the image
		}
		fclose(image);
		fclose(route);// closing open files
		printf_s("A BMP file %s was created\n\n", filename);
	}
	else return;
}

//Nitai
poolList_t* poolsFunction(pixmat** mtrx, image_t image, poolList_t* pools, int width_flag) {
	int i, j, size; //i,j counter, size is the current pool size counter
	int** temp = NULL; //2D matrix that will store the information for each pixel in the image (if it's blue or not)
	pix_t* root = NULL; //The head for the linked list of pixels
	co_t center;
	if (sizeof(int*) * image.width > 0) //Making sure we are assigning possible values in the malloc function 
		temp = malloc(sizeof(int*) * image.width); //alocating memory for matrix which will contain 1/0 for each blue/non-blue pixel
	if (temp) {
		if (sizeof(int) * image.height > 0) //Making sure we are assigning possible values in the malloc function 
			for (i = 0;i < image.width; i++) {
				temp[i] = malloc(sizeof(int) * image.height);
			}//Allocate memory to 2D 1/0 temp matrix
	}
	bluePixRec(mtrx, temp, image); //Pixel's color recognizing function
	if (temp != 0) {
		for (i = 0;i < image.height;i++) {
			for (j = 0;j < image.width;j++) {
				if (temp[j][i] == 1) { //Going over all the pixels in the matrix and checking if it is blue or not
					size = 1;
					temp[j][i] = 0;
					pixInsert(&root, mtrx[j][i].cordinate); //Saving the blue pixel to the head of linked list
					segment(root, mtrx, temp, image, i, j, &size); //Entering to Image segmentation function to find adjacent blue pixels
					if (size > 9) {
						center = pool_middle(root, size);
						poolInsert(&pools, size, center, root); //Saving the pool to linked list ONLY if it has size of 10 or more
					}
				}
				deallocPix(&root); //Deallocting the memory of the pixel's linked list
			}
		}
		for (i = 0;i < image.width;i++) {
			free(temp[i]); //Deallocating temp matrix memory
		}
		free(temp);
		return pools;
	}
	else
		return pools;
}

//Nitai
void bluePixRec(pixmat** mtrx, int** temp, image_t image) {
	for (int i = 0; i < image.width; i++) { //temp & matrx is set from index '0' up to index that is one less than 'image height\width'
		for (int j = 0;j < image.height;j++) {
			if (mtrx[i][j].color.r == 155 && mtrx[i][j].color.g == 190 && mtrx[i][j].color.b == 245) { // Registering blue(1) and non-blue(0) pixel to 2D matrix named temp
				temp[i][j] = 1;
			}
			else {
				temp[i][j] = 0;
			}
		}
	}
}

//Nitai
void segment(pix_t* root, pixmat** mtrx, int** temp, image_t image, int i, int j, int* size) {

	if (j > 0) { //In case we are at the most left side of the map there are no pixels to the left of our location
		if (temp[j - 1][i] == 1) {
			pixInsert(&root, mtrx[j - 1][i].cordinate); //If the adjacent pixel is also blue. saves it to the blue pixels list
			*size += 1; //Increasing current pool size by 1
			temp[j - 1][i] = 0; //Setting the current blue pixel to not blue pixel
			segment(root, mtrx, temp, image, i, j - 1, size); //Using the segmentation function on the neighboring blue pixel
		}
	}

	if (j < image.width - 1) { //Making sure we are not at the right edge of the map
		if (temp[j + 1][i] == 1) {
			pixInsert(&root, mtrx[j + 1][i].cordinate); //If the adjacent pixel is also blue. saves it to the blue pixels list
			*size += 1; //Increasing current pool size by 1
			temp[j + 1][i] = 0; //Setting the current blue pixel to not blue pixel
			segment(root, mtrx, temp, image, i, j + 1, size); //Using the segmentation function on the neighboring blue pixel
		}
	}

	if (i > 0) { //In case we are at the very bottom of the map there are no pixels under current location
		if (temp[j][i - 1] == 1) {
			pixInsert(&root, mtrx[j][i - 1].cordinate); //If the adjacent pixel is also blue. saves it to the blue pixels list
			*size += 1; //Increasing current pool size by 1
			temp[j][i - 1] = 0; //Setting the current blue pixel to not blue pixel
			segment(root, mtrx, temp, image, i - 1, j, size); //Using the segmentation function on the neighboring blue pixel
		}
	}

	if (i < image.height - 1) { //Making sure we are not at the top of the map
		if (temp[j][i + 1] == 1) {
			pixInsert(&root, mtrx[j][i + 1].cordinate); //If the adjacent pixel is also blue. saves it to the blue pixels list
			*size += 1; //Increasing current pool size by 1
			temp[j][i + 1] = 0; //Setting the current blue pixel to not blue pixel
			segment(root, mtrx, temp, image, i + 1, j, size); //Using the segmentation function on the neighboring blue pixel
		}
	}
}

//Nitai
void pixInsert(pix_t** root, co_t coordinate) {
	pix_t* new_pix = malloc(sizeof(pix_t)); //Allocating memory for new pixel elemnt of type pix_t
	if (new_pix == NULL) { //Validating the malloc was able to find viable adress in the memory
		exit(1);
	}
	new_pix->next = NULL; //The new elemnt is the last one in the list
	new_pix->p.x = coordinate.x;
	new_pix->p.y = coordinate.y; //Assigning the. x,y values to the new pixel in the list

	if (*root == NULL) { // If it's the first pixel that wat recognized, assigning it to the head of the list
		*root = new_pix;
		return;
	}

	pix_t* curr = *root;
	while (curr->next != NULL) { //In case that the first spot is already taken appending the new_pix element to the end of the list
		curr = curr->next;
	}
	curr->next = new_pix;
}

//Nitai
void poolInsert(poolList_t** root, int size, co_t center, pix_t* pix) {

	poolList_t* new_pool = malloc(sizeof(poolList_t)); //Allocating memory for new pixel elemnt of type poolList_t
	if (new_pool == NULL) { //Validating the malloc was able to find viable adress in the memory
		exit(1);
	}
	new_pool->next = NULL; //The new elemnt is the last one in the list
	new_pool->pool_center.x = center.x; //Assigning Pool's ceneter x coordinate
	new_pool->pool_center.y = center.y; //Assigning Pool's ceneter y coordinate
	new_pool->size = size; //Assigning Pool's size
	new_pool->pix = pix;

	if (*root == NULL) { // If it's the first pool that wat recognized, assigning it to the head of the list
		*root = new_pool;
		return;
	}

	poolList_t* curr = *root;
	while (curr->next != NULL) { //Making sure that the new pools that was found is saved to the end of the Pools list
		curr = curr->next;
	}
	curr->next = new_pool;
}

//Nitai
void deallocPix(pix_t** root) {
	pix_t* curr = *root;
	while (curr != NULL) {
		pix_t* aux = curr; //Deallocating the linked list memory from the down to its tail
		curr = curr->next;
		free(aux);
	}
	*root = NULL;
}

//Nitai
void deallocPool(poolList_t** root) {
	poolList_t* curr = *root;
	while (curr != NULL)
	{
		poolList_t* aux = curr; //Deallocating the linked list memory from the down to its tail
		curr = curr->next;
		free(aux);
	}
	*root = NULL;
}

//Nitai
int spaceMod(int x, int y) {
	int n;
	int space = 0;
	n = x;
	while (n != 0)
	{
		n = n / 10; //counting number of digits in the x value
		space++;
	}
	n = y;
	while (n != 0)
	{
		n = n / 10; //counting number of digits in the y value
		space++;
	}

	return space;
}

//Nitai
void routePainter(pixmat** matrix, int x, int y, int x_final, int y_final, int height, int width) {
	int  b, j = 0, i = 0, s_f = 1, dif;
	float movratio;
	x--;y--; //compensating for starting point which must be 1,1
	matrix[x][y].color.r = 18; matrix[x][y].color.g = 180; matrix[x][y].color.b = 30; //color pixel at the beggining
	movratio = ((float)y_final - (float)y) / ((float)x_final - (float)x); //Calculating the slope of the line
	b = y - (int)(movratio * x); //Calculating line's constant
	if (movratio == 1 || movratio < 1) //Covering cases so that the starting pixel color won't be overpainted
		x++;
	for (x; x < x_final && x < width - 1 && y < y_final && y < height - 1; x++) { //Making sure we are not going over the end point or the image borders
		if (movratio != 1)
		{
			dif = y;
			if ((int)((x * movratio) + b) == 0) { //In cases when the line function returns y = 0 due to calculations (b added to the slope)
				y++;
				matrix[x][y].color.r = 100; matrix[x][y].color.g = 30; matrix[x][y].color.b = 232;
				y = (int)(((x + 1) * movratio) + b); //New y value on the line
			}
			else
				y = (int)(((x + 1) * movratio) + b); //adding 1 to x in order to reach to next coordinate
			if (y - dif > 1) //If the slope is too steep\gentle somteimes there are gaps between the y values of two points
			{
				for (dif; y - dif >= 1;) { //In this loop we are maing sure that all the pixel between the point(In the case mentioned above) are colored
					dif++;
					matrix[x][dif].color.r = 100; matrix[x][dif].color.g = 30; matrix[x][dif].color.b = 232;
				}
			}
			else //In case the slope is not too steep\gentle
				matrix[x][y].color.r = 100; matrix[x][y].color.g = 30; matrix[x][y].color.b = 232;
		}
		else { //For slope = 1
			y = (int)((x * movratio) + b);
			matrix[x][y].color.r = 100; matrix[x][y].color.g = 30; matrix[x][y].color.b = 232;
		}
	}
	reachToEnd(matrix, x, y, x_final, y_final); //This function check if we painted the entire route (up to the end)
	matrix[width - 1][height - 1].color.r = 250; matrix[width - 1][height - 1].color.g = 180; matrix[width - 1][height - 1].color.b = 30; //Coloring pixel at the end
}

//Nitai
void reachToEnd(pixmat** matrix, int x, int y, int x_final, int y_final) {
	if (x < x_final - 1) //If we haven't reach to the x value we supposed to be at the finish point
		x -= 1;
	while (y == y_final - 1 && x < x_final - 1) { //If we inline (horizontally) with our finish point, paint the route straight to it
		matrix[x][y].color.r = 100; matrix[x][y].color.g = 30; matrix[x][y].color.b = 232;
		x += 1;
	}
	if (y < y_final - 1) //If we haven't reach to the y value we supposed to be at the finish point
		y -= 1;
	while (x == x_final - 1 && y < y_final - 1) { //If we inline (vertically) with our finish point, paint the route straight to it
		matrix[x][y].color.r = 100; matrix[x][y].color.g = 30; matrix[x][y].color.b = 232;
		y += 1;
	}
}

//Nitai
co_t bestCo(FILE* route) {
	int flag = 0, j = 0; //Flag that tells us to assign values to y after the cooma, j is a counter
	char temp[10], tmpx[3] = { 0 }, tmpy[3] = { 0 }; //Initating strings to recive the coordinate values
	co_t coordinate = { 0 }; //Initating the co_t coordinate variable

	if (route != 0) { //If the best route file exists and open

		fread(&temp, 1, 10, route); //Read 10 charecters from it to the temp char array

		for (int i = 0; i < 9; i++) //the length of the coordinate line is 9 chrecters by format requirements
		{
			if (temp[i] == ',') { //Setting the flag so we know if we are reading x or y value from the coordinate
				flag = 1;
				j = 0;
			}
			if (flag == 1) //Before the comma we read the x value 
			{
				if (temp[i] >= '0' && temp[i] <= '9') {
					tmpy[j] = temp[i];
					j++;
				}
			}
			else //After the comma (flag is set to 0) we read the y value
			{
				if (temp[i] >= '0' && temp[i] <= '9') {
					tmpx[j] = temp[i];
					j++;
				}
			}
		}
		coordinate.x = atoi(tmpx);
		coordinate.y = atoi(tmpy); //Converting string to int type variable
	}
	return coordinate;
}

//Yonatan
void printnsortpools() {
	printing_t* head = NULL; // points the head of the connected list
	FILE* pools; // to open the file
	char trash;
	co_t end_coordinate = { 0 };
	int num_of_pool = pool_counter(); //counts pools
	int poooolsize = 0, coordinate_x = 0, coordinate_y = 0;
	int* pool_size_arr = pool_size_arr_malloc(num_of_pool);
	co_t* middle_arr = middle_arr_malloc(num_of_pool);
	if (file_corrupt(num_of_pool) == 0) { //checking for corrupted file, if there are illegal edits
		printf_s("\nfunction readpools resulted with file read error. Check that the text file has the correct format.\n");
		return;
	}
	fopen_s(&pools, "pools.txt", "rt");
	if (!pools) {
		printf_s("\nProblem reading pools.txt file\nList is empty\n"); //if there is no such a file  //failed to open pools.txt
		return;
	}
	else {
		if (num_of_pool <= 0) { printf_s("\nList is empty\n"); } //if there are no pools)
		else {
			printf_s("\nSorted pools by size:\nCoordinate      Size\n==========  \t====\n"); //format
			fseek(pools, 12, SEEK_SET); //jumps 1st line
			fscanf_s(pools, "%d %c %d", &end_coordinate.x, &trash, 1, &end_coordinate.y); //stashing into trash the 2nd line
			fseek(pools, 40, SEEK_CUR); //jumps the padding
			for (int i = 0; i < num_of_pool; i++) {
				fscanf_s(pools, "%d %c %d", &coordinate_x, &trash, 1, &coordinate_y); // taking the numbers into integers
				fseek(pools, 1, SEEK_CUR);
				fscanf_s(pools, "%d %c ", &poooolsize, &trash, 1); //taking the last number into the pool size
				head = pools_sorting_ninsert(head, coordinate_x, coordinate_y, poooolsize);
			}
		}
		fclose(pools);
	}
	print_list(head);
	free_list_printing(head);
	free(pool_size_arr);
	free(middle_arr);
}

//Yonatan
void free_list_printing(printing_t* head) {
	printing_t* tmp;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

//Yonatan
void print_list(printing_t* head) {
	while (head != NULL) {
		printf_s("(%3d,%3d)  \t%d \n", head->center_x, head->center_y, head->poolsize);
		head = head->next;
	}
}

//Yonatan
printing_t* pools_sorting_ninsert(printing_t* head, int coordinate_x, int coordinate_y, int poooolsize) {
	//making the nodes in the list and sorting them
	printing_t* ptr = head; // holds the first node in the list
	printing_t* new_node = malloc(sizeof(printing_t));
	if (!new_node) return head; //if failed to allocate
	new_node->center_x = coordinate_x;
	new_node->center_y = coordinate_y;
	new_node->poolsize = poooolsize;
	new_node->next = NULL; //in order to prevent the last node to be garbage
	if (!head) // empty list_t
		return new_node;
	if (poooolsize < ptr->poolsize) { //if the size of the new pool is smaller
		while (ptr->next && poooolsize < ptr->next->poolsize) //until the new pool is in her place
			ptr = ptr->next;
		new_node->next = ptr->next; //pushes the new node in to his new place and connecting him to the next node
		ptr->next = new_node; //connects the previos node to the new node
	}
	else { // Place at beginning of list_t
		new_node->next = head; // the new head
		head = new_node;
	}
	return head;
}

//Naama
void demalloc_data(data_t* head) {
	if (!head) {
		return;
	}
	demalloc_data(head->next);
	free(head);
}

//Naama
void demalloc_route(pix_t* head) {
	if (!head) {
		return;
	}
	demalloc_route(head->next);
	free(head);
}

//Naama
data_t* numericCostEquation(pix_t* route) {//points from best route
	pix_t* tm1 = route, * tm2 = route->next;
	data_t* head = NULL;
	int drive = 1, oil = 20; //Acording to the amphibic probe location Pool - 20 movement - 1
	double cost = 0, dist = 0, t = 0; //Setting the parmetrs to 0
	while (tm2 != NULL) {
		dist = dist + distance2(tm1, tm2); //Continously calculting the distance
		while (t < dist) {
			node_adder(&head, t, cost); //Saving the data to a node in a linked list
			t += 0.1; //The steps (dx) are set to be 0.1 for each time
			if (t < dist) {
				cost += ((2.5 / (cost + 1) + drive) * 0.1); //Numeric equation given
			}
		}
		if (tm2->next) {
			cost += ((2.5 / (cost + 1) + oil) * 0.1);
			node_adder(&head, t, cost);
		}
		tm1 = tm2;
		tm2 = tm2->next;
	}
	return head;
}

//Naama
void node_adder(data_t** head, double distance, double cost) {
	data_t* new_node = NULL;
	new_node = malloc(sizeof(data_t));
	if (!new_node) { //In case malloc wasn't able to find memory to allocate
		return;
	}
	new_node->distance = distance;
	new_node->cost = cost;
	new_node->next = NULL;
	if (*head == NULL) { //If it's the first node was created
		*head = new_node;
	}
	else {
		data_t* end_node = *head;
		while (end_node->next != NULL) end_node = end_node->next; {
			end_node->next = new_node; } //Make sure the node is added to the end of the list
	}
}

//Nitai
void time2Glow(char* filename, pixmat** matrix, image_t image, int width_flag) {
	FILE* file, * txt;
	list_t* curr, * root = NULL;
	char pos;
	float time, fuel;
	int size, x, y, i = 0;
	fopen_s(&file, filename, "rt");
	if (!file) {
		printf_s("\nThe file does not open or does not exist. Try to do section 3 first.\n\n");
		return;
	}
	else {
		fopen_s(&txt, FUEL_TXT, "wt");
		if (file != 0 && txt != 0) {
			pos = fgetc(file);
			fseek(file, 0, SEEK_SET);
			while (pos != EOF) {
				fscanf_s(file, "%f %f %d %d %d", &time, &fuel, &size, &x, &y); //Getting the values for each pont from the most fuel txt file
				root = add((double)time, (double)fuel, size, x, y, root);
				root = interReverseLL(root); //Reversing the list because we added nodes in the reverse order
				pos = fgetc(file);
				for (i; pos != '>' && pos != EOF; i++)
					pos = fgetc(file);
			}
			fseek(txt, 15, SEEK_SET);
			//iterating through the list
			for (curr = root; curr != NULL; curr = curr->next) {
				fprintf_s(txt, "(%3d,%3d)\t%d\n", curr->x, curr->y, curr->size);
				if (curr->next == NULL)
					fuelStore(curr->oil);
			}
			fclose(file);
			fclose(txt);
			createBMP(MOST_FUEL, BMP, FUEL_TXT, matrix, image, image.header, width_flag); //Painting the route of most fuel to new BMP image
			free_list(root); //Deallocating linked list
		}
	}
}

//Nitai
void fuelStore(double fuel) {
	srand((unsigned int)time(NULL)); //initiating randomize function
	int country, random = rand();
	char purchase;
	link root[5];
	printf_s("\a\ncongratulations YOU HAVE MADE IT TO THE END!\nWelcome to your new plant, we have exausted our natural resorces and fuel is our only currency!\n\nyou have %.2lf $ worth fuel left in the tank\n", fuel);
	printf_s("\a\nthere is our store, you can buy anything here right away!\n (provided you have anough fuel...)\n");
	if (fuel < 4)
		printf("With this amount you can buy only a bottle of water..\n");
	else {
		printf_s("\nPlease select the country you want to buy from:\n\n 1) Israel\n 2) USA\n 3) China\n 4) Russia\n 5) Turkey\n\n Your Choice : ");
		scanf_s("%d%c", &country, &purchase, 1);
		purchase = storeMenu(country, fuel, random);
		warehouse(root, purchase, country);
		printf("\nAnd as a bonus we printed out for you a map with the route you've taken so others could learn how to save on fuel!\nlook for \"%s\" image in the folder\n", MOST_FUEL);
	}
}

//Nitai
char storeMenu(int country, double fuel, int random) {
	char purchase, enter;
	switch (country) {
	case 1:
		printf_s("\nA: Bottle of Wine (Mid-Range) %.2lf $\nB: Meal, Inexpensive Restaurant %.2lf $\nC: Monthly Pass (Regular Price) %.2lf $\nD: Meal for 2 People, Mid-range Restaurant, Three-course %.2lf $\nE: Parking in Rotschild st. in Tel Aviv %.2lf $\n", (fuel) / 5, (fuel) / 4, (fuel) / 3 + random % 2, (fuel) / 2 + random % 4, fuel + random % 10);
		printf_s("What will you would like to buy? : ");
		purchase = cashier(getchar(), fuel, random);
		enter = getchar();
		break;
	case 2:
		printf_s("\nA: Bottle of Wine (Mid-Range) %.2lf $\nB: 6 Domestic Beer (0.5 liter draught) %.2lf $\nC: 1 Pair of Jeans %.2lf $\nD: 1 Pair of Nike Running Shoes %.2lf $\nE: A tour in NASA space Station %.2lf $\n", (fuel) / 5, (fuel) / 4, (fuel) / 3 + random % 2, (fuel) / 2 + random % 4, fuel + random % 10);
		printf_s("What will you would like to buy? : ");
		purchase = cashier(getchar(), fuel, random);
		enter = getchar();
		break;
	case 3:
		printf_s("\nA: Beef Round (1kg) %.2lf $\nB: Meal for 2 People, Mid-range Restaurant, Three-course %.2lf $\nC: 1 Summer Dress in a Chain Store %.2lf $\nD: 1 Pair of Men Leather Business Shoes %.2lf $\nE: A Kung-Fu lesson with Shaolin munk %.2lf $\n", (fuel) / 5, (fuel) / 4, (fuel) / 3 + random % 2, (fuel) / 2 + random % 4, fuel + random % 10);
		printf_s("What will you would like to buy? : ");
		purchase = cashier(getchar(), fuel, random);
		enter = getchar();
		break;
	case 4:
		printf_s("\nA: Beef Round (1kg) %.2lf $\nB: 2 Bottles of Wine (Mid-Range) %.2lf $\nC: Monthly Pass (Regular Price) %.2lf $\nD: Meal for 2 People, Mid-range Restaurant, Three-course %.2lf $\nE: Horseback riding with the one and only Vladimir Putin %.2lf $\n", (fuel) / 5, (fuel) / 4, (fuel) / 3 + random % 2, (fuel) / 2 + random % 4, fuel + random % 10);
		printf_s("What will you would like to buy? : ");
		purchase = cashier(getchar(), fuel, random);
		enter = getchar();
		break;
	case 5:
		printf_s("\nA: 3 Meals, Inexpensive Restaurant %.2lf $\nB: 1 Pair of Nike Running Shoes(Fake ones) %.2lf $\nC: A couple retreat to Istanbul's best Spa & Turkish bath %.2lf $\nD: A picture infront the palace (without getting arrested) %.2lf $\nE: Starring in your own Soap Opera %.2lf $\n", (fuel) / 5, (fuel) / 4, (fuel) / 3 + random % 2, (fuel) / 2 + random % 4, fuel + random % 10);
		printf_s("What will you would like to buy? : ");
		purchase = cashier(getchar(), fuel, random);
		enter = getchar();
		break;
	default:
		purchase = 'F';
		break;
	} return purchase;
}

//Nitai
void warehouse(link* root, char purchase, int country) {
	root[0].a = "https://images.app.goo.gl/ywD6DBAA7VYeydC7A"; root[0].b = "https://images.app.goo.gl/tHh1p6ZpRtVyAU3QA"; root[0].c = "https://images.app.goo.gl/8EYuGorXd3i1Zm4S8"; root[0].d = "https://images.app.goo.gl/7EBnM4NXzPVeMVBZ6"; root[0].e = "https://images.app.goo.gl/gft2hhyj6tL16T3J6";
	root[1].a = root[0].a; root[1].b = "https://images.app.goo.gl/j4dE3DmupwjMrsmM7"; root[1].c = "https://images.app.goo.gl/iAV8FFF2Puq8jBVt5"; root[1].d = "https://images.app.goo.gl/agoj1a1cBfMntM179"; root[1].e = "https://images.app.goo.gl/jL7L1ap2dS6P5XRP7";
	root[2].a = "https://images.app.goo.gl/jZoXAhupeoL3vAvU7";root[2].b = "https://images.app.goo.gl/Qep3v3heo9oKAh9w9"; root[2].c = "https://images.app.goo.gl/mMEkYzUGVQ4N3ps27"; root[2].d = "https://images.app.goo.gl/WypDRjnJvUjRAddM8"; root[2].e = "https://images.app.goo.gl/fNQQReVSpatNh4Tv5";
	root[3].a = root[2].a; root[3].b = root[1].a; root[3].c = root[0].c; root[3].d = "https://images.app.goo.gl/H23qyGT3Co3Z6ZL29"; root[3].e = "https://images.app.goo.gl/UTKJsDncjcbXrFZ79";
	root[4].a = "https://images.app.goo.gl/rmXDTxquFaJ5wkW59"; root[4].b = "https://images.app.goo.gl/kgHje9Cg7ezoQwBT7"; root[4].c = "https://images.app.goo.gl/rUfouppXL9rt3o1o9"; root[4].d = "https://images.app.goo.gl/Wz2qRQ8Do6bD66pZ8"; root[4].e = "https://images.app.goo.gl/VxPKtzbjnmnaNpe89";
	switch (purchase)
	{
	case 'A':
		printf_s("Copy the link to your browser to get your product\n\n ----  %s  ----\n", root[country - 1].a);
		break;
	case 'B':
		printf_s("Copy the link to your browser to get your product\n\n ----  %s  ----\n", root[country - 1].b);
		break;
	case 'C':
		printf_s("Copy the link to your browser to get your product\n\n ----  %s  ----\n", root[country - 1].c);
		break;
	case 'D':
		printf_s("Copy the link to your browser to get your product\n\n ----  %s  ----\n", root[country - 1].d);
		break;
	case 'E':
		printf_s("Copy the link to your browser to get your product\n\n ----  %s  ----\n", root[country - 1].e);
		break;
	default:
		printf_s("\nSorry, you don't have enough currency for this item\nBut... I'll take your fuel anyway, TNX! \n");
		break;
	}
}

//Nitai
char cashier(char purchase, double fuel, int random) {
	char enter;
	switch (purchase)
	{
	case 'A':
		return 'A';
		break;
	case 'B':
		return 'B';
		break;
	case 'C':
		if (fuel - (fuel / 3 + random % 2) >= 0) //Making sure you have sufficient currency
			return purchase;
		else
			return 'Z';
		break;
	case 'D':
		if (fuel - (fuel / 2 + random % 4) >= 0)
			return purchase;
		else
			return 'Z';
		break;
	case 'E':
		if (fuel - (fuel + random % 10) >= 0)
			return purchase;
		else
			return 'Z';
		break;
	default:
		printf_s("No such product\n But I'll take your fuel anywayyyy\a\nJust kidding, try again, enter only capital letters A-E, make sure you have enough currency:\t");
		cashier(getchar(), fuel, random);
		enter = getchar();
		break;
	}
	return '0';
}

//Nadav
list_t* interReverseLL(list_t* root) {
	list_t* current = root;
	list_t* prev = NULL, * after = NULL;
	while (current != NULL) {
		after = current->next;
		current->next = prev;
		prev = current;
		current = after;
	}
	root = prev;
	return root;
}

//Naama
pix_t* route_coordinates(FILE* best_route_file) {
	int i = 0;
	char str[25];
	pix_t* head = NULL;
	pix_t* node = NULL;
	pix_t* temp = NULL;
	fseek(best_route_file, 0, SEEK_SET);
	while (fgets(str, sizeof(str), best_route_file)) {
		i++;
		if (i > 1) {
			temp = malloc(sizeof(pix_t));
			if (!temp) { //Validates that malloc succeed allocating memory
				return NULL;
			}
			temp->p.x = atoi(&str[1]);
			temp->p.y = atoi(strchr(str, ',') + 1); //Converting to int type and saving the coordinate values
			if (node) {
				node->next = temp;
			}
			else {
				head = temp;
			}
			node = temp;
		}
	}
	if (node) {
		node->next = NULL;
	}
	return head;
}

//Naama
void numericReport() {
	int r;
	FILE* best_route_file;
	pix_t* the_route = NULL;
	data_t* head = NULL, * temp = NULL;
	double skip_size;
	fopen_s(&best_route_file, "best-route.txt", "rt");
	if (!best_route_file) { //In case we can't open file
		printf_s("Problem with file best-route.txt, or it might be empty.\n");
		return;
	}
	the_route = route_coordinates(best_route_file); //Fetching route coordinates from file
	
	printf_s("\nPlease enter a positive intger as distance display interval: ");
	do {
		scanf_s("%lf", &skip_size);
		while ((r = getchar()) != '\n' && r != EOF); {
			if ((skip_size - (int)skip_size != 0) || skip_size <= 0) { //Inout conditions
				printf_s("Bad input, try again\n");
			}
		}
	} while ((skip_size - (int)skip_size != 0) || skip_size <= 0);
	head = numericCostEquation(the_route);
	temp = head;
	printf_s("Distance   Consumption\n========== ===========\n");
	print_to_screen(head, skip_size);
	demalloc_data(head);
	demalloc_route(the_route);
	fclose(best_route_file);
}

//Naama
void print_to_screen(data_t* head, double skip_size) {
	data_t* temp = NULL;
	temp = head;
	while (temp->next) {
		if (temp->distance == 0) {
			printf_s("   %7.3lf    %7.3lf\t\n", temp->distance, temp->cost);
		}
		else {
			double g = temp->distance - (int)temp->distance;
			if (g > 0.001) {
				g = 1 - g;
				printf_s("   %7.3lf    %7.3lf\t\n", temp->distance + g, temp->cost);
			}
			else {
				printf_s("   %7.3lf    %7.3lf\t\n", temp->distance, temp->cost);

			}
		}
		for (int i = 0; i < skip_size * 10; i++) { //According to desired interval we skip the data
			if (temp->next)temp = temp->next;
		}
	}
	printf_s("   %7.3lf    %7.3lf\t\n", temp->distance, temp->cost);
}

//Naama
double  distance2(pix_t* route, pix_t* temp) {
	double distance = 0;
	distance = sqrt(pow((double)(route->p.x) - (double)(temp->p.x), 2) + pow((double)(route->p.y) - (double)(temp->p.y), 2)); //Using Pythagoras to calculate the distance
	return distance;
}

//Nadav
double distance(co_t a, co_t b) {//Finding the distance between two coordinates
	double x1, x2, y1, y2;
	x1 = a.x, x2 = b.x, y1 = a.y, y2 = b.y;
	double d;
	d = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));//A formula for finding distance
	return d;
}

//Nadav
int closest_pool(co_t current_pos, co_t middle_arr[], int size) {
	int i, j = -1;
	double smallest_d = 10000;//Placing a large value
	for (i = 0; i < size; i++) {
		if (middle_arr[i].x != 2000)
			if (smallest_d > distance(middle_arr[i], current_pos)) {//Finding the nearest pool
				smallest_d = distance(middle_arr[i], current_pos);
				j = i;
			}
	}
	return j;
}

//Nadav
int reducing_route_finder(int x, int r, double time, co_t tracker_coordinate, co_t end_coordinate, double ab, int size_of_pool) {
	FILE* best_route;
	fopen_s(&best_route, "best-route.txt", "at");
	if (!best_route) return x;
	//Prints the data from the track to the file
	fprintf_s(best_route, "(%d) %.2lf %.2lf %d %d %d\n", r, time + distance(tracker_coordinate, end_coordinate) / 0.2, ab, size_of_pool, end_coordinate.x, end_coordinate.y);
	fclose(best_route);
	return x;
}

//Nadav
int reducing_route_finder2(co_t middle_arr[], int p1, int x, int r, double bb, double ab, int pool_size_arr[], co_t temp1) {
	if (middle_arr[p1].x != 2000) {
		FILE* best_route;
		fopen_s(&best_route, "best-route.txt", "at");
		if (!best_route) return x;
		//Prints the data from the track to the file
		fprintf_s(best_route, "(%d) %.2lf %.2lf %d %d %d	", r, bb, ab, pool_size_arr[p1], temp1.x, temp1.y);
		fclose(best_route);
	}
	return x;
}

//Nadav
int reducing_route_finder4(int x) {
	FILE* best_route;
	fopen_s(&best_route, "best-route.txt", "at");
	if (!best_route) return x;
	fprintf_s(best_route, "X\n");//Prints to file X and drops a line to indicate that it failed to reach the destination.
	fclose(best_route);
	return x;
}

//Nadav
double reducing_route_finder5(double oil, co_t tracker_coordinate, co_t end_coordinate) {
	double ab = (oil - distance(tracker_coordinate, end_coordinate) * 0.2);
	if (ab < 0)//Correction of the negative sign if the fuel is precise enough for the route
		ab = ab * -1;
	return ab;
}

//Nadav
co_t* reducing_route_finder6(int num_of_pool) {
	co_t* middle_arr2 = NULL;
	middle_arr2 = (co_t*)malloc(num_of_pool * sizeof(co_t));//Allocation of memory to the set of centers
	if (middle_arr2 == NULL) {
		exit(0);
	}
	return middle_arr2;
}

//Nadav
void reducing_route_finder7(co_t tracker_coordinate, co_t* middle_arr, int num_of_pool, double oil, co_t end_coordinate, int pool_size_arr[], int r, int x, double time) {
	int p2 = closest_pool(tracker_coordinate, middle_arr, num_of_pool);
	if (p2 == -1)return;
	co_t temp2 = middle_arr[p2];
	double ac = oil - distance(tracker_coordinate, temp2) * 0.2 + pool_size_arr[p2] * 0.2;//Reduces the fuel it consumes
	double aa = time + pool_size_arr[p2] + distance(tracker_coordinate, temp2) / 0.2;//Adds the time it moves
	if ((oil - distance(tracker_coordinate, temp2) * 0.2) > 0) {//Checks if there is enough fuel
		reducing_route_finder2(middle_arr, p2, x, r, aa, ac, pool_size_arr, temp2);//Reducing the function due to limiting 40 lines
		middle_arr[p2].x = 2000;//Placing a large value so that it will not be used for subsequent routes
		co_t* middle_arr3 = reducing_route_finder6(num_of_pool);//Reducing the function due to limiting 40 lines
		for (int i = 0; i < num_of_pool; i++)
			middle_arr3[i] = middle_arr[i];
		x *= route_finder(temp2, end_coordinate, ac, aa, pool_size_arr, middle_arr3, pool_size_arr[p2], r, num_of_pool, 1);
		free(middle_arr3);
	}
}

//Nadav
//A function with double recursion, which finds all the existing routes and prints them to a temporary file
int route_finder(co_t tracker_coordinate, co_t end_coordinate, double oil, double time, int pool_size_arr[], co_t middle_arr[], int size_of_pool, int r, int num_of_pool, int x) {
	r++;
	if (oil >= 0) {//If there is fuel left
		if ((distance(tracker_coordinate, end_coordinate)) * 0.2 - 0.009 <= oil) {//If the fuel is enough to the end
			double ab = reducing_route_finder5(oil, tracker_coordinate, end_coordinate);//Reducing the function due to limiting 40 lines
			reducing_route_finder(x, r, time, tracker_coordinate, end_coordinate, ab, size_of_pool);//Reducing the function due to limiting 40 lines
			return 0;
		}
		else {//If the fuel is not enough to reach the end
			int p1 = closest_pool(tracker_coordinate, middle_arr, num_of_pool);//Finding the nearest pool
			if (p1 == -1)return 1;
			co_t temp1 = middle_arr[p1];
			double ab = oil - distance(tracker_coordinate, temp1) * 0.2 + pool_size_arr[p1] * 0.2;//Reduces the fuel it consumes
			double bb = time + pool_size_arr[p1] + distance(tracker_coordinate, temp1) / 0.2;//Adds the time it moves
			if ((oil - distance(tracker_coordinate, temp1) * 0.2) > 0) {//Checks if there is enough fuel
				reducing_route_finder2(middle_arr, p1, x, r, bb, ab, pool_size_arr, temp1);//Reducing the function due to limiting 40 lines
				middle_arr[p1].x = 2000;//Placing a large value so that it will not be used for subsequent routes
				co_t* middle_arr2 = reducing_route_finder6(num_of_pool);//Reducing the function due to limiting 40 lines
				for (int i = 0; i < num_of_pool; i++)
					middle_arr2[i] = middle_arr[i];
				x *= route_finder(temp1, end_coordinate, ab, time + pool_size_arr[p1] + distance(tracker_coordinate, temp1) / 0.2, pool_size_arr, middle_arr2, pool_size_arr[p1], r, num_of_pool, 1);
				free(middle_arr2);
			}
			reducing_route_finder7(tracker_coordinate, middle_arr, num_of_pool, oil, end_coordinate, pool_size_arr, r, x, time);//Reducing the function due to limiting 40 lines
		}
	}
	reducing_route_finder4(x);//Reducing the function due to limiting 40 lines
	return x;
}

// Nadav
int pool_counter() {
	char pointer = 0;//Indicates the character in the file
	int num_of_pool = 0;
	FILE* pools;
	fopen_s(&pools, TXT, "rt");
	if (!pools) return 0;
	fseek(pools, 40, SEEK_CUR); // jumps over the padding of pools.txt
	for (pointer = getc(pools); pointer != EOF; pointer = getc(pools)) {
		if (pointer == '(') {//Count how many pools there are in the text file
			num_of_pool++;
		}
	}
	fclose(pools);
	return num_of_pool;
}

//Nadav
int* pool_size_arr_malloc(int num_of_pool) {
	int* pool_size_arr = NULL;
	pool_size_arr = (int*)malloc(num_of_pool * sizeof(int));//Allocation of memory to an array of pool sizes
	if (pool_size_arr == NULL) {
		exit(0);
	}
	return pool_size_arr;
}

//Nadav
co_t* middle_arr_malloc(int num_of_pool) {
	co_t* middle_arr = NULL;
	middle_arr = (co_t*)malloc(num_of_pool * sizeof(co_t));//Allocation of memory to the set of pool centers
	if (middle_arr == NULL) {
		exit(0);
	}
	return middle_arr;
}

//Nadav
void reset_files() {
	FILE* best_route;
	fopen_s(&best_route, "best-route.txt", "wt");
	if (!best_route) return;
	fprintf_s(best_route, "");//Resets the files but does not delete them
	fclose(best_route);

	fopen_s(&best_route, "best-route2.txt", "wt");
	if (!best_route) return;
	fprintf_s(best_route, "");//Resets the files but does not delete them
	fclose(best_route);
}

//Nadav
double oil_input() {
	double oil;
	char temp[80] = { 0 };//Array for input
	do {
		printf_s("Please enter valid oil supply in range 1-1000\n");
		gets_s(temp, 80);
		oil = atof(temp);//Turning the array into a value in order to put the data into the oil variable
	} while ((oil < 1) || (oil > 1000));//Proper input test
	return oil;
}

//Nadav
void file_name_changer() {
	remove("best-route2.txt"); //File Remove
	char old_name[] = "temp.txt";//Old name
	char new_name[] = "best-route2.txt"; //New name
	int d = rename(old_name, new_name);//Renaming
}

//Nadav
void reducing_correct_data1(FILE* best_route, FILE* best_route2, int i, int j, int k, char test) {
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		if (test != 'X') {//If the text is not equal to X at the end of the line
			if (test == '\n') {
				j = i;
				fseek(best_route, -j + k - 2, SEEK_CUR);
				for (test = getc(best_route); k < j + 1; test = getc(best_route)) {
					fprintf_s(best_route2, "%c", test);//Print the relevant data for a new file
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
					fprintf_s(best_route2, "%c", test);//Print the relevant data for a new file
					k++;
				}
				fprintf_s(best_route2, "\n");
				k = j + 1;
			}
		}
		i++;
	}
}

//Nadav
void correct_data1(int i, int j, int k, char test) {
	FILE* best_route;
	fopen_s(&best_route, "best-route.txt", "rt");
	if (!best_route) return;
	FILE* best_route2;
	fopen_s(&best_route2, "temp.txt", "a");
	if (!best_route2) return;
	reducing_correct_data1(best_route, best_route2, i, j, k, test);//Reducing the function due to limiting 40 lines
	fclose(best_route2);
	fclose(best_route);
	file_name_changer(); //Rename the file
}

//Nadav
void correct_data2(int i, int j, int k, char test) {
	FILE* best_route;
	FILE* best_route2;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return;
	fopen_s(&best_route2, "temp.txt", "a");
	if (!best_route2) return;
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		if (test == '\n') {//If it has reached the end of a line
			j = i;
			fseek(best_route, -3, SEEK_CUR);
			test = getc(best_route);
			if (test != 'X') {//If the text is not equal to X at the end of the line
				fseek(best_route, -j + k, SEEK_CUR);
				for (test = getc(best_route); k < j + 1; test = getc(best_route)) {
					fprintf_s(best_route2, "%c", test);//Print the relevant data for a new file
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
	file_name_changer();//Rename the file
}

//Nadav
int reducing2_correct_data3(FILE* best_route, FILE* best_route2, char test, int r, int n) {
	fseek(best_route, -r - 3, SEEK_CUR);
	for (test = getc(best_route); n < r + 1; test = getc(best_route)) {
		fprintf_s(best_route2, "%c", test);//Print the relevant data for a new file
		n++;
	}
	return n;
}

//Nadav
int reducing3_correct_data3(FILE* best_route, FILE* best_route2, char test, int r, int k, int j) {
	fseek(best_route, k - r - 1, SEEK_CUR);
	for (test = getc(best_route); k < j + 1; test = getc(best_route)) {
		fprintf_s(best_route2, "%c", test);//Print the relevant data for a new file
		k++;
	}
	fseek(best_route, -2, SEEK_CUR);
	return k;
}

//Nadav
void reducing_correct_data3(FILE* best_route, FILE* best_route2, char test, int i, int j, int k, int r, int n) {
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {//Editing the data in the temporary file
		if (test == '\n') {
			j = i;
			fseek(best_route, k - j - 1, SEEK_CUR);
			test = getc(best_route);
			if (test == 49) {//If this is the first point on the route
				fseek(best_route, -2, SEEK_CUR);
				for (test = getc(best_route); k < j + 1; test = getc(best_route)) {
					fprintf_s(best_route2, "%c", test);//Print the relevant data for a new file
					k++;
				}
				fseek(best_route, -2, SEEK_CUR);
				test = getc(best_route);
			}
			else {
				if (test != 49) {//If this is not the first point on the route
					char temp = test;
					fseek(best_route, -k - 3, SEEK_CUR);
					for (test = getc(best_route); test != '\n'; test = getc(best_route)) {
						if (test == '(') {
							test = getc(best_route);
							if (test == temp) {
								n = reducing2_correct_data3(best_route, best_route2, test, r, n);//Reducing the function due to limiting 40 lines
								k = reducing3_correct_data3(best_route, best_route2, test, r, k, j);//Reducing the function due to limiting 40 lines
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

//Nadav
void correct_data3(char test, int i, int j, int k, int r, int n) {
	FILE* best_route;
	FILE* best_route2;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return;
	fopen_s(&best_route2, "temp.txt", "a");
	if (!best_route2) return;
	reducing_correct_data3(best_route, best_route2, test, i, j, k, r, n);//Reducing the function due to limiting 40 lines
	fclose(best_route2);
	fclose(best_route);
	file_name_changer();//Renames the file
}

//Nadav
int set_counter() {
	char test = 0;
	int counter = 0;
	FILE* best_route;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return 0;
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		if (test == '\n')//Count lines in file
			counter++;
	}
	fclose(best_route);
	return counter;
}

//Nadav
double* malloc_data(int counter) {//Memory allocation
	double* data = NULL;
	data = (double*)malloc(counter * sizeof(double));
	if (data == NULL) {
		exit(0);
	}
	return data;
}

//Nadav
double* set_finel_time_arr(char test, int i, int j, int k, double* data) {
	FILE* best_route;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return 0;
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		if (test == ')')//Finds the last part that contains the time
			j = i;
		if (test == '\n') {
			fseek(best_route, j - i, SEEK_CUR);
			fscanf_s(best_route, "%lf", &data[k]);//Transfers to the array only the final time of the routes
			k++;
			for (test = getc(best_route); test != '\n'; test = getc(best_route));
		}
		i++;
	}
	fclose(best_route);
	return data;
}

//Nadav
double* set_finel_oil_arr(char test, int i, int j, int k, double* data) {
	double trash;//An unnecessary data
	FILE* best_route;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return 0;
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		if (test == ')')//Finds the last part that contains the amount of oil
			j = i;
		if (test == '\n') {
			fseek(best_route, j - i, SEEK_CUR);
			fscanf_s(best_route, "%lf %lf", &trash, &data[k]);//Transfers to the array only the final amount of fuel of the routes
			k++;
			for (test = getc(best_route); test != '\n'; test = getc(best_route));
		}
		i++;
	}
	fclose(best_route);
	return data;
}

//Nadav
//A recursive function that prints the organs in reverse order
int print(list_t* head, int i, int counter2) {
	if (head == NULL)//Reaching the last organ of the list
		return i;
	i = print(head->next, i, counter2);
	if (i == 0) {//Print the list from last member to first
		printf_s("Time=%.2lf (%d,%d) oil=%.2lf", head->time, head->x, head->y, head->oil);
		i++;
	}
	else {
		printf_s(" -> Time=%.2lf (%d,%d) oil=%.2lf", head->time, head->x, head->y, head->oil);
		i++;
	}
	return i;
}

//Nadav
list_t* add(double time, double oil, int size, int x, int y, list_t* head) {
	list_t* new_node = NULL;
	new_node = (list_t*)malloc(sizeof(list_t));
	if (new_node == NULL) {
		exit(0);
	}
	//Adds an organ to the linked list
	new_node->time = time;
	new_node->oil = oil;
	new_node->size = size;
	new_node->x = x;
	new_node->y = y;
	new_node->next = head;
	head = new_node;
	return head;
}

//Nadav
void free_list(list_t* head) {
	list_t* tmp;
	while (head != NULL) {//Release the linked list organ by organ
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

//Nadav
void printing_to_screen(char test, int counter2, int j, int i, int p, int counter, double kk, double data[], double oil, co_t current_pos, double timeb, double oilb, int garbi, int xb, int yb) {
	FILE* best_route;
	fopen_s(&best_route, "best-route2.txt", "rt");
	if (!best_route) return;
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		for (test = getc(best_route); (test != '\n') && (test != EOF); test = getc(best_route)) {
			if (test == ')')//Count how many stopping points there are on the route
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
				head = add(timeb, oilb, garbi, xb, yb, head);//Transfers data from the file to a linked list
				fseek(best_route, 3, SEEK_CUR);
				if (p == (counter2 - 1)) {
					fseek(best_route, -4, SEEK_CUR);//Skip a few characters
					counter2 = 0;
					j = 0;
				}
			}
			print(head, 0, counter2);//A function that prints the linked list
			printf_s("\n");
			free_list(head);//Release the linked list
		}
		i++;
	}
	fclose(best_route);
}

//Nadav
void best_route_file_creation(int i, int counter, double data[], double kk, int j, char test, int p, int counter2, int c, double timeb, double oilb, int sizeb, int xb, int yb, co_t current_pos, co_t end_coordinate) {
	FILE* best_route;
	FILE* best_route3;
	fopen_s(&best_route, "best-route2.txt", "rt"); // Reading from one file and copying to another
	if (!best_route) return;
	fopen_s(&best_route3, "best-route.txt", "at");
	if (!best_route3) return;
	for (i = 0; i < counter; i++)//Finds the route with the smallest time
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
			fseek(best_route, -c, SEEK_CUR);//Count how many stops there are on each route
			test = getc(best_route);
			for (p = 0; p < counter2; p++) {
				fscanf_s(best_route, "%lf %lf %d %d %d ", &timeb, &oilb, &sizeb, &xb, &yb);
				fseek(best_route, 3, SEEK_CUR);
				if (p == 0) {
					fprintf_s(best_route3, "Best Route	Size\n");//Prints to a file according to the format
					fprintf_s(best_route3, "(%3d,%3d)	0\n", current_pos.x, current_pos.y);
				}
				if (p < counter2 - 1)
					fprintf_s(best_route3, "(%3d,%3d)	%d\n", xb, yb, sizeb);
			}
		}
	}
	fprintf_s(best_route3, "(%3d,%3d)	0\n", end_coordinate.x, end_coordinate.y);//Print end coordinates
	fclose(best_route3);
	fclose(best_route);
}

//Nadav
void max_oil_file_creation(int i, int counter, double data[], double kk, int j, char test, int p, int counter2, int c, double timeb, double oilb, int sizeb, int xb, int yb, co_t current_pos, co_t end_coordinate, double oil) {
	remove("for-nitai.txt");//Remove the file before creating the file
	FILE* best_route;
	FILE* best_route3;
	fopen_s(&best_route, "best-route2.txt", "rt");//Reading from one file and copying to another
	if (!best_route) return;
	fopen_s(&best_route3, "for-nitai.txt", "at");
	if (!best_route3) return;
	for (i = 0; i < counter; i++)//Finding the route that has the most fuel left
		if (kk <= data[i]) {
			kk = data[i];
			j++;
		}
	j--;
	for (test = getc(best_route); test != EOF; test = getc(best_route)) {
		if (test == '\n')
			p++;
		if (p == j) {
			for (test = getc(best_route); test != '\n'; test = getc(best_route)) {//Count how many stops there are on each route
				if (test == ')')
					counter2++;
				c++;
			}
			fseek(best_route, -c - 2, SEEK_CUR);
			for (p = 0; p < counter2 + 1; p++) {//Print the data to a file
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

//Nadav
void there_a_route(double oil, co_t current_pos, co_t end_coordinate) {
	correct_data1(0, 0, 0, 0);//fixing the temporary file before use the data
	correct_data2(0, 0, 0, 0);//fixing the temporary file before use the data
	correct_data3(0, 0, 0, 0, 0, 0);//fixing the temporary file before use the data
	int counter = set_counter();//count how many row in the file
	double* data = malloc_data(counter);//get memory for the data
	data = set_finel_time_arr(0, 0, 0, 0, data);// get the fastest route from the file
	printing_to_screen(0, 0, 0, 0, 0, counter, data[0], data, oil, current_pos, 0, 0, 0, 0, 0);//print the data to the screen
	remove("best-route.txt");//Remove the file before editing according to the required format
	best_route_file_creation(0, counter, data, data[0], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, current_pos, end_coordinate);
	data = set_finel_oil_arr(0, 0, 0, 0, data);//Creates the file with the fastest route
	max_oil_file_creation(0, counter, data, data[0], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, current_pos, end_coordinate, oil);//Creates the file with the most oil-efficient route
	remove("best-route2.txt");//Removes the temporary file
	printf_s("New best-route.txt file was created\n");//Notification that the file was created successfully
	free(data);
}

//Nadav
co_t input_check(co_t image) {
	co_t coordinate = { 0 };
	do {
		char input[81];
		char x[80] = { 0 }, y[80] = { 0 };
		int i, j = 0, flag = 0, commacount = 0;
		printf_s("Please Enter valid x,y start coordinate, bmp width is %d and height is %d\n", image.x, image.y);
		gets_s(input, 81);
		stringfixer(input, &commacount, image);
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
		coordinate.x = atoi(x); //we set the matrix from 0,0 in order to answer the demend of bottom left coordinate is 1,1 we subtract 1 from the input
		coordinate.y = atoi(y);
		if (coordinate.x == 0 || coordinate.y == 0 || commacount > 1)
			coordinate = input_check(image);
	} while (coordinate.x > image.x || coordinate.y > image.y);
	return coordinate;
}

//Nitai
void stringfixer(char* input, int* commacount, co_t image) {
	char* dex;
	int i;
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
		if (input[i] == ',')
			*commacount += 1;
	}
}

/*Section 3 -
Shows the fastest route from a certain point on the map to the end point considering the amount of oil*/

//Nadav
int section_3(char trash) {
	FILE* pools;
	co_t end_coordinate = { 0 };
	int num_of_pool = pool_counter();//get the number of pools
	int* pool_size_arr = pool_size_arr_malloc(num_of_pool); //get the size of the pools array
	if (file_corrupt(num_of_pool) == 0) {//if the file is corrupted
		printf_s("function readpools resulted with file read error.Check that the text file has the correct format.\n");
		return -1;
	}
	co_t* middle_arr = middle_arr_malloc(num_of_pool);
	fopen_s(&pools, TXT, "rt");
	if (!pools) {
		printf_s("Error open the %s\n", TXT);
		return -1;
	}
	else {
		fseek(pools, 12, SEEK_SET);//take data for the file to the arrays
		fscanf_s(pools, "%d %c %d", &end_coordinate.x, &trash, 1, &end_coordinate.y);
		fseek(pools, 40, SEEK_CUR);
		for (int i = 0; i < num_of_pool; i++) {
			fscanf_s(pools, "%d %c %d", &middle_arr[i].x, &trash, 1, &middle_arr[i].y);
			fseek(pools, num_of_pool, SEEK_CUR);
			fscanf_s(pools, "%d %c ", &pool_size_arr[i], &trash, 1);
		}
		fclose(pools);
		reset_files();//reset the file before start working
		co_t current_pos = input_check(end_coordinate); //get start coordinate
		double oil = oil_input();//get oil for the start
		reducing_section_3(current_pos, end_coordinate, oil, pool_size_arr, middle_arr, num_of_pool);//to help the function be under 40 rows
	}
	free(pool_size_arr);
	free(middle_arr);
	return 0;
}

//Nadav
void reducing_section_3(co_t current_pos, co_t end_coordinate, double oil, int pool_size_arr[], co_t middle_arr[], int num_of_pool) {
	if (route_finder(current_pos, end_coordinate, oil, 0, pool_size_arr, middle_arr, 0, 0, num_of_pool, 1) != 0) {
		printf_s("Sorry, could not reach destination with these inputs\n\n");//if there is not enough oil to get to the end
		remove("best-route.txt");//removing the files
		remove("best-route2.txt");
	}
	else {
		there_a_route(oil, current_pos, end_coordinate);
	}
}

//Nadav
int file_corrupt(int num_of_pool) { //checking if the file is corrupted by passing each line and checking for characters different than numbers
	FILE* pools;
	int a, b, d, e;
	char c, f, g;
	fopen_s(&pools, "pools.txt", "rt");
	if (!pools) {
		return -1; //if the file failed to open
	}
	else {
		fseek(pools, 12, SEEK_SET);//skip to the size of the map numbers
		fscanf_s(pools, "%d %c %d", &a, &c, 1, &b); //stashing the 2nd line into trash data
		fseek(pools, 39, SEEK_CUR);//skip to the pools data
		for (int i = 0; i < num_of_pool; i++) {
			fscanf_s(pools, "%c %d %c %d %c", &c, 1, &d, &f, 1, &e, &g, 1);//checking if the data is corrupted
			if (c != '(') {
				fclose(pools);
				return 0; // if the coordinates format is corupted
			}
			if (f != ',') {
				fclose(pools);
				return 0;
			}
			if (g != ')') {
				fclose(pools);
				return 0;
			}
			fscanf_s(pools, "%d ", &d);
		}
		c = getc(pools);
		if (c != EOF) {
			fclose(pools);
			return 0;
		}
		fclose(pools);
	}
	return 1;
}

//Nadav
int input_menu() {
	int choice = 0;
	char str[81];
	while (choice == 0) {
		printf_s("--------------------------\nME LAB services\n--------------------------");//menu body
		printf_s("\nMenu:\n1. Scan pools\n2. Print sorted pool list\n3. Select route\n4. Numeric report.\n5. Students addition\n9. Exit.\nEnter choice: "); //menu options
		gets_s(str, 80);//get input
		if ((str[0] > '0') && ((str[0] < '6')) || (str[0] == '9')) {//check the input
			if (str[1] == '\0') {
				choice = str[0] - 48;
				return choice;
			}
		}
		printf_s("\nBad input, try again\n\n");//the input is incorrect
	}
	return choice;
}

//Nadav
void section_1(pixmat** matrix, image_t image, int width_flag, poolList_t* pools, int val, int count, int i) {
	FILE* tx;
	if (imgtrx(matrix, image, BMP, width_flag) != -1) {
		if (pools) deallocPool(&pools);
		val = fopen_s(&tx, TXT, "w");
		if (!tx) return;
		pools = poolsFunction(matrix, image, pools, width_flag);
		if (pools == NULL) {//if there is no pool
			printf_s("\nTotal of 0 pools.\n");
			fprintf_s(tx, "%s%dx%d%s", "Image size (", image.width, image.height, ")\nPool Center	Size\n===========	====");
			fclose(tx);
			return;
		}
		else {
			printf_s("\nCoordinate x1,y1 of the first discoverd pool (%d,%d)", pools->pool_center.x, pools->pool_center.y);
			printf_s("\nSize %d", pools->size);
			if (val == 0) {
				fprintf_s(tx, "%s%dx%d%s", "Image size (", image.width, image.height, ")\nPool Center	Size\n===========	====\n");
				for (poolList_t* curr = pools; curr != NULL; curr = curr->next) {
					fprintf_s(tx, "(%d,%d)", curr->pool_center.x, curr->pool_center.y);
					for (i = 0; i < 9 - spaceMod(curr->pool_center.x, curr->pool_center.y); i++) //Total line length in the pools.txt file
						fputc(' ', tx);
					fprintf_s(tx, "%d\n", curr->size);
					count++; //iterating through the pool list, printing size and center
				}
			}
			else {
				printf_s("ERROR! couldn't open %s", TXT);
			}
			printf_s("\nTotal of %d pools.\n", count);
			fclose(tx);
		}
	}
}
