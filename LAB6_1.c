//산술식을 구성하는 연산자, 피연산자, 괄호는 전체 50개 이하
//space로 구분한다.
//피연산자는 두자리수 이상도 가능
//사용하는 연산자는 +, -, /, *, %
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

typedef enum { lparen, plus, minus, times, divide, mod } Oper;

typedef Oper ElementType;

typedef struct {
	ElementType data;
}Node;

typedef struct {
	int top;
	int capacity;
	Node* nodes;
}StackType;

//함수 선언
/*--------------------------------------------------------*/
void CreateStack(StackType** s);
void DestroyStack(StackType** s);
int IsEmpty(StackType* s);
int IsFull(StackType* s);
void Push(StackType* s, ElementType data);
ElementType Pop(StackType* s);
ElementType Peek(StackType* s);
void ReadFile(char* FileName);
void PrintExp(char* Exp);
void InfixToPostfix();
Oper SymbolToOper(char op);
char OperToSymbol(Oper op);
int GetPrec(Oper op);
int CalPostfix();
void append(char* str, char ch);
/*--------------------------------------------------------*/

int main(int argc, char* argv[]) {

	////0. 인자 검사
	if (argc < 2) { //argv[0]: 프로그램 실행 경로, argv[1]: input.txt
		fprintf(stderr, "[Usage] %s <input> \n", argv[0]);
		return 1;
	}

	////1. input.txt: ( 4 * ( 2 * 1 - 2 ) ) + ( 3 % 2 ) - ( 4 / ( 8 - 6 ) )  

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
void CreateStack(StackType** s) {

	//create Stack
	(*s) = (StackType*)malloc(sizeof(StackType));
	if ((*s) == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

	//initialize
	(*s)->capacity = 10;
	(*s)->top = -1;

	//allocate memory for Nodes
	(*s)->nodes = (Node*)malloc((*s)->capacity * sizeof(Node));
	if ((*s)->nodes == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

	return;
}

void DestroyStack(StackType** s) {

	//free allocated nodes
	free((*s)->nodes);
	(*s)->nodes = NULL; //메모리를 해제한 후 포인터를 NULL로 설정하는 습관을 들이면, 이후 이중 해제(Bug)를 방지할 수 있다.

	//free allocated stack
	free(*s);
	(*s) = NULL;

	return;
}

int IsEmpty(StackType* s) {
	return (s->top == -1);
}

int IsFull(StackType* s) {
	return (s->top == s->capacity - 1);
}

void Push(StackType* s, ElementType data) {
	if (IsFull(s)) {
		s->capacity *= 2;
		s->nodes = (Node*)realloc(s->nodes, s->capacity * sizeof(Node));
		if (s->nodes == NULL) {
			fprintf(stderr, "Memory reallocation failed\n");
			exit(1);
		}
	}

	s->nodes[++(s->top)].data = data;
}

ElementType Pop(StackType* s) {
	if (IsEmpty(s)) {
		fprintf(stderr, "Stack is empty\n");
		exit(1);
	}

	return s->nodes[(s->top)--].data;
}

ElementType Peek(StackType* s) {
	if (IsEmpty(s)) {
		fprintf(stderr, "Stack is empty\n");
		exit(1);
	}

	return s->nodes[s->top].data;
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
	//Infix 수식을 복사해서 사용
	char InfixCopy[MAX_LEN];
	strcpy(InfixCopy, Infix);

	// 연산자의 enum 값(Oper 타입)을 담을 스택 생성
	StackType* s = NULL;
	CreateStack(&s);

	char* token = strtok(InfixCopy, " ");// 공백 기준으로 토큰화
	Oper op;

	while (token != NULL) { // 모든 토큰 처리
		if (isdigit(token[0])) { //숫자(피연산자)인 경우: 바로 Postfix에 출력
			strcat(Postfix, token); //Postfix에 숫자 출력
			append(Postfix, ' '); //숫자 구분을 위한 공백 추가
		}
		else if (strcmp(token, "(") == 0) { //왼쪽 괄호인 경우: 무조건 스택에 삽입
			op = SymbolToOper(*token);
			Push(s, op);
		}
		else if (strcmp(token, ")") == 0) { //오른쪽 괄호를 만나면 
			// 왼쪽 괄호를 만날때까지 스택에서 연산자를 꺼내서(Pop) Postfix에 출력
			op = Pop(s);
			while (op != lparen) {
				append(Postfix, OperToSymbol(op));
				append(Postfix, ' '); //연산자 구분을 위한 공백 추가
				op = Pop(s);
			}
		}
		else { //연산자인 경우: 우선순위를 고려하여 스택에 삽입
			while (!IsEmpty(s) && (GetPrec(SymbolToOper(*token)) <= GetPrec(Peek(s)))) {
				append(Postfix, OperToSymbol(Pop(s)));
				append(Postfix, ' '); //연산자 구분을 위한 공백 추가
			}

			Push(s, SymbolToOper(*token));
		}

		token = strtok(NULL, " "); //다음 토큰
	}

	//스택에 남아있는 연산자들 모두 Postfix에 출력
	while (!IsEmpty(s)) {
		append(Postfix, OperToSymbol(Pop(s)));
		append(Postfix, ' '); //연산자 구분을 위한 공백 추가
	}

	//Postfix의 맨 끝을 '\0'로 설정
	len = strlen(Postfix);
	if (len > 0 && Postfix[len - 1] == ' ') {
		Postfix[len - 1] = '\0';
	}

	//스택 사용이 끝나면 삭제
	DestroyStack(&s);
}

Oper SymbolToOper(char op) {
	switch (op) {
		case '(': return lparen;
		case '+': return plus;
		case '-': return minus;
		case '*': return times;
		case '/': return divide;
		case '%': return mod;
	}

	return -1;
}

char OperToSymbol(Oper op) {
	switch (op) {
		case lparen: return '(';
		case plus: return '+';
		case minus: return '-';
		case times: return '*';
		case divide: return '/';
		case mod: return '%';
	}
}

int GetPrec(Oper op) {
	switch (op) {
		case lparen: return 0;
		case plus: case minus: return 1;
		case times: case divide: case mod: return 2;
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
	//Postfix 수식을 복사해서 사용
	char PostfixCopy[MAX_LEN];
	strcpy(PostfixCopy, Postfix);

	// 피연산자(int 타입)를 담을 스택 생성
	StackType* s = NULL;
	CreateStack(&s);

	char* token = strtok(PostfixCopy, " "); //공백 기준으로 토큰화
	int op1, op2, value;

	while (token != NULL) {
		if (isdigit(token[0])) { //숫자(피연산자)인 경우: 스택에 삽입
			value = atoi(token); //문자열을 정수로 변환한 뒤 삽입
			Push(s, value);
		}
		else { //연산자인 경우: 스택 상단에서 피연산자 두개를 꺼내서 연산자로 연산, 결과를 다시 스택에 삽입
			op2 = Pop(s);
			op1 = Pop(s);

			if (*token == '+') {
				Push(s, op1 + op2);
			}
			else if (*token == '-') {
				Push(s, op1 - op2);
			}
			else if (*token == '*') {
				Push(s, op1 * op2);
			}
			else if (*token == '/') {
				Push(s, op1 / op2);
			}
			else if (*token == '%') {
				Push(s, op1 % op2);
			}
		}

		token = strtok(NULL, " "); //다음 토큰
	}

	//스택에 남아있는 연산 최종결과를 저장하여 리턴
	int result = Pop(s);

	//스택 사용이 끝나면 삭제
	DestroyStack(&s);

	return result;
}

void append(char* str, char ch) {
	int len = strlen(str);

	str[len] = ch;
	str[len + 1] = '\0';

	return;
}
/*--------------------------------------------------------*/
