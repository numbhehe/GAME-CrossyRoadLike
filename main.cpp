#include <iostream>
#include <conio.h>
#include <dos.h>
#include <windows.h>
#include <time.h>
#include <fstream>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 90
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;


char car1[3][13] = {' ',' ',' ',' ','_','_','_','_',' ',' ',' ',' ',' ',
                   ' ','_','_','/','_','_','_','_','\\','_','_','_',' ',
                   '|','-','-','O','-','-','-','-','O','-','-','-','|'};

char rabbit[3][6] = {'(','\\','_','_','/',')',
                     '(','=','\'','\'','=',')',
                     '(','"',')','(','"',')'};

int enemyStart[3] = {7,12,17};
int enemyX[3] = {7,12,17};
int enemySpeed[3] = {3, 1, 2};

int Carrot[2] = {0,0};

int rabbitPosX = WIN_WIDTH/2;
int rabbitPosY = 26;

int upperBorder = 1;
int lowerBorder = 29;

int score = 0*100;

int i;
string user;

bool isCollision = true;

void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
	if(size == 0)
        size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}

void drawBorder(){
  for (i = 2; i <= 77; i++) {
    gotoxy(i, 0);
    cout << '-';
    gotoxy(i, 29);
    cout << '-';
  }
  for (i = 0; i <= 29; i++) {
    gotoxy(2, i);
    cout << '|';
    gotoxy(77, i);
    cout << '|';
  }
}

void drawStreet(){
    gotoxy(3, 6); cout<<"==========================================================================";
    gotoxy(3, 11);cout<<"--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --";
    gotoxy(3, 16);cout<<"--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --";
    gotoxy(3, 21);cout<<"==========================================================================";

}

void drawRabbit(){
    for(int i=0; i<3; i++){
        for(int j=0; j<6; j++){
            gotoxy(j+rabbitPosX,i+rabbitPosY); cout << rabbit[i][j];
        }
    }
}

void eraseRabbit(){
    for(int i=0; i<3; i++){
        for(int j=0; j<6; j++){
            gotoxy(j+rabbitPosX,i+rabbitPosY); cout << " ";
        }
    }
}

void drawEnemy(int num){
    for(int i=0; i<3; i++){
        for(int j=0; j<13; j++){
            gotoxy(j+enemyX[num],i+enemyStart[num]); cout << car1[i][j];
        }
    }
}

void eraseEnemy(int num){
    for(int i=0; i<3; i++){
        for(int j=0; j<13; j++){
            gotoxy(j+enemyX[num],i+enemyStart[num]); cout << " ";
        }
    }
}

void moveEnemies(){
    for (int i = 0; i < 3; i++) {
        eraseEnemy(i);
        enemyX[i] += enemySpeed[i];
        if (enemyX[i] > 64)
            enemyX[i] = 3;
        }
    }

void drawCarrot(){
     if (Carrot[0] == 0 && Carrot[1] == 0) {
        Carrot[0] = rand() % 51 + 10;
        Carrot[1] = rand() % 7 + 9;
    }

     for(int i=0; i<3; i++){
        gotoxy(Carrot[0], Carrot[1]); cout << "x=>";
     }
}

void eraseCarrot(){
     for(int i=0; i<3; i++){
        gotoxy(Carrot[0], Carrot[1]); cout << "   ";
     }
}

void ScoreUpdate(){
    gotoxy(WIN_WIDTH + 27, 9); cout<<score;
}

void collectCarrot() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            if (rabbitPosY + i == Carrot[1] && (rabbitPosX + j == Carrot[0] || rabbitPosX + j == Carrot[0] + 1)) {
                eraseCarrot();
                score++;
                Carrot[0] = 0;
                Carrot[1] = 0;
                drawCarrot();
                ScoreUpdate();
                return;
            }
        }
    }
}

void GameOver(){

    isCollision = false;

    rabbitPosX = WIN_WIDTH / 2;
    rabbitPosY = 26;
    Carrot[0] = 0;
    Carrot[1] = 0;
    enemyX[0] = 7;
    enemyX[1] = 12;
    enemyX[2] = 17;

    system("cls");

    gotoxy(45, 9);  cout<< "================";
    gotoxy(45, 10); cout<< "|   GAMEOVER   |";
    gotoxy(45, 11); cout<< "================";
    gotoxy(45, 12); cout<< "Insert user name" << endl;
    gotoxy(45, 13); cin >> user;
    gotoxy(37, 14); cout<<"Press any key to go back to menu";

    ofstream save("leaderboard.txt",ios::app);
    save << user << ": " << score << endl;

    score = 0;

    getch();


}


