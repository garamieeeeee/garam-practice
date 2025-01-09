//원형 큐 구현(1차원 배열 사용, 정적으로 큐 생성)

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
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
int IsFull(QueueType* q);
int IsEmpty(QueueType* q);
void PrintQueue(QueueType* q);
void Enqueue(QueueType* q, element data);
element Dequeue(QueueType* q);
element Peek(QueueType* q);

int main() {
	QueueType q;
	InitQueue(&q);
	int data;

	printf("---데이터 추가 단계---\n");
	while (!IsFull(&q)) {
		printf("정수를 입력하세요: ");
		scanf("%d", &data);
		Enqueue(&q, data);
		PrintQueue(&q);
	}
	printf("큐는 포화상태 입니다.\n\n");

	printf("---데이터 삭제 단계---\n");
	while (!IsEmpty(&q)) {
		data = Dequeue(&q);
		printf("꺼내진 정수: %d\n", data);
		PrintQueue(&q);
	}
	printf("큐는 공백상태 입니다.\n\n");

	return 0;
}

void Error(char* message) {
	fprintf(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}

void InitQueue(QueueType* q) {
	q->front = 0;
	q->rear = 0;

	return;
}

int IsFull(QueueType* q) {
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

int IsEmpty(QueueType* q) {
	return (q->front == q->rear);
}

void PrintQueue(QueueType* q) {
	printf("Front Pos: %d, Rear Pos: %d\n", q->front, q->rear);
	if (!IsEmpty(q)) {
		int i = q->front;
		do {
			i = (i + 1) % MAX_QUEUE_SIZE;
			printf("[%d] %d | ", i, q->data[i]);
			if (i == q->rear) {
				break;
			}
		} while (i != q->front);
	}
	printf("\n");
}

void Enqueue(QueueType* q, element data) {
	if (IsFull(q)) {
		Error("Queue is Full\n");
		return;
	}

	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
	q->data[q->rear] = data;

	return;
}

element Dequeue(QueueType* q) {
	if (IsEmpty(q)) {
		Error("Queue is Empty\n");
		return -1;
	}

	q->front = (q->front + 1) % MAX_QUEUE_SIZE;
	return q->data[q->front];
}

element Peek(QueueType* q) {
	if (IsEmpty(q)) {
		Error("Queue is Empty\n");
		return -1;
	}

	return q->data[(q->front+1) % MAX_QUEUE_SIZE];
}
