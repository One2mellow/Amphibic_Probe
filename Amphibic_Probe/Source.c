#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct pixel {
	co_t p;
	struct pixel* next;
}pix_t;


typedef struct {
	int x;
	int y;
}co_t;

typedef struct data {
	double d; //distance
	double c; //cost
	struct data* next;
}data_t




void NumericReport(image_t image);
pix_t* pointsOfRoute(FILE* route);// לברר 
void addNewNode(data** head, double d, double c);
data_t* costscalc(co_t* route);
distance(start, end);// להוסיף את הפונקציה פה 55
// להוסיף את Best_co



void NumericReport(image_t image) {
	co_t start, end;
	FILE* route;
	char position;
	double dist, dt//dt is the input user


		fopen_s(&route, BEST_TXT, "rt");
	if (!route) {
		printf_s("Problem with file best-route.txt, or it might be empty.\n");
	}
	position = fgetc(route);
	fseek(route, 15, SEEK_SET);
	start = best_co(route);// קוראים קוארודינה ראשונה
	do {
		position = fgetc(route);
		while (position != '(' && position != EOF)
			position = fgetc(route);
		fseek(route, -1, SEEK_CUR);
		end = best_co(route);//קוראמים קוארדיטת סיום 


		pix_t* curr = NULL; //current step 
		int c;
		curr = pointsOfRoute(route);
		printf_s("Please enter a positive intger as distance display interval: ");

		do
		{

			scanf_s("%lf", &dt);
			while ((c = getchar()) != '\n' && c != EOF);
			if ((dt - (int)dt != 0) || dt <= 0)
				printf_s("Bad input, try again\n");
		} while ((dt - (int)dt != 0) || dt <= 0);

		data_t* head = costscalc(curr);
		data_t* temp = head;
		printf_s("Distance   Consumption\n========== ===========\n");
		print_data(head, dt);
		freedata_t(head);
		freepix_t(curr);
		return 0;

	}
}    //end of numericreport!!!







void addnewnode(data_t** head, double d, double c) {

	data_t* newNode = malloc(sizeof(data_t));
	if (!newNode)return;
	newNode->d = d;
	newNode->c = c;
	newNode->next = NULL;

	if (*head == NULL)
		*head = newNode;

	else {
		data_t* lastNode = *head;
		while (lastNode->next != NULL) lastNode = lastNode->next;
		lastNode->next = newNode;
	}

}


data_t* costsCalc(pix_t* curr) {     // לברר האם points זה רק הקוארדינטות מהbest route!
	int  fuel = 20, drive = 1;
	double costs = 0, D = 0, n = 0;// n is step counter D is distance  
	pix_t* start = curr, * end = curr->next;
	data_t* head = NULL;
	while (end) {
		D = D + distance(start, end);
		while (n < D) {
			addNewNode(&head, n, costs);
			n += 0.1;
			if (n < D)
				costs += ((2.5 / (costs + 1) + drive) * 0.1);
		}
		if (end->next)//אם קיים איבר הבא אז קטע חדשמ {
			costs += ((2.5 / (costs + 1) + fuel) * 0.1);
		addNewNode(&head, n, costs);
	}
	start = end;
	end = end->next;//
}
return head;



	}

	start = end;// ממישיכים את הצעדים מאיפה שסיימנו 


	void print_jump(data_t* head, double dt) {
		data_t* temp = head;
		int i;
		while (temp->next) {
			printf_s("   %.67.3lf    %7.3lf\t\n", temp->d, temp->c);
			for (i = 0; i < dt * 10; i++) {
				if (temp->next)temp = temp->next;
			}
		}
		printf_s("   %7.3lf    %7.3lf\t\n", temp->d, temp->p);
	}


	while (end.x != image.width && end.y != image.height);// at the end of one calculations
	n = dist / dt;
	//dist = malloc(sizeof(float) * n);
	cx = malloc(sizeof(float) * n);
	costs = malloc(sizeof(float) * n);
	dc = malloc(sizeof(float) * n);
	dc[0] = d;// should be print

	for (i = 1; i <= n + 1; i++) {
		dist[i] = i * dt;

		dc[i] = (A / dc[i - 1] + 1) * dx;
		dc[i - 1] += //למחיר מוסיפה את השינוי 
		//printf_s("\n %f \n", dist[i]);

	}




	pix_t* pointsOfRoute(FILE * route) {
		pix_t* tail = NULL, * temp, * head = NULL;
		char data[40];
		int i = 0;
		fseek(route, 0, SEEK_SET);
		while (fgets(data, sizeof(data), route)) {
			i++;
			if (i > 1) {
				temp = malloc(sizeof(pix_t));
				if (!temp)return NULL;
				temp->p.x = atoi(&data[1]);
				temp->p.y = atoi(strchr(data, ',') + 1);
				if (tail)tail->next = temp;
				else head = temp;
				tail = temp;
			}
		}

	}
