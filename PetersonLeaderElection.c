#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include  <semaphore.h>

struct Node  {
	int id;
	struct Node* next;
	struct Node* prev;
	int que[3];
};
int cntr=0;
int ittr=0;
int pcntr=0;
int phase=1;
sem_t  sem,sem1;
struct Node* head; // global variable - pointer to head node.

//Creates a new Node and returns pointer to it. 
struct Node* GetNewNode(int x) {
	struct Node* newNode
		= (struct Node*)malloc(sizeof(struct Node));
	newNode->id = x;
	newNode->que[2]=x;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}


//Inserts a Node at tail of Doubly linked list
void InsertAtTail(int x) {
	struct Node* temp = head;
	struct Node* newNode = GetNewNode(x);
	if(head == NULL) {
		head = newNode;
		pcntr++;
		return;
		
	}
	int l=1;
	while(l<pcntr){ 
	temp = temp->next; l++;} // Go To last Node
	temp->next = newNode;
	//printf("temp to new\n");

	newNode->prev = temp;
	//printf("new to temp\n");

	head->prev=newNode;
	//printf("head to new\n");

	newNode->next=head;
	//printf("new to head\n");
	pcntr++;

}

//Prints all the elements in linked list in forward traversal order
void Print(int x, int phase) {
	struct Node* temp = head;
		int g=0;
	while(g<x) {

		printf("[");
		printf("%d",phase); //phase number
		printf("] ");
		printf("[");
		printf("%d",temp->id );  //node id
		printf("]" );
		printf("[");
		printf("%d",temp->que[2] ); //the nodes temp uid
		printf("]\n");
		temp = temp->next;
		g++;
	}
	
	
}
	void write (struct Node* x , int value) {
sem_wait(&sem);  //locks the race condtion semaphone
struct Node* temp2 = x->next;  //creats temporary node to store the next nodes adress
temp2->que[0]=value;  //stores the retrived uid to the next 2 nodes
temp2->next->que[1]=value;
		sem_post (&sem); //releas the CS
		sem_post(&sem1);
}

int read (struct Node* x ) {
	sem_wait(&sem1);
	sem_wait(&sem);  //lock the race condtion symaphone
    int value = x->que[3]; //gets the temp uid of the node
	sem_post(&sem);  //unlock the race condiotion semaphone
return value ;  //returns the nodes uid
}

//Prints all elements in linked list in reverse traversal order. 

void* selectleader(void* x){
	
	while(pcntr>1){  //while there is more than 1 node in the linked list
	struct Node* temp = head;
		
	for(int i=0; i<pcntr; i++){ //get the id for the current node and pass it to the next two  
		int e=read(temp);
		write(temp,e);
		temp=temp->next;
	}
	struct Node* temp3 = head;
	for(int v=0; v<pcntr; v++){ 
	if(temp3->que[2]==temp3->que[0]){  //if 1hop == temp uid
		printf("leader: [");
		printf("%d", temp3->id);
		printf("]\n");

		pcntr=1;
		break;

	}
	if(temp3->que[0]>temp3->que[1] && temp3->que[0]>temp3->que[2]){ //if 1 hop > 2hop and temp uid
		//node stays avtive
		temp3->que[2]=temp3->que[0]; //id of first hop will be
	}
	else{   //else remove the node form the list
		temp3->prev->next=temp3->next;
		temp3->next->prev=temp3->prev;
		pcntr--;
//		printf("%d",temp->id );
//		printf(" node was not safe \n");
	}}
		sem_wait(&sem);  //lock the race condtion symaphone
	temp=temp->next;
	Print(pcntr, phase);  //after checking each mode, print the stat of the remoaining
	phase++;  //move to the next phas
	sem_post(&sem);
}
	pthread_exit(NULL);  
}

int main() {
	sem_init (&sem , 0, 1);  //initalize the race condition semaphone
	sem_init (&sem1 , 0, 1);  //initalize the syncronization semaphone
	/*Driver code to test the implementation*/
	head = NULL; // empty list. set head as NULL. 
	 int i;
  scanf("%d",&cntr);  //input the number of nodes

   for(int m=0; m<cntr; m++){
   	scanf("%d",&i);  //input the nodes id
	InsertAtTail(i);  //inserets the id
	}
pthread_t  threadIds [2];  
for (i = 0; i < 2; i++) {
pthread_create (& threadIds[i], NULL , (void *)selectleader ,&cntr); //creats ids
}
for (i = 0; i < 2; i++) {
pthread_join(threadIds[i], NULL); //wait for them to finsh
//  waiting  for  the  threads
}
     return 0;
	


}