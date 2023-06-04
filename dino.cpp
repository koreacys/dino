//[C/C++ game] very simple google dinosaur. (by. BlockDMask)
//소스코드 해석 & 바꿔보기 과제~
/*지금까지 추가된것 & 수정한것
	1. extern
	2. 클래스
	3. 하트를 통한 추가점수
	4. 목숨
	5. 나무 위치 랜덤
	6. if문을 if ~ else로 수정
	7. GetKeyDown을 받던 if문을 switch문을 통해 구현
	8. 새 장애물 추가
	9. x키를 통해 엎드린 공룡 추가
	10. 2단 점프 구현(화면 크기 키움)*/

#include<iostream> //헤더파일 iostream 추가
using namespace std; //cout을 사용할 때, 앞에 std::생략 가능
#include<cstdio> //c언어 헤더파일을 C++에서도 사용 가능하게 만듬
#include<windows.h> //헤더파일 windows.h 추가
#include<conio.h> //헤더파일 conio.h 추가
#include<time.h> //헤더파일 time.h 추가
#include <random>//난수파일 random 추가
#define DINO_BOTTOM_Y 22  //공룡 캐릭터가 땅에서 멈춰있는 위치를 나타냄
#define SDINO_BOTTOM_Y 28  //누워있는 공룡 캐릭터가 땅에서 멈춰있는 위치를 나타냄
#define TREE_BOTTOM_Y 30  //나무가 나타나는 위치의 행(row) 값을 나타냄
#define TREE_BOTTOM_X 55  //나무가 나타나는 위치의 열(column) 값을 나타냄
#define BIRD_BOTTOM_Y 20  //새가 나타나는 위치의 행(row) 값을 나타냄
#define BIRD_BOTTOM_X 55  //새가 나타나는 위치의 열(column) 값을 나타냄
#define HERAT_BOTTOM_Y 18
#define HERAT_BOTTOM_X 40

