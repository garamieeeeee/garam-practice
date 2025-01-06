#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	void* data;
	int elementSize;
}Node;

typedef struct {
	int capacity;
	int top;
	Node* nodes;
}StackType;

void CreateStack(StackType** s, int elementSize);
void DestroyStack(StackType** s);
void Push(StackType* s, void* data);
void* Pop(StackType* s);
void* Peek(StackType* s);
int IsFull(StackType* s);
int IsEmpty(StackType* s);

int main() {

	//스택 생성
	StackType* s_i = NULL; //int형 스택 포인터 s_i
	StackType* s_c = NULL; //char형 스택 포인터 s_c
	StackType* s_f = NULL; //float형 스택 포인터 s_f

	CreateStack(&s_i, sizeof(int));
	CreateStack(&s_c, sizeof(char));
	CreateStack(&s_f, sizeof(float));

	int value_i;
	char value_c;
	float value_f;

	//삽입(Push)
	for (int i = 0; i < 5; i++) {
		value_i = i * 2;
		Push(s_i, &value_i);
	}

	for (int i = 0; i < 5; i++) {
		value_c = 'a' + i;
		Push(s_c, &value_c);
	}

	for (int i = 0; i < 5; i++) {
		value_f = i * (0.2);
		Push(s_f, &value_f);
	}

	//출력(Pop)
	printf("int형 스택 출력: \n");
	for (int i = s_i->top; i >= 0; i--) {
		value_i = *(int*)Pop(s_i);
		printf("%2d [%d]\n", value_i, i);
	}
	printf("\n");

	printf("char형 스택 출력: \n");
	for (int i = s_c->top; i >= 0; i--) {
		value_c = *(char*)Pop(s_c);
		printf("%c [%d]\n", value_c, i);
	}
	printf("\n");

	printf("float형 스택 출력: \n");
	for (int i = s_f->top; i >= 0; i--) {
		value_f = *(float*)Pop(s_f);
		printf("%f [%d]\n", value_f, i);
	}
	printf("\n");

	//스택 메모리 해제
	DestroyStack(&s_i);
	DestroyStack(&s_c);
	DestroyStack(&s_f);

	return 0;
}

void CreateStack(StackType** s, int elementSize) {
	(*s) = (StackType*)malloc(sizeof(StackType));
	if ((*s) == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

	(*s)->capacity = 10;//스택의 초기 용량은 10으로 설정
	(*s)->top = -1;

	(*s)->nodes = (Node*)malloc((*s)->capacity * sizeof(Node));
	if ((*s)->nodes == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

	//자료형 크기 저장
	for (int i = 0; i < (*s)->capacity; i++) {
		(*s)->nodes[i].data = malloc(elementSize); //자료형 크기만큼 메모리 할당
		(*s)->nodes[i].elementSize = elementSize;
	}

	return;
}

void DestroyStack(StackType** s) {
	for (int i = 0; i < (*s)->capacity; i++) {
		free((*s)->nodes[i].data);
	}
	free((*s)->nodes);

	free(*s);
	(*s) = NULL;

	return;
}

void Push(StackType* s, void* data) {
	if (IsFull(s)) {
		s->capacity *= 2; //용량을 두배로 늘리고 메모리 재할당
		s->nodes = realloc(s->nodes, s->capacity * sizeof(Node));

		if (s->nodes == NULL) {
			fprintf(stderr, "Memory reallocation failed\n");
			exit(1);
		}

		for (int i = s->top + 1; i < s->capacity; i++) {
			s->nodes[i].data = malloc(s->nodes[0].elementSize);
			if (s->nodes[i].data == NULL) {
				fprintf(stderr, "Memory allocation failed for new node\n");
				exit(1);
			}

			s->nodes[i].elementSize = s->nodes[0].elementSize;
		}
	}

	memcpy(s->nodes[++(s->top)].data, data, s->nodes[s->top].elementSize);//전달된 데이터를 메모리 복사로 저장한다.

	return;
}

void* Pop(StackType* s) {
	if (IsEmpty(s)) {
		fprintf(stderr, "Stack is empty\n");
		exit(1);
	}

	return s->nodes[(s->top)--].data;
}

void* Peek(StackType* s) {
	if (IsEmpty(s)) {
		fprintf(stderr, "Stack is empty\n");
		exit(1);
	}

	return s->nodes[s->top].data;
}

int IsFull(StackType* s) {
	return (s->top == s->capacity - 1);
}

int IsEmpty(StackType* s) {
	return (s->top == -1);
}
