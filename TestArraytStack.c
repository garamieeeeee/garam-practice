#include "ArrayStack.h"

int main() {
	StackType* s = NULL;

	CreateStack(&s, 5);

	Push(s, 10);
	Push(s, 20);
	Push(s, 30);
	Push(s, 40);
	Push(s, 50);
	Push(s, 60);

	printf("Capacity: %d, Size: %d, Top: %d\n\n", s->capacity, GetSize(s), Peek(s));

	printf("Show Stack..\n");
	ShowStack(s);

	while (1) {
		if (IsEmpty(s)) {
			break;
		}

		printf("Popped: %d\n", Pop(s));

		if (!IsEmpty(s)) {
			printf("Current Top: %d\n", Peek(s));
		}
		else {
			printf("Stack is Empty.\n");
		}
	}

	DestroyStack(&s);

	printf("스택 삭제 확인: %d\n", s);


	return 0;
}
