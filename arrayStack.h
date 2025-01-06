#ifndef ARRAYSTACK_H
#define ARRAYSTACK_H

#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;

typedef struct {
	ElementType data;
}Node;

typedef struct {
	int top;
	int capacity;
	Node* nodes;
}StackType;

void CreateStack(StackType** s, int capacity);
void DestroyStack(StackType** s);
int IsEmpty(StackType* s);
int IsFull(StackType* s);
void Push(StackType* s, ElementType data);
ElementType Pop(StackType* s);
ElementType Peek(StackType* s);
int GetSize(StackType* s);
void ShowStack(StackType* s);

#endif
