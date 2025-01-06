//산술식을 구성하는 연산자, 피연산자, 괄호는 전체 50개 이하
//space로 구분한다.
//피연산자는 두자리수 이상도 가능
//사용하는 연산자는 +, -, /, *, %, &&, ||, ==, !=, <, >, <=, >=
//input.txt를 읽어 산술식을 변환하고 계산

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100
char Infix[MAX_LEN] = { '\0' };
char Postfix[MAX_LEN] = { '\0' };

typedef struct {
	void* data;
	int elementSize;
}Node;

typedef struct {
	int capacity;
	int top;
	Node* nodes;
}StackType;

//함수 선언
/*--------------------------------------------------------*/
void CreateStack(StackType** s, int elementSize);
void DestroyStack(StackType** s);
void Push(StackType* s, void* data);
void* Pop(StackType* s);
void* Peek(StackType* s);
int IsFull(StackType* s);
int IsEmpty(StackType* s);
void ReadFile(char* FileName);
void PrintExp(char* Exp);
void InfixToPostfix();
int GetPrec(char* op);
int CalPostfix();
void append(char* str, char ch);
/*--------------------------------------------------------*/

int main(int argc, char* argv[]) {

	////0. 인자 검사
	if (argc < 2) { //argv[0]: 프로그램 실행 경로, argv[1]: input.txt
		fprintf(stderr, "[Usage] %s <input.txt> \n", argv[0]);
		return 1;
	}

	////1. input.txt:   ( 3 != 9 ) && ( 4 >= 9 ) 
	//Infix 수식 읽고 저장
	ReadFile(argv[1]);

	//Infix 확인
	printf("INFIX: ");
	PrintExp(Infix);

	//수식 변환
	InfixToPostfix();

	//Postfix 확인
	printf("POSTFIX: ");
	PrintExp(Postfix);

	//Postfix 계산
	printf("계산 결과: ");
	printf("%d\n", CalPostfix());
	printf("\n");

	return 0;
}

//스택 관련 함수
/*--------------------------------------------------------*/
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
/*--------------------------------------------------------*/

//프로그램 관련 함수
/*--------------------------------------------------------*/
void ReadFile(char* FileName) {
	FILE* file = fopen(FileName, "r");
	if (!file) {
		fprintf(stderr, "파일을 여는데 실패했습니다.\n");
		exit(EXIT_FAILURE);
	}

	fgets(Infix, MAX_LEN, file); //한줄 전체 읽기
	fclose(file);

	//공백을 제외한 문자 수 확인
	int count = 0;
	for (int i = 0; Infix[i] != '\0'; i++) {
		if (Infix[i] != ' ' && Infix[i] != '\n') { //공백, 줄바꿈 제외
			count++;
		}
	}

	if (count > 50) {
		printf("산술식을 구성하는 연산자, 피연산자(10이하 양의 정수), 괄호는 전체 50개 이하여야 합니다.\n");
		exit(EXIT_FAILURE);
	}

	fclose(file);

	return;
}

void PrintExp(char* Exp) {
	int len = strlen(Exp);

	for (int i = 0; i < len; i++) {
		printf("%c", Exp[i]);
	}
	printf("\n");

	return;
}

void InfixToPostfix() {

	//먼저 Infix가 입력되었는지를 확인
	int len = strlen(Infix);

	if (len == 0) {
		printf("Infix 수식이 입력되지 않았습니다.\n");
		return;
	}

	////수식 변환 시작
	// 연산자를 담을 스택 생성
	StackType* s_c = NULL;
	CreateStack(&s_c, sizeof(char[3])); //두글자 연산자를 위해 크기 변경

	char* token = strtok(Infix, " ");// 공백 기준으로 토큰화
	char temp[3]; //두글자 연산자까지 저장 가능
	char top_op[3];

	while (token != NULL) { // 모든 토큰 처리
		if (isdigit(token[0])) { //숫자(피연산자)인 경우: 바로 Postfix에 출력
			strcat(Postfix, token); //Postfix에 숫자 출력
			append(Postfix, ' '); //숫자 구분을 위한 공백 추가
		}
		else if (strcmp(token, "(") == 0) { //왼쪽 괄호인 경우: 무조건 스택에 삽입
			strcpy(temp, token);
			Push(s_c, temp);
		}
		else if (strcmp(token, ")") == 0) { //오른쪽 괄호를 만나면 
			// 왼쪽 괄호를 만날때까지 스택에서 연산자를 꺼내서(Pop) Postfix에 출력
			strcpy(top_op, (char*)Pop(s_c));
			while (strcmp(top_op, "(") != 0) {
				strcat(Postfix, top_op);
				append(Postfix, ' '); //연산자 구분을 위한 공백 추가
				strcpy(top_op, (char*)Pop(s_c));
			}
		}
		else { //연산자인 경우: 우선순위를 고려하여 스택에 삽입
			while (!IsEmpty(s_c) && (GetPrec(token) <= GetPrec((char*)Peek(s_c)))) {
				strcat(Postfix, (char*)Pop(s_c));
				append(Postfix, ' '); //연산자 구분을 위한 공백 추가
			}

			strcpy(temp, token);
			Push(s_c, temp);
		}

		token = strtok(NULL, " "); //다음 토큰
	}

	//스택에 남아있는 연산자들 모두 Postfix에 출력
	while (!IsEmpty(s_c)) {
		strcat(Postfix, (char*)Pop(s_c));
		append(Postfix, ' '); //연산자 구분을 위한 공백 추가
	}

	//스택 사용이 끝나면 삭제
	DestroyStack(&s_c);
}

