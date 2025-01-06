//미로찾기 프로그램
//0은 갈수있는 곳, 1은 벽
//대각선 이동 가능
//스택을 정적으로 생성

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

/*---------------------------------------------*/
//Maze의 최대 크기는 20*20
#define MAX_MAZE_HEIGHT 20
#define MAX_MAZE_WIDTH 20

int** Maze = NULL;
//Mark: 방문 여부를 기록. 0:미방문 1:방문
int** Mark = NULL;
int height = 0;
int width = 0;
int contents[MAX_MAZE_HEIGHT*MAX_MAZE_WIDTH];
//n*n maze의 EXIT는 (n,n)
int ExitRow = 0;
int ExitCol = 0;

typedef struct {
	short int vertical;
	short int horizon;
}Offset;

//Direction: 8가지 이동방향(N,NE,E,SE,S,SW,W,NW)
enum Direction { N, NE, E, SE, S, SW, W, NW };
Offset move[8] = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };

//길을 찾았는지 여부
#define TRUE 1
#define FALSE 0
int Found = FALSE; 
/*---------------------------------------------*/

/*---------------------------------------------*/
//스택의 최대 크기: 20*20
#define MAX_STACK_SIZE 400

typedef struct {
	short int row;
	short int col;
	short int dir;
}ElementType;

typedef struct {
	ElementType data;
}Node;

typedef struct {
	int top;
	Node nodes[MAX_STACK_SIZE];
}StackType;
/*---------------------------------------------*/

/*---------------------------------------------*/
void ReadFile(const char* fileName);
void CreateMaze();
void ShowMaze();
void DeleteMaze(StackType** s);
StackType* SearchPath();
void PrintPath(StackType* s);
void CreateStack(StackType** s);
int IsEmpty(StackType* s);
int IsFull(StackType* s);
void Push(StackType* s, ElementType data);
ElementType Pop(StackType* s);
int GetSize(StackType* s);
/*---------------------------------------------*/

int main(int argc, char* argv[]) {

	//1. 인자 검사
	if (argc < 2) { //argv[0]: 프로그램 실행 경로, argv[1]: input.txt
		fprintf(stderr, "[Usage] %s <파일 이름>\n", argv[0]);
		return 1;
	}

	//2. input 파일 읽고 Maze 정보 저장
	ReadFile(argv[1]);

	//3. Maze 생성
	CreateMaze();

	//4. Maze 출력
	ShowMaze();

	//5. 길 찾기
	StackType* s = SearchPath();

	//6. 길찾기 성공 여부와 함께 경로 출력
	PrintPath(s);

	//7. Maze 지우기
	DeleteMaze(&s);

	//8. 확인
	ShowMaze();

	return 0;
}

void ReadFile(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (!file) {
		perror("file open failed\n");
		exit(EXIT_FAILURE);
	}

	//Maze 정보 저장
	fscanf(file, "%d %d", &height, &width);
	for (int i = 0; i < height * width; i++) {
		int c;
		fscanf(file, "%d", &c);
		contents[i] = c;
	}
	ExitRow = height;
	ExitCol = width;

	//파일 닫기
	fclose(file);

	return;
}

void CreateMaze() { //Maze 정보를 확인하고 Maze를 생성하는 함수
	//Maze 사이즈 확인
	if (height == 0 || width == 0) {
		fprintf(stderr, "Maze 정보가 입력되지 않았습니다.\n");
		return;
	}
	else if (height > MAX_MAZE_HEIGHT || width > MAX_MAZE_WIDTH) {
		fprintf(stderr, "Maze 크기는 최대 20*20 입니다.\n");
		return;
	}
	
	//(row+2) x (col+2) 사이즈의 Maze와 Mark를 만든다.(동적할당)
	Maze = (int**)malloc((height + 2) * sizeof(int*));
	for (int i = 0; i < height +2; i++) {
		Maze[i] = (int*)malloc((width +2) * sizeof(int));
	}

	Mark = (int**)malloc((height + 2) * sizeof(int*));
	for (int i = 0; i < height + 2; i++) {
		Mark[i] = (int*)malloc((width + 2) * sizeof(int));
	}

	//초기화(Maze&Mark)
	for (int i = 0; i < height +2; i++) {
		for (int j = 0; j < width +2; j++) {
			Maze[i][j] = 1;
			Mark[i][j] = FALSE;
		}
	}

	//Maze 내용 채우기
	int k = 0;
	for (int i = 1; i <= height; i++) {
		for (int j = 1; j <= width; j++) {
			Maze[i][j] = contents[k++];
		}
	}

	fprintf(stdout, "%d x %d Maze 생성 완료.\n", height, width);

	return;
}

