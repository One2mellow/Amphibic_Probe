#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define BMP "fishpool.bmp"
#define BMPCPY "fishpool-copy.bmp"
#define TXT "pools.txt"
#define BEST_TXT "best-route.txt"


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


int main() {


	return 0;
}