void collision(){
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 13; k++) {
                if (rabbitPosY + j == enemyStart[i] && (rabbitPosX + k >= enemyX[i] && rabbitPosX + k < enemyX[i] + 13)) {
                     GameOver();
                     isCollision = true;
                     return ;
                }
            }
        }

    }
    isCollision = false;
}

void instruction(){
    system("cls");

    gotoxy(40,5);   cout<<" Move the rabbit to collect carrots.";
    gotoxy(40,6);   cout<<" Carrots are points. ";
    gotoxy(40,7);   cout<<" Beware of traffic on the road.";
    gotoxy(40,8);   cout<<" Try to collect as many carrots as u can.";
    gotoxy(40,9);   cout<<" Check leaderboard to see your score history";
    gotoxy(40,10);  cout<<" Gooooood luck and have fun !!!!!";
    gotoxy(40,12);  cout<<" Press any key to continue ";

    getche();

}

void leaderboard(){
    system("cls");

    ifstream read("leaderboard.txt");
    gotoxy(45,5);   cout << "Leaderboard" << endl;
    string linia;
    while(getline(read,linia)){
    cout<< linia << endl;
    }
    getche();

}

void play(){

    isCollision = false;

    system("cls");
	drawBorder();

	gotoxy(WIN_WIDTH + 20, 7);  cout<<"|    SCORE     |";
	gotoxy(WIN_WIDTH + 20, 8);  cout<<"|--------------|";
	gotoxy(WIN_WIDTH + 20, 9);  cout<<"|";
	gotoxy(WIN_WIDTH + 35, 9);  cout<<"|";
	gotoxy(WIN_WIDTH + 20, 10); cout<<"|--------------|";
	gotoxy(WIN_WIDTH + 20, 16); cout<<"|    Control   |";
	gotoxy(WIN_WIDTH + 20, 17); cout<<"|--------------|";
	gotoxy(WIN_WIDTH + 20, 18); cout<<"|  W - Upwards |";
	gotoxy(WIN_WIDTH + 20, 19); cout<<"|   S - Down   |";
	gotoxy(WIN_WIDTH + 20, 20); cout<<"|   A - Left   |";
	gotoxy(WIN_WIDTH + 20, 21); cout<<"|   D - Right  |";
	gotoxy(WIN_WIDTH + 20, 22); cout<<"|--------------|";

	gotoxy(30, 10);cout<<"Press any key to start";
    getch();
	gotoxy(30, 10);cout<<"                      ";



    while(1){
        if(kbhit()){
            char key = getch();
            if(key == 'a' || key == 'A'){
                if( rabbitPosX > 3)
                    rabbitPosX -=4;
            }
            if(key == 'd' || key == 'D'){
                if( rabbitPosX < 70)
                    rabbitPosX +=4;
            }
            if(key == 'w' || key == 'W'){
                if( rabbitPosY > 1)
                    rabbitPosY -=3;
                    if (rabbitPosY < 1)
                        rabbitPosY = 26;
            }
            if(key == 's' || key == 'S'){
                if( rabbitPosY > 1)
                    rabbitPosY +=3;
                    if (rabbitPosY > 26)
                        rabbitPosY = 2;
            }
            if(key == 27){
                break;
            }
        }

        drawStreet();
        drawCarrot();
        drawEnemy(0);
        drawEnemy(1);
        drawEnemy(2);
        drawRabbit();
        collectCarrot();
        collision();
        if(isCollision){
            break;
        }
        Sleep(50);
        moveEnemies();
        eraseRabbit();

    }



}

int main() {

    setcursor(0,0);
	srand( (unsigned)time(NULL));

	do{
		system("cls");
		gotoxy(45,5);   cout<<" --------------------------- ";
		gotoxy(45,6);   cout<<" |       Crossy Road       | ";
		gotoxy(45,7);   cout<<" --------------------------- ";
		gotoxy(45,8);   cout<<" |     1. Start Game       | ";
		gotoxy(45,9);   cout<<" |     2. Instruction      | ";
		gotoxy(45,10);  cout<<" |     3. Leaderboard      | ";
		gotoxy(45,11);  cout<<" |     4.    Quit          | ";
		gotoxy(45,12);  cout<<" --------------------------- ";
		gotoxy(45,14);  cout<<"Select option: ";
		char option = getche();

		if( option =='1') play();
		else if( option =='2') instruction();
		else if( option =='3') leaderboard();
		else if( option =='4') exit(0);

	}while(1);

    return 0;
}