//콘솔 창의 크기와 제목을 지정하는 함수
void SetConsoleView() //콘솔 창에 대한 초기 설정을 수행
{
	system("mode con:cols=100 lines=35"); //게임 콘솔 창의 각각 가로와 세로를 정한다
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

extern "C" void Dino_body_draw(); //공룡 몸체 그리는 C함수 선언
extern "C" void Dino_Rleg_draw(); //공룡 오른발 그리는 C함수 선언
extern "C" void Dino_Lleg_draw(); //공룡 왼발 그리는 C함수 선언
extern "C" void Dino_slide_draw(); //엎드린 공룡 그리는 C함수 선언
extern "C" void SDino_Rleg_draw(); //엎드린 공룡의 오른발 그리는 C함수 선언
extern "C" void SDino_Lleg_draw(); //엎드린 공룡의 왼발 그리는 C함수 선언

//공룡을 그리는 함수
void DrawDino(int dinoY)
{
	GotoXY(0, dinoY); // 공룡의 x좌표는 0이다
	static bool legFlag = true; // legFlag는 참이다
	Dino_body_draw(); //공룡 몸체 그리는 c함수 부름
	if (legFlag) //legFlag이면
	{
		Dino_Rleg_draw(); //공룡 오른발 그리는 c함수 부름
		legFlag = false; //legFlag가 거짓이므로 
	}
	else //아래의 모형이 출력된다
	{
		Dino_Lleg_draw(); //공룡 왼발 그리는 c함수 부름
		legFlag = true; //legFlag는 참이다
	}
}

void DrowSDino(int dinoZ) //엎드린 공룡을 그리는 함수
{
	GotoXY(0, dinoZ); //엎드린 공룡의 좌표
	Dino_slide_draw(); //엎드린 공룡 그리기
	static bool legFlag = true; // legFlag는 참이다
	if (legFlag) //legFlag이면
	{
		SDino_Rleg_draw(); //엎드린 공룡의 오른발 그리기
		legFlag = false; //legFlag가 거짓이므로 
	}
	else //아래의 모형이 출력된다
	{
		SDino_Lleg_draw(); //엎드린 공룡의 왼발 그리기
		legFlag = true; //legFlag는 참이다
	}
}

//나무를 그리는 함수
void DrawTree(int treeX)
{
	GotoXY(treeX, TREE_BOTTOM_Y); //나무의 좌표(treeX,20)
	cout << "$$$$"; //해당 좌표에 나무일부 출력
	GotoXY(treeX, TREE_BOTTOM_Y + 1); //나무의 좌표(treeX,21)
	cout << " $$ "; //해당 좌표에 나무일부 출력
	GotoXY(treeX, TREE_BOTTOM_Y + 2); //나무의 좌표(treeX,22)
	cout << " $$ "; //해당 좌표에 나무일부 출력
	GotoXY(treeX, TREE_BOTTOM_Y + 3); //나무의 좌표(treeX,23)
	cout << " $$ "; //해당 좌표에 나무일부 출력
	GotoXY(treeX, TREE_BOTTOM_Y + 4); //나무의 좌표(treeX,24)
	cout << " $$ "; //해당 좌표에 나무일부 출력
}

//새를 그리는 함수
void DrawBird(int birdX)
{
	GotoXY(birdX, BIRD_BOTTOM_Y); //새의 좌표(birdX,20)
	cout << "              $$$$"; //해당 좌표에 새의 일부 출력
	GotoXY(birdX, BIRD_BOTTOM_Y + 1); //새의 좌표(birdX,21)
	cout << "    $$$$$$  $$$"; //해당 좌표에 새의 일부 출력
	GotoXY(birdX, BIRD_BOTTOM_Y + 2); //새의 좌표(birdX,22)
	cout << "$$$$$$  $$$$$$$$$$$$$"; //해당 좌표에 새의 일부 출력
	GotoXY(birdX, BIRD_BOTTOM_Y + 3); //새의 좌표(birdX,23)
	cout << "    $$$$$$      $$$"; //해당 좌표에 새의 일부 출력
}

//하트 그리는 함수
void DrawHeart(int heartX)
{
	GotoXY(heartX, HERAT_BOTTOM_Y);
	cout<<"$$$ $$$";
	GotoXY(heartX, HERAT_BOTTOM_Y + 1);
	cout<<" $$$$$";
	GotoXY(heartX, HERAT_BOTTOM_Y + 2);
	cout<<"   $ ";
}


//(v2.0) 충돌 했을때 게임오버 그려줌
void DrawGameOver(const int score) //게임 오버 화면을 나타내는 함수
{
	system("cls"); // 콘솔 화면을 지움
	//출력할 메세지와 위치를 설정
	int x = 18;
	int y = 13;
	GotoXY(x, y); //x와 y의 좌표에 맞춰 게임오버 문구 출력
	cout << "===========================";
	GotoXY(x, y + 1);
	cout << "======G A M E O V E R======";
	GotoXY(x, y + 2);
	cout << "===========================";
	GotoXY(x, y + 5);
	cout << "SCORE : " << score; //점수를 출력

	cout << "\n\n\n\n\n\n\n\n\n"; //줄바꿈으로 화면 정리
	system("pause"); //사용자 입력을 받기 전까지 프로그램을 일시 정지
}

class crash //class crash 선언
{
public: //public으로 어디서든 접근 가능하게 선언
	bool isCollision(const int treeX, const int dinoY, const int life)//함수에 목숨을 추가
	{
		GotoXY(0, 0);
		printf("treeX : %d, dinoY : %d, life : %d", treeX, dinoY, life);//우측 상단에 목숨의 수를 추가로 출력
		if (treeX <= 8 && treeX >= 4 && dinoY > 18) {

			return true;
		}

		else // if else 구문으로 수정함
			dinoY < 8;
		{
			return false;
		}
	}

	bool isCollision2(const int birdX, const int dinoY) // 충돌판정을 하기위한 함수
	{
		//새의 X가 공룡의 몸체쪽에 있을때,
		//공룡의 높이가 새와 같거나 높으면 충돌 처리
		GotoXY(0, 1); //디버깅 용도로 현재 x, y의 좌표를 출력
		printf("birdX : %d, dinoY : %d", birdX, dinoY); // 현재 새와 공룡의 좌표를 출력
		if (birdX <= 5 && birdX >= 2 && //새의 좌표가 일정 범위 안에 있고
			dinoY > 2) //공룡의 높이가 충분하지 않으면 충돌로 처리
		{
			return true; //dinoY의 값이 2보다 크다면 충돌로 처리
		}
		return false; //dinoY의 값이 2보다 크지 않다면 false로 처리
	}

	//하트와 공룡이 충돌했을때
	bool isHeartCollision(const int heartX, const int dinoY)
	{

		GotoXY(0, 0);
		printf("heartX : %d, dinoY : %d", heartX, dinoY); //이런식으로 적절한 X, Y를 찾습니다.
		if (heartX <= 6 && heartX >= 4 &&
			dinoY > 8)
		{
			return true;
		}
		return false;
	}
};


int main() //main 함수 시작
{
	SetConsoleView(); //콘솔창의 크기 지정 함수 부름
	srand(time(NULL)); // 난수 발생기 초기화

	while (true)		//(v2.0) 게임 루프
	{
		//게임 시작시 초기화
		bool isJumping = false; //점프를 하지 않음.
		bool isDoubleJumping = false; //더블 점프를 하지 않음.
		bool isBottom = true; //바닥이다.
		bool isSliding = false; //엎드리지 않음.
		const int gravity = 3; //중력 = 3
		bool isCollisionDetected = false;  // 충돌 감지 여부

		int dinoY = DINO_BOTTOM_Y; //공룡의 y축 함수를 공룡에 지정
		int dinoZ = SDINO_BOTTOM_Y; //공룡이 엎드릴 때의 y축 함수를 공룡에 지정
		int treeX = TREE_BOTTOM_X; //나무의 x축 함수를 나무에 지정
		int birdX = BIRD_BOTTOM_X; //새의 x축 함수를 새에게 지정
		int heartX = HERAT_BOTTOM_X; //하트의 x축 함수를 하트에 지정
		bool showTree = true; // 나무를 보여줄지 여부
		

		int score = 0; //점수를 0으로 초기화
		int life = 3;   //목숨 갯수 지정
		clock_t start, curr;	//점수 변수 초기화
		start = clock();		//시작시간 초기화


		crash isCollision; //crash 클래스를 isCollision으로 선언
		int jumpCount = 0; // 연속으로 점프한 횟수를 저장하는 변수
		clock_t jumpTime = clock(); // 점프가 시작된 시간
		while (true)	//한 판에 대한 루프
		{
			if (isCollision.isCollision(treeX, dinoY, life))
			{
				if (!isCollisionDetected)  // 충돌이 한 번도 감지되지 않았을 때
				{
					life--;  // 생명 1 감소
					isCollisionDetected = true;  // 충돌 감지 플래그 설정
				}

				if (life == 0)
				{
					DrawGameOver(score);  // 게임 오버 화면 출력
					break;  // 충돌이 발생하면 게임 루프 종료
				}
			}
			else
			{
				isCollisionDetected = false;  // 충돌이 감지되지 않았음을 플래그로 표시
			}

			//하트와 충돌했을때 10점 추가
			if (isCollision.isHeartCollision(heartX, dinoY))
			{
				score += 10;
				isCollisionDetected = true;  // 충돌 감지 플래그 설정
			}
			else
			{
				isCollisionDetected = false;  // 충돌이 감지되지 않았음을 플래그로 표시
			}

			int key = GetKeyDown(); //키보드 입력을 key로 받는다.
			switch (key) //z키가 눌리거나 점프중일때의 스위치 케이스문
			{
			case 'x': isSliding = !isSliding; break; //엎드린다
			case 'z': clock_t currentTime = clock();
				if (currentTime - jumpTime <= 300 && jumpCount < 2) {
					// 0.8초 이내에 연속으로 누르고 점프 횟수가 2번 미만인 경우 더블 점프
					isDoubleJumping = true;
					jumpCount++;
				}
				else if (!isDoubleJumping && jumpCount < 2) {
					isJumping = true;
					isBottom = false;
					jumpCount++;
					jumpTime = currentTime;
				}
				break;
			}

			if (isSliding == false) {
				if (isCollision.isCollision2(birdX, dinoY)) // '공룡이 새에 충돌했으면 함수' if문에 넣기
					break; //if문이 맞으면 정지
			}

			//점프중이라면 Y를 감소, 점프가 끝났으면 Y를 증가.
			if (isJumping || isDoubleJumping) //만약 점프 중이거나 더블 점프 중이라면
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
				jumpCount = 0;
			}

			// 난수 생성기
			std::random_device rd; //하드웨어 장치에서 시드값을 얻기 위한 random_device 생성
			std::mt19937 gen(rd()); //메르센 트위스터 알고리즘을 사용하는 유사 난수 생성기를 선언

			// 균등 분포
			std::uniform_int_distribution<> dis(0, 9); // 0부터 9까지의 정수를 균등하게 생성하는 분포 객체를 선언

			// 랜덤한 정수 생성
			int random = dis(gen); // 위에서 선언한 분포 객체에 유사 난수 생성기를 인자로 넘겨서 랜덤한 정수를 생성하고, 그 값을 변수에 저장

			if (showTree) //나무를 보여줄 경우
			{
				treeX -= 2; //나무는 -2씩 이동
				if (treeX <= 0) //나무의 위치가 0보다 작거나 같을 때
				{
					if (treeX <= 0)
					{
						treeX = rand() % (TREE_BOTTOM_X - 20) + (TREE_BOTTOM_X - 30); // 랜덤한 treeX 값 설정
					}

					// 추가된 부분: 랜덤한 정수 생성 및 showTree 값 변경
					random = dis(gen); //랜덤한 정수 생성
					if (random % 2 == 0) //랜덤한 정수가 짝수이면
					{
						showTree = true; //showTree 값을 true로 유지
					}
					else //랜덤한 정수가 홀수이면
					{
						showTree = false; //showTree 값을 false로 바꿈
					}
				}
			}

			else //새를 보여줄 경우
			{
				birdX -= 2; //새는 -2씩 이동
				if (birdX <= 0) //새의 위치가 0보다 작거나 같을 때
				{
					birdX = BIRD_BOTTOM_X; //새는 처음의 x좌표로 돌아감

					// 추가된 부분: 랜덤한 정수 생성 및 showTree 값 변경
					random = dis(gen); //랜덤한 정수 생성
					if (random % 2 == 0) //랜덤한 정수가 짝수이면
					{
						showTree = true; //showTree 값을 true로 바꿈
					}
					else //랜덤한 정수가 홀수이면
					{
						showTree = false; //showTree 값을 false로 유지
					}
				}
			}

			//하트가 왼쪽으로 (x음수) 가도록하고
		   //하트의 위치가 왼쪽 끝으로가면 다시 오른쪽 끝으로 소환
			heartX -= 2;
			if (heartX <= 0)
			{
				heartX = HERAT_BOTTOM_X;
			}

			//점프의 맨위를 찍으면 점프가 끝난 상황.
			if (dinoY <= 10) //공룡의 위치가 -6보다 작거나 같다.
			{
				if (isJumping) //점프 중이라면
				{
					isJumping = false; //점프를 멈춘다.
				}
				else if (isDoubleJumping) //더블 점프 중이라면
				{
					isDoubleJumping = false; //더블 점프를 멈춘다.
				}
			}

			if (isJumping || isDoubleJumping)
			{
				if (dinoY <= 15)
				{
					isJumping = false; //점프를 멈춘다
				}
				else if (dinoY <= 8)
				{
					isDoubleJumping = false; //더블 점프를 멈춘다.
				}

			}

			if (isSliding) { //엎드리는 상태라면
				DrowSDino(dinoZ); //엎드린 공룡을 그린다
			}
			else { //아니라면
				DrawDino(dinoY); //draw dino, 공룡을 그린다(위의 함수로 부터 부른다.)
			}
			if (showTree) { //만약 showTree라면
				DrawTree(treeX); //draw Tree, 나무를 그린다(위의 함수로 부터 부른다.)
			}
			else { //아니라면
				DrawBird(birdX); //draw Bird, 새를 그린다(위의 함수로 부터 부른다.)
			}
			DrawHeart(heartX);      //draw heart

			//(v2.0)
			curr = clock();			//현재시간 받아오기
			if (((curr - start) / CLOCKS_PER_SEC) >= 1)	// 1초가 넘었을 때(1과 같거나 이상)(if문 현재시간 – 시작시간 % 초당 시간)
			{
				score++;	//스코어 UP(현 스코어에 +1)
				start = clock();	//시작시간 초기화 (start 클릭시 시간 초기화)
			}
			//메세지(문자)를 다시 출력해줌
			GotoXY(0, 0);
			cout << "treeX : " << treeX << ", dinoY : " << dinoY<<", life : "<< life;
			GotoXY(0, 1);
			cout << "birdX : " << birdX << ", dinoY : " << dinoY;
			GotoXY(22, 0);
			cout << "Score : " << score;
			Sleep(60); // (지연하고자 하는 시간을 0.060초롤 설정)
			system("cls");	//clear (현재 도스 프롬프트 화면을 지워준다)
		}

		//(v2.0) 게임 오버 메뉴
		DrawGameOver(score); //(화면에 스코어와 같이 뜬다)
	}
	return 0;
}