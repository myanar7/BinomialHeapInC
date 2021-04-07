# BinomialHeapInC

1-)Firstly, we take an input from user. The program search the keyword among all documents. And 
calculate the similarity scores of them.

![]()

2-)Then scan all files to find number of the keyword. We increase the number of relevant documents 
if the document have one keyword at least.

![]()

3-)Here is the enqueue and dequeue part of my source codes:

### ENQUEUE

Insertion part is making a new heap and insert it to the main heap.

```C
Node* insertNode(Node* heap, Node* node) { //INSERTION
 Node* newHeap = makeHeap();// MAKE A HEAP AS NULL
 node->parent = NULL;
 node->child = NULL;// CLEAR ALL DATA FIELDS
 node->sibling = NULL;
 node->degree = 0;
 newHeap = node; // NODE IS THE HEADER OF 
THE HEAP WHOSE DEGREE IS 0
 heap = unionHeap(heap, newHeap); // AND SET UP ALL OF THE ROOTS
 return heap; // RETURN HEAP
}

```

We also used unionHeap(Heap1 and Heap2) function in the insertion method. If there are two root 
has the same degree then linked the trees by the value of the root. unionHeap(heap1,heap2):

```C
Node* unionHeap(Node* heap1, Node* heap2) {// SET UP THE LOCATION AND CONTENT OF THE 
HEAPS
 Node* prev; Node* next;// 3 NODE PREV,NEXT AND CURRENT
 Node* current;
 Node* heap = makeHeap();// MAKE A NODE AS NULL
 heap = mergeHeaps(heap1, heap2); // MERGE TWO NODES AND 
ASSIGN IT TO TEMP HEAP
 if (heap == NULL)
 return heap;// IF TEMP HEAP IS NULL THEN JUST RETURN NULL
 prev = NULL;// 
FIRSTLY CLEAR THE PREV NODE
 current = heap;
 next = current->sibling;
 while (next != NULL) {//
IF THERE IS A SIBLING THEN GO ON
 if ((current->degree != next->degree) || ((next->sibling != NULL)
 && (next->sibling)->degree == current->degree)) { // SKIP THE NEXT ONE 
 prev = current;
 current = next;
 } else {
 if (current->data >= next->data) { // IF CURRENT IS BIGGER THAN THE NEXT 
THEN CHANGE THE LOCATION OF THE NEXT 
 current->sibling = next->sibling;
 link(next, current); //SET NEXT AS CHILD OF CURRENT
 } else {
 if (prev== NULL)
 heap = next;
 else
 prev->sibling = next;
 link(current, next); //OTHERWISE SET CURRENT AS CHILD OF THE 
NEXT
 current = next;
 }
 }
 next = current->sibling;// ASSIGN THE NEW NEXT TO FORMER NEXT
 } return heap;// RETURN HEAP 
}

```

In the unionHeaps function we firstly merged two heaps to make the change on the binomial heap. 
Then we will control both side of the current root and if there are two node has same degree linked 
(make one become child of the other one) by the magnitude of the value of the node->data. 
mergeHeap method :

```C
Node* unionHeap(Node* heap1, Node* heap2) {// SET UP THE LOCATION AND CONTENT OF THE 
HEAPS
 Node* prev; Node* next;// 3 NODE PREV,NEXT AND CURRENT
 Node* current;
 Node* heap = makeHeap();// MAKE A NODE AS NULL
 heap = mergeHeaps(heap1, heap2); // MERGE TWO NODES AND 
ASSIGN IT TO TEMP HEAP
 if (heap == NULL)
 return heap;// IF TEMP HEAP IS NULL THEN JUST RETURN NULL
 prev = NULL;// 
FIRSTLY CLEAR THE PREV NODE
 current = heap;
 next = current->sibling;
 while (next != NULL) {//
IF THERE IS A SIBLING THEN GO ON
 if ((current->degree != next->degree) || ((next->sibling != NULL)
 && (next->sibling)->degree == current->degree)) { // SKIP THE NEXT ONE 
 prev = current;
 current = next;
 } else {
 if (current->data >= next->data) { // IF CURRENT IS BIGGER THAN THE NEXT 
THEN CHANGE THE LOCATION OF THE NEXT 
 current->sibling = next->sibling;
 link(next, current); //SET NEXT AS CHILD OF CURRENT
 } else {
 if (prev== NULL)
 heap = next;
 else
 prev->sibling = next;
 link(current, next); //OTHERWISE SET CURRENT AS CHILD OF THE 
NEXT
 current = next;
 }
 }
 next = current->sibling;// ASSIGN THE NEW NEXT TO FORMER NEXT
 } return heap;// RETURN HEAP 
}

```

In the merge function (above) the main purpose is to order roots in the heap by the magnitude of the 
degrees. Finally link method :

```C
int link(Node* node1, Node* node2) {// LINK NODE1 WITH NODE2
 node1->parent = node2;
 node1->sibling = node2->child; // NODE2
 node2->child = node1; // \------>NODE1
 node2->degree++;
}

```

In the link method we adjust the least one as the child of the other one

### DEQUEUE

Deletion part is firstly find the maximum root and then delete it from the heap and return it
And these are results of program with “the” keyword.

```C
Node* extractMax(Node* heap1) { // EXTRACT THE MAX NODE IN THE HEAP
(THE MOST RELEVANT DOCUMENT)
 Node* prev = NULL;
 Node* temp1 = heap1;
 Node* node;
 tempHeap = NULL;
 if (temp1 == NULL) { // IF HEAP IS EMPTY THEN JUST RETURN NULL
 printf("\nNOTHING TO EXTRACT");
 return temp1;
 }
 int max=temp1->data;// SET VALUE OF THE TEMP1 AS MAXIMUM
 node = temp1;
 while (node->sibling != NULL) { // FIND THE MAXIMUM HEAP AMONG ROOTS
 if ((node->sibling)->data > max) {
 max = (node->sibling)->data;
 prev = node; // IF FOUND, STORE PREV,VALUE AND 
CURRENT temp1 = node->sibling;
 }
 node = node->sibling;// SKIP THE NEXT
 }
 if (prev == NULL && temp1->sibling == NULL)
 heap1 = NULL; // IF THERE IS ONLY ONE 
ROOT THEN SET NULL
 else if (prev == NULL) // IF PREV IS NULL THEN HEAP IS THE SIBLING OF TEMP1
 heap1 = temp1->sibling;
 else if (prev->sibling == NULL) //IF THE SIBLING OF PREV IS NULL THEN PREV IS NULL
 prev = NULL;
 else
 prev->sibling = temp1->sibling; // OTHERWISE LINK BOTH SIDE OF THE MAXIMUM HEAP
 
if (temp1->child != NULL) {// IF THE MAXIMUM HEAP HAS ANY CHILD OR CHILDREN THEN 
REVERSE THEM FROM MIN TO MAX
 reverse(temp1->child);
 (temp1->child)->sibling = NULL;
 }
 
 heap = unionHeap(heap1, tempHeap);// SET POSITIONS OF ALL HEAPS
 return temp1;
}
int reverse(Node* node) { //REVERSE FUNCTION FOR LINKED LISTS
 if (node->sibling != NULL) {
 reverse(node->sibling);
 (node->sibling)->sibling = node;
 } else {
 tempHeap = node;
 }
}


```

In the extract method , we firstly found the maximum node then we delete it. When we extract it, 
childs of the maximum root will be new roots. So we have to reverse them and union them.

4-) And printed relevant documents respectively

![]()

