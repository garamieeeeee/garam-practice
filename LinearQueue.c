//선형 큐 구현(1차원 배열 사용, 정적으로 큐 생성)

#include <stdio.h>
#include <stdlib.h>
#define MAX_QUEUE_SIZE 5

typedef int element;

typedef struct {
	int front;
	int rear;
	element data[MAX_QUEUE_SIZE];
}QueueType;

void Error(char* message);
void InitQueue(QueueType* q);
void PrintQueue(QueueType* q);
int IsFull(QueueType* q);
int IsEmpty(QueueType* q);
void Enqueue(QueueType* q, element data);
element Dequeue(QueueType* q);

int main() {
	QueueType q;
	InitQueue(&q);
	int data;

	Enqueue(&q, 10);
	printf("<< Enqueue 10 >>\n");
	PrintQueue(&q);
	printf("\n");

	Enqueue(&q, 20);
	printf("<< Enqueue 20 >>\n");
	PrintQueue(&q);
	printf("\n");

	Enqueue(&q, 30);
	printf("<< Enqueue 30 >>\n");
	PrintQueue(&q);
	printf("\n");

	data = Dequeue(&q);
	printf("<< Dequeue >>\n");
	PrintQueue(&q);
	printf("\n");

	data = Dequeue(&q);
	printf("<< Dequeue >>\n");
	PrintQueue(&q);
	printf("\n");

	data = Dequeue(&q);
	printf("<< Dequeue >>\n");
	PrintQueue(&q);
	printf("\n");

	return 0;
}

void Error(char* message) {
	fprintf(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}

void InitQueue(QueueType* q) {
	q->front = -1;
	q->rear = -1;

	return;
}

void PrintQueue(QueueType* q) {
	printf("Front Pos: %d, Rear Pos: %d\n", q->front, q->rear);
	for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
		if (i <= q->front || i > q->rear) {
			printf("[%d]  |", i);
		}
		else {
			printf("[%d] %d| ", i, q->data[i]);
		}
	}
	printf("\n");
	return;
}

int IsFull(QueueType* q) {
	return (q->rear == MAX_QUEUE_SIZE - 1);
}

int IsEmpty(QueueType* q) {
	return (q->front == q->rear);
}

void Enqueue(QueueType* q, element data) {
	if (IsFull(q)) {
		Error("Queue is Full\n");
		return;
	}

	q->data[++(q->rear)] = data;

	return;
}

element Dequeue(QueueType* q) {
	if (IsEmpty(q)) {
		Error("Queue is Empty\n");
		return -1;
	}

	return (q->data[++(q->front)]);
}