void ShowMaze() {
	//Maze가 생성되었는지 확인
	if (!Maze) {
		fprintf(stderr, "Maze가 생성되지 않았습니다.\n");
		return;
	}

	int count = 0;
	for (int i = 1; i <= width*2; i++) {
		printf("=");
	}
	printf("\n");
	for (int i = 1; i <= width; i++) {
		for (int j = 1; j <= width; j++) {
			printf("%d ", Maze[i][j]);
			
			count++;
			if (count % width == 0) {
				printf("\n");
			}
			
		}
	}
	for (int i = 1; i <= width*2; i++) {
		printf("=");
	}
	printf("\n");
}

void DeleteMaze(StackType** s) {
	if (!Maze) {
		fprintf(stdout, "Maze가 생성되지 않았습니다.\n");
		return;
	}
	else {
		//Maze 메모리 해제
		for (int i = 0; i < height + 2; i++) {
			free(Maze[i]);
		}
		free(Maze);
		Maze = NULL;

		//Mark 메모리 해제
		for (int i = 0; i < height + 2; i++) {
			free(Mark[i]);
		}
		free(Mark);
		Mark = NULL;
		
		//스택 메모리 해제
		free(*s);
		*s = NULL;

		fprintf(stdout, "Maze가 제거되었습니다.\n");
		return;
	}
}

StackType* SearchPath() {

	//길을 찾기 전 Found를 초기화
	Found = FALSE;

	int row; int col;
	int nextRow, nextCol;
	int dir;

	//경로 node를 담을 스택 생성
	StackType* s = NULL;
	CreateStack(&s);

	ElementType position;
	Mark[1][1] = 1; //시작 위치는 (1,1)
	position.row = 1;
	position.col = 1;
	position.dir = N;
	Push(s, position);

	while (!IsEmpty(s) && !Found) {
		//스택에서 요소(row,col,dir) 하나 꺼내서 현재 위치로 설정
		position = Pop(s);
		row = position.row;
		col = position.col;
		dir = position.dir;

		//dir:N부터 NW까지 총 8개(0~7)
		//방향을 돌려가며 갈 수 있는 곳 탐색
		while (dir < 8 && !Found) {
			//다음에 갈 곳 설정(현재 좌표 + move[dir]): dir 방향으로 이동
			nextRow = row + move[dir].vertical;
			nextCol = col + move[dir].horizon;

			//next가 EXIT인 경우: 탈출경로 찾음. 탐색 종료
			if (nextRow == ExitRow && nextCol == ExitCol) {
				
				//EXIT 직전 위치(현재 위치) 추가
				position.row = row;
				position.col = col;
				position.dir = dir;
				Push(s, position);

				//EXIT 위치(next 위치) 추가
				position.row = nextRow;
				position.col = nextCol;
				position.dir = dir;
				Push(s, position);

				Found = TRUE;
			}//next가 EXIT는 아니지만 갈 수 있는 경우(벽이 아니고, 아직 방문하지않음)
			else if (!Maze[nextRow][nextCol] && !Mark[nextRow][nextCol]) {
				Mark[nextRow][nextCol] = TRUE; //방문 표시

				//현재 위치를 스택에 push 해놓는다.
				//이때 ++dir을 하는 것은 이미 이 경로로는 방문을 했으니
				//다음번에 이 위치로 돌아왔을때 다른 방향으로 갈 수 있도록 해주는 것
				position.row = row;
				position.col = col;
				position.dir = ++dir;
				Push(s, position);

				//이동. 현재위치인 row와 col을 초기화
				//dir은 0(N)으로 초기화
				row = nextRow;
				col = nextCol;
				dir = N;
			}
			else { // 다 아닌 경우: 못감. 방향을 바꿔야함(스택에 push하지 않는다!!)
				++dir;
			}
		}
	}

	//길 찾기 종료

	return s;
}

void PrintPath(StackType* s) {
	if (Found) {
		printf("found the Path!!\n\n");
		printf("path is: ");
		for (int i = 0; i <= s->top; i++) {
			printf("[%d,%d] ", s->nodes[i].data.row, s->nodes[i].data.col);
		}
		printf("\n");

		printf("사용한 스택의 크기 / 스택의 최대 크기 = %d/%d\n", GetSize(s), height * width);
	}
	else
	{
		printf("the Maze doesn't have a Path.\n\n");
	}

	return;
}

void CreateStack(StackType** s) {
	//스택 생성
	(*s) = (StackType*)malloc(sizeof(StackType));
	
	//스택 초기화
	(*s)->top = -1;
}

int IsEmpty(StackType* s) {
	return (s->top == -1);
}

int IsFull(StackType* s) {
	return (s->top == MAX_STACK_SIZE - 1);
}

void Push(StackType* s, ElementType data) {
	if (IsFull(s)) {
		fprintf(stderr, "Stack is full\n");
		return;
	}
	s->nodes[++(s->top)].data = data;
}

ElementType Pop(StackType* s) {
	if (IsEmpty(s)) {
		fprintf(stderr, "Stack is empty\n");
		exit(EXIT_FAILURE);
	}

	return s->nodes[(s->top)--].data;
}

int GetSize(StackType* s) {
	return (s->top + 1);
}
