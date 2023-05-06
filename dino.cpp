//[C/C++ game] very simple google dinosaur. (by. BlockDMask)
//2019.12.03 (v2.0)점수 추가, 충돌처리 추가.
//소스코드 해석 & 바꿔보기 과제~
#include<stdio.h> //헤더파일 stdio.h 추가
#include<windows.h> //헤더파일 windows.h 추가
#include<conio.h> //헤더파일 conio.h 추가
#include<time.h> //헤더파일 time.h 추가
#define DINO_BOTTOM_Y 12  //공룡 캐릭터가 땅에서 멈춰있는 위치를 나타냄
#define TREE_BOTTOM_Y 20  //나무가 나타나는 위치의 행(row) 값을 나타냄
#define TREE_BOTTOM_X 45  //나무가 나타나는 위치의 열(column) 값을 나타냄

//콘솔 창의 크기와 제목을 지정하는 함수
void SetConsoleView() //콘솔 창에 대한 초기 설정을 수행
{
	system("mode con:cols=100 lines=25"); //게임 콘솔 창의 각각 가로와 세로를 정한다
	system("title Google Dinosaurs. By BlockDMask."); //콘솔 창의 이름을 정한다
}

//커서의 위치를 x, y로 이동하는 함수
void GotoXY(int x, int y) //x와 y 좌표값을 받음
{
	COORD Pos; //x, y를 가지고 있는 구조체
	Pos.X = 2 * x; //커서를 이동시킬 열(column)의 위치를 지정, 2를 곱해 출력되는 수가 한글이나 2바이트 문자를 출력할 때 올바른 위치로 커서가 이동할 수 있도록 보정
	Pos.Y = y; //커서를 이동시킬 행(row)의 위치를 지정
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos); // 커서의 위치를 지정한 좌표값(Pos)으로 이동
}

//키보드의 입력을 받고, 입력된 키의 값을 반환하는 함수
int GetKeyDown()
{
	if (_kbhit() != 0) //키가 눌렸을 경우에 실행
	{
		return _getch(); //키보드에서 입력한 값을 반환
	}
	return 0;
}

//공룡을 그리는 함수
void DrawDino(int dinoY)
{
	GotoXY(0, dinoY); // 공룡의 x좌표는 0이다
	static bool legFlag = true; // legFlag는 참이다
	printf("        $$$$$$$ \n"); //42-52줄 공룡 모형 출력
	printf("       $$ $$$$$$\n");
	printf("       $$$$$$$$$\n");
	printf("$      $$$      \n");
	printf("$$     $$$$$$$  \n");
	printf("$$$   $$$$$     \n");
	printf(" $$  $$$$$$$$$$ \n");
	printf(" $$$$$$$$$$$    \n");
	printf("  $$$$$$$$$$    \n");
	printf("    $$$$$$$$    \n");
	printf("     $$$$$$     \n");
	if (legFlag) //legFlag이면
	{
		printf("     $    $$$    \n"); //공룡발 모형 출력
		printf("     $$          ");
		legFlag = false; //legFlag가 거짓이므로 
	}
	else //아래의 모형이 출력된다
	{
		printf("     $$$  $     \n"); //공룡발 모형 출력
		printf("          $$    ");
		legFlag = true; //legFlag는 참이다
	}
}

//나무를 그리는 함수
void DrawTree(int treeX)
{
	GotoXY(treeX, TREE_BOTTOM_Y); //나무의 좌표(treeX,20)
	printf("$$$$"); //해당 좌표에 나무일부 출력
	GotoXY(treeX, TREE_BOTTOM_Y + 1); //나무의 좌표(treeX,21)
	printf(" $$ "); //해당 좌표에 나무일부 출력
	GotoXY(treeX, TREE_BOTTOM_Y + 2); //나무의 좌표(treeX,22)
	printf(" $$ "); //해당 좌표에 나무일부 출력
	GotoXY(treeX, TREE_BOTTOM_Y + 3); //나무의 좌표(treeX,23)
	printf(" $$ "); //해당 좌표에 나무일부 출력
	GotoXY(treeX, TREE_BOTTOM_Y + 4); //나무의 좌표(treeX,24)
	printf(" $$ "); //해당 좌표에 나무일부 출력
}

//(v2.0) 충돌 했을때 게임오버 그려줌
void DrawGameOver(const int score) //게임 오버 화면을 나타내는 함수
{
	system("cls"); // 콘솔 화면을 지움
	//출력할 메세지와 위치를 설정
	int x = 18;
	int y = 8;
	GotoXY(x, y); //x와 y의 좌표에 맞춰 게임오버 문구 출력
	printf("===========================");
	GotoXY(x, y + 1);
	printf("======G A M E O V E R======");
	GotoXY(x, y + 2);
	printf("===========================");
	GotoXY(x, y + 5);
	printf("SCORE : %d", score); //점수를 출력

	printf("\n\n\n\n\n\n\n\n\n"); //줄바꿈으로 화면 정리
	system("pause"); //사용자 입력을 받기 전까지 프로그램을 일시 정지
}

