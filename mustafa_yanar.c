#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <string.h> 
#include <ctype.h>
#include <stdbool.h>


typedef struct node Node;         //Struct of the Node
 struct node {
    int data;//	DATA
    char fileName[50];// FILE NAME
    int degree;// DEGREE OF THE NODE
    Node* parent; // PARENT OF THE NODE
    Node* child;	// CHILD OF THE NODE
    Node* sibling; // SIBLING OF THE NODE
};

int hasKeyword(char*,const char*);
Node* makeHeap();
int link(Node*, Node*);
Node* createNode(int);
Node* unionHeap(Node*, Node*);
Node* insertNode(Node*, Node*);
Node* mergeHeaps(Node*, Node*);
Node* extractMax(Node*);
int reverse(Node*);

Node * heap = NULL;		//HEAP I VE USED
Node *tempHeap = NULL; // TEMP HEAP

int main(int argc, char *argv[]) {
	char* input=(char*)malloc(50);  // INPUT
	char* str=(char*)malloc(100);	//FILE NAME 
	int relevantDoc=0;
	printf("Enter the keyword: ");
	scanf("%s",input);
	DIR *d;
  	struct dirent *dir;
  	d = opendir("files/.");//	OPEN THE FILE
  	
  	Node* node;
  	char* text=(char*)malloc(100);  //	HOLD THE WORDS ONE BY ONE
  	
  	if (d) {	// DOES FOLDER IS EXIST
  		int dircounter=0;
    	while ((dir = readdir(d)) != NULL) { //		ALL FILES IN THE DIRECTORY
      	if(dircounter>1){//  FIRST AND SECON FILE IS NOT EXIST       "."   ".."
      		strcpy(str,"files/");//		ADD files/ PROFIX
      		strcat(str,dir->d_name);
      		FILE* file=fopen(str,"r");
      		int similarityScore=0;// 	SIMILARITY SCORE IS THE NUMBER OF THE KEYWORD IN THE DOCUMENT
      		if(!file)// IS EMPTY
      			printf("The File Does not Exist\n");
      		else{
      			while (fscanf(file, "%s", text) != EOF){	//		SCAN THE WORDS IN DOCUMENT ONE BY ONE
      			similarityScore+=hasKeyword(text,input);	//		SET SIMILARITY SCORE BY A FUNCTION
        		}
        		fclose(file);
        		relevantDoc=(similarityScore!=0)?relevantDoc+1:relevantDoc;	// IF THERE IS ONE KEYWORD IN DOC AT LEAST, THEN INCREASE RELEVANTDOC 1
			  }	
				node = createNode(similarityScore);// CREATE NEW NODE
				strcpy(node->fileName,str); // SET FILE NAME OF NODE
            	heap = insertNode(heap, node); // INSERT THE NODE TO HEAP
			  }
    	dircounter++;// PASS TO THE NEXT DOCUMENT
		}
	}
    closedir(d);// CLOSE
    int i;
    printf("\nNUMBER OF RELEVANT DOCUMENTS: %d\n",relevantDoc);//	PRINT THE NUMBER OF RELEVANT DOCUMENTS
    Node relevant[5];		// 5 NODE RELATED
    
    
    if(relevantDoc!=0){	
    	printf("\nThe Relevance order is: ");
    for(i=0;i<5;i++){							//EXTRACT MAX ONE AND ASSIGN IT TO THE RELEVANT ARRAY
    	relevant[i] = *(extractMax(heap));	
    	if(relevant[i].data==0)				// IF DATA IS 0, JUST SKIP
    		continue;
    	printf("%s(%d) ",relevant[i].fileName+6,relevant[i].data);	//PRINT THE NAME AND NUMBER OF KEYWORD
	}
	printf("\n\n");
    for(i=0;i<5;i++){
    	if(relevant[i].data==0)	//IF DATA IS 0, JUST SKIP
    		continue;
    	FILE* file=fopen(relevant[i].fileName,"r");//	OPEN THE DOC AGAIN AND PRINT WHOLE DOCUMENT
      		if(!file)
      			printf("The File Does not Exist\n");
      		else{
      			printf("%s (%d): ",relevant[i].fileName+6,relevant[i].data);//PRINT THE NAME AND NUMBER OF KEYWORD
      			while (fscanf(file, "%s", text) != EOF){
      				printf("%s ",text);//       PRINT WORDS ONE BY ONE
        		}
        		printf("\n\n");
        		fclose(file);
			  }
    	
	}
	}else
		printf("\nSorry, There is no relevant document :/");
	return 0;
}
int hasKeyword(char* word,const char* keyword){//	RETURN HOW MANY WORD CONTAINS KEYWORD 
	
	int i=0,j=0,total=0;
	bool isOnly=true;//CHECK WORD IS OVER OR NOT
	
	if(strlen(word)<strlen(keyword))		//UNLESS POSSIBLE FOR THE WORD TO CONTAIN THE KEYWORD
		return 0;
	while(word[i]!='\0'&&keyword[j]!='\0'){		//UNLESS KEYWORD AND WORD ARE EMPTY
		
		if(word[i]==keyword[j]){//		IF BOTH EQUAL
			j++;
			if(j>=strlen(keyword)&&(isalpha(word[i+1])==0)&&isOnly &&word[i+1]!='\''){// IF THERE IS KEYWORD IN THE WORD
				total++;		//+1
				j=0;
				isOnly=true;
			}
		}else if(isalpha(word[i])!=0){	//SET FALSE IF IT IS A LETTER BUT NOT EQUAL TO KEYWORD
			isOnly=false;
			j=0;
		}else{
			isOnly=true;
			j=0;
		}
 		i++;
	}
	return total; 
}
Node* makeHeap() {
    Node* np;
    np = NULL;		// MAKE AN NEW HEAP
    return np;
}
int link(Node* node1, Node* node2) {// LINK NODE1 WITH NODE2
    node1->parent = node2;
    node1->sibling = node2->child;			//		NODE2
    node2->child = node1;					//		\------>NODE1
    node2->degree++;
}
Node* createNode(int data) {//  CREATE A NODE WITH DATA,STORE DATA TO THE NODE
    Node* newNode;
    newNode = (Node*) malloc(sizeof(Node));// MEMORY ALLOCATION
    newNode->data = data;// SET DATA
    return newNode;// RETURN NODE
}
Node* mergeHeaps(Node* heap1, Node* heap2) {		//     MERGE TWO HEAP
    Node* heap = makeHeap();// NEW HEAP
    Node* temp1;
    Node* temp2;		//TEMPORARY HEAPS
    Node* temp;
    temp1 = heap1;
    temp2 = heap2;
    if (temp1 != NULL) {
        if (temp2 != NULL && temp1->degree > temp2->degree)//    IF BOTH HEAP ARE NOT NULL THEN ASSIGN THE SMALLEST ONE TO HEAP
            heap = temp2;
        else
            heap = temp1;
    } else
        heap = temp2;								//IF  HEAP1 IS NULL THEN HEAP IS NOT HEAP1
    while (temp1 != NULL && temp2 != NULL) {
        if (temp1->degree < temp2->degree) {// FIND THE TEMP1 IS SMALLER THAN TEMP2 IF IT IS THEN SKIP THE SIBLING OF TEMP1
            temp1 = temp1->sibling;
        } else if (temp1->degree == temp2->degree) {// IF THEY ARE EQUAL THEN SWAP
            temp = temp1->sibling;
            temp1->sibling = temp2;		//SWAP
            temp1 = temp;
        } else {						//IF TEMP1 BIGGER THEN SWAP
            temp = temp2->sibling;
            temp2->sibling = temp1;		//SWAP
            temp2 = temp;
        }
    }
    return heap;
}
Node* unionHeap(Node* heap1, Node* heap2) {// SET UP THE LOCATION AND CONTENT OF THE HEAPS
    Node* prev;
    Node* next;//  3 NODE PREV,NEXT AND CURRENT
    Node* current;
    Node* heap = makeHeap();// MAKE A NODE AS NULL
    heap = mergeHeaps(heap1, heap2);					// MERGE TWO NODES AND ASSIGN IT TO TEMP HEAP
    if (heap == NULL)
        return heap;// 										IF TEMP HEAP IS NULL THEN JUST RETURN NULL
    prev = NULL;// 											FIRSTLY CLEAR THE PREV NODE
    current = heap;
    next = current->sibling;
    while (next != NULL) {//													IF THERE IS A SIBLING THEN GO ON
        if ((current->degree != next->degree) || ((next->sibling != NULL)
                && (next->sibling)->degree == current->degree)) {			// SKIP THE NEXT ONE 
            prev = current;
            current = next;
        } else {
            if (current->data >= next->data) {		// IF CURRENT IS BIGGER THAN THE NEXT THEN CHANGE THE LOCATION OF THE NEXT 
                current->sibling = next->sibling;
                link(next, current);				//SET NEXT AS CHILD OF CURRENT
            } else {
                if (prev== NULL)
                    heap = next;
                else
                    prev->sibling = next;
                link(current, next);				//OTHERWISE SET CURRENT AS CHILD OF THE NEXT
                current = next;
            }
        }
        next = current->sibling;//   ASSIGN THE NEW NEXT TO FORMER NEXT
    }
    return heap;//		RETURN HEAP 
}
Node* insertNode(Node* heap, Node* node) {		//INSERTION
    Node* newHeap = makeHeap();// MAKE A HEAP AS NULL
    node->parent = NULL;
    node->child = NULL;//     CLEAR ALL DATA FIELDS
    node->sibling = NULL;
    node->degree = 0;
    newHeap = node;							// NODE IS THE HEADER OF THE HEAP WHOSE DEGREE IS 0
    heap = unionHeap(heap, newHeap);	// AND SET UP ALL OF THE HEAPS
    return heap;	// RETURN HEAP
}
Node* extractMax(Node* heap1) {		// EXTRACT THE MAX NODE IN THE HEAP		(THE MOST RELEVANT DOCUMENT)
    Node* prev = NULL;
    Node* temp1 = heap1;
    Node* node;
    tempHeap = NULL;
    if (temp1 == NULL) {					// IF HEAP IS EMPTY THEN JUST RETURN NULL
        printf("\nNOTHING TO EXTRACT");
        return temp1;
    }
    int max=temp1->data;//				SET VALUE OF THE TEMP1 AS MAXIMUM
    node = temp1;
    while (node->sibling != NULL) {			// FIND THE MAXIMUM HEAP AMONG ROOTS
        if ((node->sibling)->data > max) {
            max = (node->sibling)->data;
            prev = node;					// IF FOUND, STORE PREV,VALUE AND CURRENT
            temp1 = node->sibling;
        }
        node = node->sibling;// SKIP THE NEXT
    }
    if (prev == NULL && temp1->sibling == NULL)
        heap1 = NULL;							// IF THERE IS ONLY ONE ROOT THEN SET NULL
    else if (prev == NULL)			// IF PREV IS NULL THEN HEAP IS THE SIBLING OF TEMP1
        heap1 = temp1->sibling;
    else if (prev->sibling == NULL)	//IF THE SIBLING OF PREV IS NULL THEN PREV IS NULL
        prev = NULL;
    else
        prev->sibling = temp1->sibling;	// OTHERWISE LINK BOTH SIDE OF THE MAXIMUM HEAP
    
	if (temp1->child != NULL) {// IF THE MAXIMUM HEAP HAS ANY CHILD OR CHILDREN THEN REVERSE THEM FROM MIN TO MAX
        reverse(temp1->child);
        (temp1->child)->sibling = NULL;
    }
    
    heap = unionHeap(heap1, tempHeap);// SET POSITIONS OF ALL HEAPS
    return temp1;
}
int reverse(Node* node) {			//REVERSE FUNCTION FOR LINKED LISTS
    if (node->sibling != NULL) {
        reverse(node->sibling);
        (node->sibling)->sibling = node;
    } else {
        tempHeap = node;
    }
}
