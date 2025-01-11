#include "ArrayStack.h"

int main() {
	StackType* s = NULL;
	CreateStack(&s, 5);

	printf("capacity 5인 스택 생성 완료\n");

	printf("10 삽입\n");
	Push(s, 10);

	Push(s, 20);
	printf("20 삽입\n");

	Push(s, 30);
	printf("30 삽입\n");
	
	Push(s, 40);
	printf("40 삽입\n");

	Push(s, 50);
	printf("50 삽입\n");

	Push(s, 60);
	printf("60 삽입\n");

	printf("Capacity: %d, Size: %d, Top: %d\n\n", s->capacity, GetSize(s), Peek(s));

	printf("----------Show Stack----------\n");
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
		printf("\n");
	}

	DestroyStack(&s);

	printf("스택 삭제 확인: %d\n", s);


	return 0;
}
