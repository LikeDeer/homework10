/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE];
int top = -1;

Node* pop();
void push(Node* aNode);

/* for queue */
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

Node* deQueue();
void enQueue(Node* aNode);


int initializeBST(Node** h);

/* functions that you have to implement */
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int deleteNode(Node* head, int key);  /* delete the node for the key */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* you may add your own defined functions if necessary */
int IsInitialized(Node* head);

// void printStack();



int main()
{
	char command;
	int key;
	Node* head = NULL;

	/* node 수가 20이 넘지 않도록 조정하겠습니다. */
	 /*	68행, 93~102행을 수정하였습니다.*/
	int countNode = 0;

	/* 큐를 circular 로 조정하겠습니다. */
	 /* 전역변수 rear, front 를 -1이 아닌 0으로 초기화하고 문제를 풀겠습니다 */
	rear = front = 0;

	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			if (countNode + 1 > 20) {
				printf("The tree is full.. Unable to insert yet. :(\n");
				continue;
			}
			else {
				printf("Your Key = ");
				scanf("%d", &key);
				insert(head, key);
				countNode++;
			}
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			// printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) {

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if(*h != NULL)
		freeBST(*h);

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	/* root */
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;

	front = rear = -1;

	return 1;
}



void recursiveInorder(Node* ptr)
{
	if(ptr) {
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/**
 * textbook: p 224
 */
/* 반복적 중위 순회는 stack 이 필요하다. */
void iterativeInorder(Node* node)
{
	Node* stack[MAX_STACK_SIZE];
	while (1) {
		for (; node; node = node->left)
			push(node);				// 스택에 삽입
		node = pop();				// 스택에서 삭제

		if (!node) break;			// 공백 스택

		printf("%d", node->key);
		node = node->right;
	}
}

/**
 * textbook: p 225
 */
/* Level-order Traversal은 큐를 사용한다 */
void levelOrder(Node* ptr)			// LRV
{
	if (!ptr) return;
	enQueue(ptr);

	while (1) {
		ptr = deQueue();
		if (ptr) {
			printf("%d ", ptr->key);
			if (ptr->left)
				enQueue(ptr->left);
			if (ptr->right)
				enQueue(ptr->right);
		}
		else break;
	}
}


int insert(Node* head, int key)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL) {
		head->left = newNode;
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left;

	Node* parentNode = NULL;
	while(ptr != NULL) {

		/* if there is a node for the key, then just return */
		if(ptr->key == key) return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr;

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		if(ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	/* linking the new node to the parent */
	if(parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}


int deleteNode(Node* head, int key)
{
	/* 전처리 */
	if (IsInitialized(head)) {
		printf("Please initialize first and try again.\n");
		return 1;
	}

	
	if (head->left == NULL) {
		printf("Nothing to delete.\n");
		return 1;
	}


	Node* ptr = head->left;
	Node* prev = head;				// prev는 삭제하기 전 부모 노드

	while(ptr) {

		if(ptr->key == key) {		// key를 가진 노드를 찾았는데,
			if(ptr->left == NULL && ptr->right == NULL) {		// leaf 노드 이면, 삭제
				
				if(prev == head)		// 첫 노드가 leaf 였다면, 삭제 후 공백 트리
					head->left = NULL;

				// 부모 노드 링크 NULL로 초기화
				if(prev->left == ptr)
					prev->left = NULL;
				else
					prev->right = NULL;

				free(ptr);

				return 0;
			}
			else {					// leaf 노드가 아니면 아무것도 안하기
				printf("Found the node but it was not a leaf");
			}
			return 0;
		}

		// key를 가진 노드를 발견하지 못하면 다음 노드를 확인하기
		prev = ptr;				// prev는 ptr을 따라가기

		if(ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	printf("There's no key node.\n");

	return 1;
}


void freeNode(Node* ptr)
{
	if(ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}


int freeBST(Node* head)
{

	if(head->left == head)
	{
		free(head);
		return 1;
	}

	Node* p = head->left;

	freeNode(p);

	free(head);
	return 1;
}



Node* pop()
{
	 if(top == -1)
        return NULL;
    else
        return stack[top--];
}

void push(Node* aNode)
{
	stack[++top] = aNode;			// 우리 스택은 '포인터 배열'
}



Node* deQueue()
{
	front = (front + 1) % MAX_QUEUE_SIZE;
	return queue[front];
}

void enQueue(Node* aNode)
{
	rear = (rear + 1) % MAX_QUEUE_SIZE;
	queue[rear] = aNode;
}

// void printStack()
// {

// }



/* ------------- 개인 정의 함수 ------------- */
int IsInitialized(Node* head) {
	if (head)
		return 1;
	else return 0;
}