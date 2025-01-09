//DEQUE(덱) 구현 - 원형 큐를 확장하여 구현(1차원 배열 사용, 정적)

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 5

typedef int element;

typedef struct {
	int front;
	int rear;
	element data[MAX_QUEUE_SIZE];
}DequeType;

void Error(char* message);
void InitDeque(DequeType* dq);
int IsEmpty(DequeType* dq);
int IsFull(DequeType* dq);
void PrintDequeue(DequeType* dq);
void AddRear(DequeType* dq, element data);
element DeleteFront(DequeType* dq);
element GetFront(DequeType* dq);
void AddFront(DequeType* dq, element data);
element DeleteRear(DequeType* dq);
element GetRear(DequeType* dq);

int main() {
	DequeType dq;
	InitDeque(&dq);

	for (int i = 0; i < 3; i++) {
		AddFront(&dq, i);
		printf("<< %d 삽입 >>\n", i);
		PrintDequeue(&dq);
		printf("\n");
	}

	for (int i = 0; i < 3; i++) {
		DeleteRear(&dq);
		printf("<< 삭제 >>\n");
		PrintDequeue(&dq);
		printf("\n");
	}

	return 0;
}

void Error(char* message) {
	fprintf(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}

void InitDeque(DequeType* dq) {
	dq->front = 0;
	dq->rear = 0;

	return;
}

int IsEmpty(DequeType* dq) {
	return (dq->front == dq->rear);
}

int IsFull(DequeType* dq) {
	return ((dq->rear + 1) % MAX_QUEUE_SIZE == dq->front);
}

void PrintDequeue(DequeType* dq) {
	printf("Front Pos: %d, Rear Pos: %d\n", dq->front, dq->rear);
	if (!IsEmpty(dq)) {
		int i = dq->front;
		do {
			i = (i + 1) % MAX_QUEUE_SIZE;
			printf("[%d] %d | ", i, dq->data[i]);
			if (i == dq->rear) {
				break;
			}
		} while (i != dq->front);
	}
	printf("\n");

	return;
}

void AddRear(DequeType* dq, element data) {
	if (IsFull(dq)) {
		Error("DEQUE is Full");
		return;
	}

	dq->rear = (dq->rear + 1) % MAX_QUEUE_SIZE;
	dq->data[dq->rear] = data;

	return;
}

element DeleteFront(DequeType* dq) {
	if (IsEmpty(dq)) {
		Error("DEQUE is Empty");
		return -1;
	}

	dq->front = (dq->front + 1) % MAX_QUEUE_SIZE;
	return dq->data[dq->front];
}

element GetFront(DequeType* dq) {
	if (IsEmpty(dq)) {
		Error("DEQUE is Empty");
		return -1;
	}

	return dq->data[(dq->front + 1) % MAX_QUEUE_SIZE];
}

void AddFront(DequeType* dq, element data) {
	if (IsFull(dq)) {
		Error("DEQUE is Full");
		return;
	}

	dq->data[dq->front] = data;
	dq->front = (dq->front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;

	return;
}

element DeleteRear(DequeType* dq) {
	if (IsEmpty(dq)) {
		Error("DEQUE is Empty");
		return -1;
	}

	int prev = dq->rear;
	dq->rear = (dq->rear - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;

	return dq->data[prev];
}

element GetRear(DequeType* dq) {
	if (IsEmpty(dq)) {
		Error("DEQUE is Empty");
		return -1;
	}

	return dq->data[dq->rear];
}
