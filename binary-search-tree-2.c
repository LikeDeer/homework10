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


// void printStack();



int main()
{
	char command;
	int key;
	Node* head = NULL;

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
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
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
/* Iterative-Inorder Traversal는 stack 이 필요하다. */
void iterativeInorder(Node* node)			// LVR
{
	Node* stack[MAX_STACK_SIZE];
	while (1) {
		for (; node; node = node->left)		// L
			push(node);						// 첫 노드부터 왼쪽으로 가며 스택에 쌓아 넣는다.
		node = pop();						// 끝까지 도착하면 하나 꺼내고, 다음은 V

		if (!node) break;			// 공백 스택

		printf(" [%d] ", node->key);		// V
		node = node->right;					// R. 오른쪽으로 한칸 가서 다시 LVR. 반복
	}
}

/**
 * textbook: p 225
 */
/* Level-order Traversal은 큐를 사용한다 */
void levelOrder(Node* ptr)					// 그래프의 BFS와 유사
{
	if (!ptr) return;

	enQueue(ptr);							// 첫 노드 큐에 넣고 시작
	while (1) {					// 한 루프 마다 노드 하나씩 방문. VLR
		ptr = deQueue();
		if (ptr) {
			printf(" [%d] ", ptr->key);		// V. 큐에서 하나 꺼내고 출력
			if (ptr->left)					// L
				enQueue(ptr->left);			//
			if (ptr->right)					// R
				enQueue(ptr->right);		// 자식 노드들을 큐에 넣고 반복
		}
		else {					// 큐가 비면 순회 종료
			rear = front = -1;
			break;
		}
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
	Node* ptr = head->left;
	Node* prev = head;				// prev는 삭제하기 전 부모 노드

	while(ptr) {
		if(ptr->key == key) {		// key를 가진 노드를 찾았는데,

			if(ptr->left == NULL && ptr->right == NULL) {	// case 1: leaf 노드 이면,
				
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

			else {					// leaf 노드가 아니면,
				/* case 2: 두 개의 자식을 가진 non-leaf 노드 */
				if ((ptr->left != NULL) && (ptr->right != NULL))
				{
					/* 왼쪽 서브트리에서 가장 큰 원소로 대체 */
					Node* searchHeir = ptr->left;

					while (searchHeir->right != NULL) {
						searchHeir = searchHeir->right;
					}

					printf("substitute : [%d]\n", searchHeir->key);		// 대체되는 key 출력
					ptr->key = searchHeir->key;

					/* 대체한 원소의 노드 삭제 */
					// ** 대체된 노드는 leaf 이거나 하나의 자식을 가진 non-leaf 노드 (case 1 or case 3)
					deleteNode(ptr, searchHeir->key);
				}
				
				/* case 3: 하나의 자식을 가진 non-leaft 노드 이면, */
				else		/* 링크 재구성을 위해 몇 가지 케이스를 나눠 생각 */
				{
					if (ptr->left) {	// 그것(ptr)이 left 자식을 가진 노드라면
						if (prev->left == ptr) {	// 그것이 left 자식이었다면
							ptr = ptr->left;
							free(prev->left);
							prev->left = ptr;
						}
						else {						// 그것이 right 자식이었다면
							ptr = ptr->left;
							free(prev->right);
							prev->right = ptr;
						}
					}
					else {				// 그것이 right 자식을 가진 노드라면
						if (prev->left == ptr) {	// 그것이 left 자식이었다면
							ptr = ptr->right;
							free(prev->left);
							prev->left = ptr;
						}
						else {						// 그것이 right 자식이었다면
							ptr = ptr->right;
							free(prev->right);
							prev->right = ptr;
						}
					}
				}
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
        return NULL;			// 공백 스택
    else
        return stack[top--];		// pop
}

void push(Node* aNode)
{
	stack[++top] = aNode;			// push. *우리 스택은 '포인터 배열'
}


/* circular-queue 를 이용 */
Node* deQueue()
{
	front = (front + 1) % MAX_QUEUE_SIZE;		// front(rear)의 범위 : 0 ~ 19
	return queue[front];
}

void enQueue(Node* aNode)
{
	rear = (rear + 1) % MAX_QUEUE_SIZE;			// front(rear)의 범위 : 0 ~ 19
	queue[rear] = aNode;
}

// void printStack()
// {

// }

