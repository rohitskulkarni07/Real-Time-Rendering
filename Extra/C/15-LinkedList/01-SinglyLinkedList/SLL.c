#include <stdio.h>	
#include <stdlib.h> // malloc()


#define NO_DATA 000000


/*					
			How One 'Node' Looks
		____________________________
		|			|	Address		|
		|	info	|	   of		|
		|			|	Next Node	|
		---@------@-----@---------@--
*/

typedef struct NODE {

	int info;
	struct NODE* next;

} NODE;

// Head Node [Engine]

NODE* gnHead;				// Same As 'stuct NODE* gnHead'

/*
				!!!   !!!!
			!!!!   !!!				
		___| |______________________	
	 ___| @ |						|	
	{	----+	gnHead				|__
	{___	Address of First Node	|__} 
	   |--@------@-----@---------@--|
*/

//Global Function Declaration

void CreateList	  (void);
void DisplayList  (void);
void InsertNode   (void);
int  CountOfNodes (void);
int  DeleteNode   (void);


int main(void) {

	//Local Variables

	int iChoice = 0;
	int iNumberOfNodes = 0;
	int iCount = 0;
	int iDeletedData = 0;

	int i = 0; //iterator for loop

	printf("\n************* Singly Linked List *************\n");

	do {
		printf("\n\n");

		printf("\t1. Create  List\n");
		printf("\t2. Display List\n");
		printf("\t3. Count   Nodes\n");
		printf("\t4. Add     Node [at the End]\n");
		printf("\t5. Insert  Node [at the Position]\n");
		printf("\t6. Delete  Node\n");
		printf("\t0. Exit");

		printf("\n\n");

		printf("\n\tEnter Your Choice :\t");
		scanf("%d", &iChoice);


		switch (iChoice) {
			
			case 1:
				printf("\n\n");
				printf("\tEnter Number Of Node You Want To Create:\t");
				scanf("%d",&iNumberOfNodes);

				for (i = 0; i < iNumberOfNodes; i++) {
					CreateList();
				}

				break;

			case 2:
				printf("\n\n");
				DisplayList();
				break;

			case 3:
				printf("\n\n");
				iCount = CountOfNodes(); // Length
				printf("\tNumber Of Nodes In List Are '%d' \n", iCount);

				break;

			case 4:
				printf("\n\n");

				printf("\tEnter Number Of Nodes You Want To 'Add' In List: ");
				scanf("%d", &iNumberOfNodes);

				for (i = 1; i <= iNumberOfNodes; i++) {

					CreateList();
				}

				break;

			case 5:
				printf("\n\n");
				InsertNode();

				break;

			case 6:
				printf("\n\n");
				iDeletedData = DeleteNode();

				if (iDeletedData == NO_DATA) {
					printf("\tCannot Delete The Node\n");
				} else {
					printf("\tData '%d' And Node Is Deleted From List\n", iDeletedData);
				}
				
				break;
			
			case 0:
			default:

				printf("\n\n\tEXITING . . . !");
				exit(0);
		}

	} while (iChoice > -1 || iChoice < 7);

	return(0);
}

void CreateList() {

	//Local Funtion Declaration
	int GetData(void);

	NODE* nNewNode = (NODE*)malloc(sizeof(NODE));					// same as struct NODE* nNewNode = (struct Node *) malloc(sizeof(struct NODE));
	NODE* nTempNode = NULL;

	if (nNewNode == NULL) {
		printf("\n\tOops!!! Not Enough Memory, Exiting Now...\n");
		exit(0);
	}

	nNewNode->info = GetData();
	nNewNode->next = NULL;

	if (gnHead == NULL) {
		gnHead = nNewNode;

	} else {
		
		nTempNode = gnHead;

		while (nTempNode->next != NULL) {

			nTempNode = nTempNode->next;
		}
		nTempNode->next = nNewNode;

	}

	nTempNode = NULL;
	nNewNode  = NULL;
}

int GetData() {

	int iData;
	printf("\n\tEnter Data :\t");
	scanf("%d", &iData);

	return iData;
}


/* if( ghaed == NULL )
	gnHead = nNewNode

			    !!!   !!!!						  
			!!!!   !!!							  	 	Newly Created Node
		___| |______________________			   ____________________________
	 ___| @ |						|			  |			|				   |
	{	----+	gnHead '200'		| --------->  |	iData	|	  NULL		   |
	{___							|			  |			|				   |
	   |--@------@-----@---------@--|			  ---@------@-----@---------@---
					100											200
	

*/					 							  

