//Singly Linked List

#include "SinglyLinkedList.h"

void Error(char* message) {
	fprintf(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}

void Init(ListNode** head) {
	*head = NULL;
}

int IsEmpty(ListNode* head) {
	return (head == NULL);
}

int GetPos(ListNode* head, ElementType data) {
	//리스트가 비었는지 검사
	if (IsEmpty(head)) {
		Error("List is Empty");
	}

	ListNode* ptr;
	ptr = head;
	int Flag = 0;
	int i = 0;

	while (ptr != NULL) {
		if (ptr->data == data) {
			Flag = 1;
			break;
		}
		ptr = ptr->link;
		i++;
	}

	if (Flag) { //index 값을 찾았으면
		return i;
	}
	else {
		printf("해당하는 데이터가 리스트에 없습니다.");
		return -1;
	}
}

int GetSize(ListNode* head) {

	if (IsEmpty(head)) {
		printf("리스트가 비었습니다.\n");
		return 0;
	}

	ListNode* ptr;
	ptr = head;
	int i = 0;

	while (ptr != NULL) {
		ptr = ptr->link;
		i++;
	}

	return i;
}

ListNode* CreateNode(ElementType data) {
	ListNode* newNode;
	newNode = (ListNode*)malloc(sizeof(ListNode));
	if (!newNode) {
		Error("memory allocation failed");
	}
	newNode->data = data;
	newNode->link = NULL;

	return newNode;
}

ListNode* GetNode(ListNode* head, int index) {

	//리스트가 비어있는지 확인
	if (IsEmpty(head)) {
		Error("List is Empty");
	}

	//인덱스 범위 검사
	int size = GetSize(head);
	if (index < 0 || index >= size) {
		Error("인덱스가 리스트의 범위를 초과했습니다.");
	}

	ListNode* ptr;
	ptr = head;
	int i = 0;
	while (ptr != NULL) {
		if (i == index) {
			break;
		}
		ptr = ptr->link;
		i++;
	}

	return ptr;
}

void InsertFirst(ListNode** head, ElementType data) {
	ListNode* newNode = CreateNode(data);

	if (IsEmpty(*head)) { //리스트가 비었으면
		printf("리스트가 비었습니다. 첫 노드가 됩니다.\n");
		*head = newNode;
		return;
	}
	else {//노드가 하나라도 있으면
		newNode->link = *head;
		*head = newNode;
		return;
	}
}

void InsertLast(ListNode** head, ElementType data) {

	ListNode* newNode = CreateNode(data);

	if (IsEmpty(*head)) {//리스트가 비었으면
		printf("리스트가 비었습니다. 첫 노드가 됩니다.\n");
		*head = newNode;
		return;
	}
	else {
		ListNode* ptr;
		ptr = *head;
		while (ptr->link != NULL) {
			ptr = ptr->link;
		}
		//ptr이 마지막 노드를 가리키게 됨
		ptr->link = newNode;
		return;
	}
}

void InsertAfter(ListNode* pre, ElementType data) {
	ListNode* newNode = CreateNode(data);
	newNode->link = pre->link;
	pre->link = newNode;
}

void InsertAt(ListNode** head, int index, ElementType data) {

	//index 범위 검사
	int size = GetSize(*head);
	if (index < 0 || index > size) { //index == size인 경우는, 리스트의 마지막에 삽입하는 경우
		Error("인덱스가 리스트의 범위를 초과했습니다.");
		return;
	}

	ListNode* newNode = CreateNode(data);

	////리스트가 비어있는 경우
	if (IsEmpty(*head)) {
		if (index == 0) {//리스트가 비었고, 0번째에 삽입하는 경우
			printf("리스트가 비었습니다. 첫 노드가 됩니다.\n");
			*head = newNode;
		}
		else {//리스트가 비었는데 index가 0이 아닌 경우
			Error("리스트가 비었습니다. 인덱스 오류");
		}
		return;
	}

	////리스트가 비어있지 않은 경우
	if (index == 0) {//리스트의 맨 처음에 삽입하는 경우: head 포인터만 조작하면 됨
		newNode->link = *head;
		*head = newNode;
		return;
	}
	else { //중간이나 끝에 삽입하는 경우: 선행노드를 찾아야 함
		ListNode* ptr;
		ptr = *head;
		int i = 0;
		while (ptr != NULL) {
			if (i == index - 1) {
				break;
			}
			ptr = ptr->link;
			i++;
		}
		//ptr이 index-1번 노드를 가리키게 됨
		newNode->link = ptr->link;
		ptr->link = newNode;
		return;
	}
}

ElementType DeleteFirst(ListNode** head) {
	//리스트가 비어있는지 검사
	if (IsEmpty(*head)) {
		Error("리스트가 비었습니다. 삭제할 수 없습니다.");
	}

	ElementType removedData;
	ListNode* ptr;

	ptr = *head; //ptr은 첫번째 노드를 가리킴
	removedData = ptr->data;
	*head = (*head)->link;
	free(ptr);

	return removedData;
}

ElementType DeleteLast(ListNode** head) {
	//리스트가 비어있는지 검사
	if (IsEmpty(*head)) {
		Error("리스트가 비었습니다. 삭제할 수 없습니다.");
	}

	ListNode* pre;
	ListNode* removed;
	ElementType removedData;

	pre = *head;
	while (pre->link->link != NULL) {
		pre = pre->link;
	}
	removed = pre->link;

	//pre는 마지막 노드의 선행 노드를 가리킴
	//removed는 마지막 노드를 가리키게 됨
	removedData = removed->data; //마지막 노드의 값 저장
	pre->link = NULL; //마지막 이전 노드의 링크를 NULL로 설정
	free(removed); //마지막 노드 삭제

	return removedData;
}

ElementType DeleteThis(ListNode** head, ElementType data) {
	//리스트가 비어있는지 검사
	if (IsEmpty(*head)) {
		Error("리스트가 비었습니다. 삭제할 수 없습니다.");
	}

	//데이터가 리스트 안에 있는지 확인
	int pos = GetPos(*head, data);
	ElementType removedData;
	if (pos == -1) { //GetPos의 값이 -1인 경우(리스트 안에 해당 데이터가 없는 경우)
		Error("삭제할 수 없습니다.");
	}
	else { //리스트 안에 있다면 pos 위치의 데이터를 삭제
		removedData = DeleteAt(head, pos);
		return removedData;
	}
}

ElementType DeleteAt(ListNode** head, int index) {
	//index의 범위 검사
	int size = GetSize(*head);
	if (index < 0 || index >= size) {
		Error("인덱스가 리스트의 범위를 초과했습니다.");
	}

	//리스트가 비어있는지 검사
	if (IsEmpty(*head)) {
		Error("리스트가 비었습니다. 삭제할 수 없습니다.");
	}

	ElementType removedData;
	ListNode* removed;
	removed = *head; //removed는 맨 첫 노드를 가리킴

	////리스트가 비어있지 않고, 맨 첫 노드를 삭제하는 경우
	if (index == 0) {
		removedData = removed->data;
		*head = (*head)->link;
		free(removed);

		return removedData;
	}

	////리스트가 비어있지 않고, 중간이나 끝 노드를 삭제하는 경우
	ListNode* pre;
	pre = *head;
	int i = 0;
	while (pre != NULL) { //삭제할 노드의 선행노드 알아내기
		if (i == index - 1) {
			break;
		}
		pre = pre->link;
		i++;
	}
	removed = pre->link;

	//pre는 삭제할 i번째 노드의 선행 노드를 가리킴
	//removed는 삭제할 i번째 노드를 가리킴
	removedData = removed->data;
	pre->link = removed->link; //삭제할 노드의 앞노드와 뒤노드를 연결
	free(removed); //i번째 노드 삭제

	return removedData;
}

void ConcatTwo(ListNode** head1, ListNode** head2, ListNode** head3) {

	////리스트3이 비어있는지 확인
	if (!IsEmpty(*head3)) {
		Error("리스트3이 비어있는 리스트가 아닙니다.");
	}

	////리스트1, 2가 비어있는지 확인
	ElementType copiedData;
	int size = 0;

	//리스트1, 2가 둘 다 비어있는 경우
	if (*head1 == NULL && *head2 == NULL) {
		return;
	}//1번 리스트만 비어있는 경우
	else if (*head1 == NULL) {
		size = GetSize(*head2);
		for (int i = 0; i < size; i++) {
			copiedData = GetNode(*head2, i)->data;
			InsertLast(head3, copiedData);
		}

		return;
	}//2번 리스트만 비어있는 경우
	else if (*head2 == NULL) {
		size = GetSize(*head1);
		for (int i = 0; i < size; i++) {
			copiedData = GetNode(*head1, i)->data;
			InsertLast(head3, copiedData);
		}

		return;
	}
	else {//둘 다 빈 리스트가 아닌 경우
		size = GetSize(*head1);
		for (int i = 0; i < size; i++) {
			copiedData = GetNode(*head1, i)->data;
			InsertLast(head3, copiedData);
		}

		size = GetSize(*head2);
		for (int i = 0; i < size; i++) {
			copiedData = GetNode(*head2, i)->data;
			InsertLast(head3, copiedData);
		}

		return;
	}
}

void Reverse(ListNode** head) {

	//리스트가 비었는지 확인
	if (IsEmpty(*head)) {
		printf("리스트가 비어있습니다.\n");
		return;
	}

	ListNode* p, * q, * r; //리스트의 노드를 따라갈 포인터 3개 생성

	p = *head;
	q = NULL;
	while (p != NULL) {
		r = q;
		q = p;
		p = p->link; //p는 처음부터 끝까지 순회하고, r은 q를, q는 p를 차례로 따라간다.
		q->link = r; //q의 링크 방향을 반대로 바꾼다.
	}
	//p는 NULL에 도달, q는 역순으로 바뀐 리스트의 첫 노드를 가리키고, r은 q 다음 노드를 가리키게 됨

	*head = q;

	return;
}

void Clear(ListNode** head) {
	if (*head == NULL) {
		return; // 리스트가 비어있으면 바로 반환
	}

	ListNode* temp = NULL;

	while (*head != NULL) {
		temp = *head;          // 현재 노드를 temp로 저장
		*head = (*head)->link; // head를 다음 노드로 이동
		free(temp);            // temp로 저장된 노드 해제
	}

	*head = NULL; // 모든 노드를 해제한 후 head를 NULL로 설정
	return;
}

void PrintList(ListNode* head) {

	//리스트가 비어있는지 검사
	if (head == NULL) {
		printf("리스트가 비어있어서 출력하지 않습니다.\n\n");
		return;
	}

	ListNode* ptr;
	ptr = head;

	int i = 0;
	while (ptr != NULL) {
		printf("[%d] %d", i, ptr->data);
		if (ptr->link != NULL) { //마지막 노드가 아닌 경우
			printf(" -> ");
		}
		ptr = ptr->link;
		i++;
	}
	printf("\n\n");
}