class crash
{
public:
	bool isCollision(const int treeX, const int dinoY) // 충돌판정을 하기위한 함수
	{
		//트리의 X가 공룡의 몸체쪽에 있을때,
		//공룡의 높이가 충분하지 않다면 충돌로 처리
		GotoXY(0, 0); //디버깅 용도로 현재 x, y의 좌표를 출력
		printf("treeX : %d, dinoY : %d", treeX, dinoY); // 현재 트리와 공룡의 좌표를 출력
		if (treeX <= 8 && treeX >= 4 && //트리 좌표가 일정 범위 안에 있고
			dinoY > 8) //공룡의 높이가 충분하지 않으면 충돌로 처리
		{
			return true; //dinoY의 값이 8보다 크다면 충돌로 처리
		}
		return false; //dinoY의 값이 8보다 크지 않다면 false로 처리
	}
};


int main() //main 함수 시작
{
	SetConsoleView(); //콘솔창의 크기 지정 함수 부름

	while (true)		//(v2.0) 게임 루프
	{
		//게임 시작시 초기화
		bool isJumping = false; //점프를 하지 않음.
		bool isBottom = true; //바닥이다.
		const int gravity = 3; //중력 = 3

		int dinoY = DINO_BOTTOM_Y; //공룡의 y축 함수를 공룡에 지정
		int treeX = TREE_BOTTOM_X; //나무의 x축 함수를 나무에 지정

		int score = 0; //점수를 0으로 초기화
		clock_t start, curr;	//점수 변수 초기화
		start = clock();		//시작시간 초기화


		crash isCollision;
		bool result = isCollision.isCollision(treeX, dinoY);
		while (true)	//한 판에 대한 루프
		{
			//(v2.0) 충돌체크 트리의 x값과 공룡의 y값으로 판단
			if (isCollision.isCollision(treeX, dinoY)) // '공룡이 나무에 충돌했으면 함수' if문에 넣기
				break; //if문이 맞으면 정지

			//z키가 눌렸고, 바닥이 아닐때 점프
			if (GetKeyDown() == 'z' && isBottom) //만약 z키가 눌리고 바닥이다.
			{
				isJumping = true; //점프한다.
				isBottom = false; //바닥이 아니다.
			}

			//점프중이라면 Y를 감소, 점프가 끝났으면 Y를 증가.
			if (isJumping) //만약 점프중이라면
			{
				dinoY -= gravity; //공룡의 중력 감소
			}
			else //아니라면
			{
				dinoY += gravity; //공룡의 중력 증가
			}

			//Y가 계속해서 증가하는걸 막기위해 바닥을 지정.
			if (dinoY >= DINO_BOTTOM_Y) //만약 공룡이 바닥에 있거나 바닥보다 위에 있을 때
			{
				dinoY = DINO_BOTTOM_Y; //공룡의 처음의 y좌표로 돌아감
				isBottom = true; //바닥이다
			}

			//나무가 왼쪽으로 (x음수) 가도록하고
			//나무의 위치가 왼쪽 끝으로가면 다시 오른쪽 끝으로 소환.
			treeX -= 2; //나무는 -2씩 이동
			if (treeX <= 0) //나무의 위치가 0보다 작거나 같을 때
			{
				treeX = TREE_BOTTOM_X; //나무는 처음의 x좌표로 돌아감
			}

			//점프의 맨위를 찍으면 점프가 끝난 상황.
			if (dinoY <= 3) //공룡의 위치가 3보다 작거나 같다.
			{
				isJumping = false; //점프 못한다.
			}

			DrawDino(dinoY);		//draw dino, 공룡을 그린다(위의 함수로 부터 부른다.)
			DrawTree(treeX);		//draw Tree, 나무를 그린다(위의 함수로 부터 부른다.)

			//(v2.0)
			curr = clock();			//현재시간 받아오기
			if (((curr - start) / CLOCKS_PER_SEC) >= 1)	// 1초가 넘었을 때(1과 같거나 이상)(if문 현재시간 – 시작시간 % 초당 시간)
			{
				score++;	//스코어 UP(현 스코어에 +1)
				start = clock();	//시작시간 초기화 (start 클릭시 시간 초기화)
			}
			Sleep(60); // (지연하고자 하는 시간을 0.060초롤 설정)
			system("cls");	//clear (현재 도스 프롬프트 화면을 지워준다)

			//(v2.0) 점수출력을 1초마다 해주는것이 아니라 항상 출력해주면서, 1초가 지났을때 ++ 해줍니다.
			GotoXY(22, 0);	//커서를 가운데 위쪽으로 옮긴다. 콘솔창이 cols=100이니까 2*x이므로 22정도 넣어줌
			printf("Score : %d ", score);	//점수 출력해줌.
		}

		//(v2.0) 게임 오버 메뉴
		DrawGameOver(score); //(화면에 스코어와 같이 뜬다)
	}
	return 0;
}