/*
	else
		while (nTempNode->next != NULL) {
					
					 ____| |____________
					|					|
				 ___|	Temprory Node	|--------------------|
				{___		'200'		|					 |	
					|---@----------@-----					 |	
							'69'							 |
															 |
															 |
															 |
															 |
				!!!   !!!!									 |
			!!!!   !!!							  	 	First Node									Second Node
		___| |______________________			   ____________________________				 ____________________________
	 ___| @ |						|			  |			|	Address		   |			|			|	Address		 |
	{	----+	gnHead '200'		| --------->  |	iData	| Of Next Node	   | -------->  |	iData	| Of Next Node	 |
	{___							|			  |			|	  '300'		   |			|			|	'NULL'	     |
	   |--@------@-----@---------@--|			  ---@------@-----@---------@---			---@------@-----@---------@---
					100										 200										300

*/

/*
	else
		while (nTempNode->next != NULL) {

					 ____| |____________
					|					|
				 ___|	Temprory Node	|--------------------------------------------------------------|
				{___		'300'		|															   |
					|---@----------@-----															   |
							'69'																	   |
																									   |
																									   |
															                                           |
															                                           |
				!!!   !!!!																			   | 
			!!!!   !!!							  	 	First Node									Second Node								Third Node
		___| |______________________			   ____________________________				 ____________________________			  ____________________________
	 ___| @ |						|			  |			|	Address		   |			|			|	Address		 |			 |			|	Address		  |
	{	----+	gnHead '200'		| --------->  |	iData	| Of Next Node	   | -------->  |	iData	| Of Next Node	 | --------> |	iData	| Of Next Node	  |
	{___							|			  |			|	  '300'		   |			|			|	'400'	     |			 |			|	'NULL'	      |
	   |--@------@-----@---------@--|			  ---@------@-----@---------@---			---@------@-----@---------@---			 ---@------@-----@---------@---
					100											200										300										400

*/

void DisplayList(void) {

	NODE* nTempNode = NULL;	
	nTempNode = gnHead;

	if (gnHead == NULL) {
		printf("\n\tList Is Empty!!!");
		return;
	}

	printf("\t");
	while (nTempNode != NULL) {

		printf("| %d |--", nTempNode->info);
		nTempNode = nTempNode->next;
	}

	nTempNode = NULL;
	printf("\n\n");
}

int  CountOfNodes(void) {
	
	NODE* nTempNode = NULL;
	nTempNode = gnHead;

	int iTotalCount = 0;

	while (nTempNode != NULL) {
		iTotalCount++;
		nTempNode = nTempNode->next;
	}

	nTempNode = NULL;
	return iTotalCount;
}

void InsertNode(void) {

	NODE* nTempNode = NULL;
	nTempNode = gnHead;

	// Local Variable
	int iPosition = 0;

	printf("\tAt Which Position You Want To Insert New Node:\t");
	scanf("%d", &iPosition);

	if (gnHead == NULL || (iPosition > CountOfNodes() || iPosition <= 0)) {
		printf("\tList Is Empty OR You May Have Entered Wrong Position!!!\n\n");
		return;
	
	} else {

		NODE* nNewNode = (NODE*)malloc(sizeof(NODE));
		nNewNode->info = GetData();
		nNewNode->next = NULL;

		if (iPosition == 1) {							// Insert At Begining 
			nNewNode->next = gnHead;
			gnHead = nNewNode;

		} else {										// Insert At Position

			for (int i = 1; i < iPosition-1	; i++) {

				nTempNode = nTempNode->next;
			}
			nNewNode->next = nTempNode->next;
			nTempNode->next = nNewNode;

		}
		nNewNode = NULL;
		nTempNode = NULL;
	}
}

int  DeleteNode(void) {

	int iPosition;
	int iDeletedData = 0;

	NODE* nTempNode = NULL;
	NODE* nDeleteNode = NULL;

	printf("\tWhich Node You Want Delete[Position Of The Node]:\t");
	scanf("%d", &iPosition);

	if (gnHead == NULL || (iPosition > CountOfNodes() || iPosition <= 0)) {
		printf("\tList Is Empty OR You May Have Entered Wrong Position!!!\n\n");
		return NO_DATA;

	} else {

		nTempNode = gnHead;

		if (iPosition == 1) {
			iDeletedData = gnHead->info;

			gnHead = gnHead->next;
			nTempNode->next = NULL;
			free(nTempNode);

		} else {

			for (int i = 1; i < iPosition-1; i++) {
				nTempNode = nTempNode->next;
			}

			iDeletedData = nTempNode->next->info;
			
			nDeleteNode = nTempNode->next;
			nTempNode->next = nDeleteNode->next;
			nDeleteNode->next = NULL;
			free(nDeleteNode);

		}
		nDeleteNode = NULL;
		nTempNode   = NULL;

		return iDeletedData;
	}
}