int GetPrec(char* op) {

	if (*op == '(') {
		return 0;
	}
	else if (strcmp(op, "||") == 0) {
		return 1;
	}
	else if (strcmp(op, "&&") == 0) {
		return 2;
	}
	else if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
		return 3;
	}
	else if (*op == '<' || *op == '>' || (strcmp(op, "<=") == 0) || (strcmp(op, ">=") == 0)) {
		return 4;
	}
	else if (*op == '+' || *op == '-') {
		return 5;
	}
	else if (*op == '*' || *op == '/' || *op == '%') {
		return 6;
	}

	return -1;
}

int CalPostfix() {

	//먼저 Postfix가 입력되었는지를 확인
	int len = strlen(Postfix);

	if (len == 0) {
		printf("Postfix 수식이 입력되지 않았습니다.\n");
		exit(EXIT_FAILURE);
	}

	////Postfix 계산 시작
	// 피연산자(int 타입)를 담을 스택 생성
	StackType* s_i = NULL;
	CreateStack(&s_i, sizeof(int));

	char* token = strtok(Postfix, " "); //공백 기준으로 토큰화
	int op1, op2, value;
	int temp;

	while (token != NULL) {
		if (isdigit(token[0])) { //숫자(피연산자)인 경우
			value = atoi(token); //문자열을 정수로 변환
			Push(s_i, &value);
		}
		else { //연산자인 경우
			op2 = *(int*)Pop(s_i);
			op1 = *(int*)Pop(s_i);

			if (*token == '+') {
				temp = op1 + op2;
			}
			else if (*token == '-') {
				temp = op1 - op2;
			}
			else if (*token == '*') {
				temp = op1 * op2;
			}
			else if (*token == '/') {
				temp = op1 / op2;
			}
			else if (*token == '%') {
				temp = op1 % op2;
			}
			else if (strcmp(token, "&&") == 0) {
				temp = (op1 && op2); //결과는 true(1)이거나 false(0) 둘중에 하나
			}
			else if (strcmp(token, "||") == 0) {
				temp = (op1 || op2); //결과는 true(1)이거나 false(0) 둘중에 하나
			}
			else if (strcmp(token, "==") == 0) {
				temp = (op1 == op2); //결과는 true(1)이거나 false(0) 둘중에 하나
			}
			else if (strcmp(token, "!=") == 0) {
				temp = (op1 != op2); //결과는 true(1)이거나 false(0) 둘중에 하나
			}
			else if (*token == '<') {
				temp = (op1 < op2); //결과는 true(1)이거나 false(0) 둘중에 하나
			}
			else if (*token == '>') {
				temp = (op1 > op2); //결과는 true(1)이거나 false(0) 둘중에 하나
			}
			else if (strcmp(token, "<=") == 0) {
				temp = (op1 <= op2); //결과는 true(1)이거나 false(0) 둘중에 하나
			}
			else if (strcmp(token, ">=") == 0) {
				temp = (op1 >= op2); //결과는 true(1)이거나 false(0) 둘중에 하나
			}

			Push(s_i, &temp);
		}

		token = strtok(NULL, " "); //다음 토큰
	}

	//스택에 남아있는 연산 최종결과를 저장하여 리턴
	int result = *(int*)Pop(s_i);

	//스택 사용이 끝나면 삭제
	DestroyStack(&s_i);

	return result;
}

void append(char* str, char ch) {
	int len = strlen(str);

	str[len] = ch;
	str[len + 1] = '\0';

	return;
}
/*--------------------------------------------------------*/
