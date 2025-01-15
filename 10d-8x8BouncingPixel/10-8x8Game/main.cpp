#include <stdio.h>
#include <stdlib.h>


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "canvas.h"

bool aPressed = false;
bool sPressed = false;

bool wPressed = false;
bool dPressed = false;
bool entPressed = false;

#define WINDOW_X	100
#define WINDOW_Y	100

#define WINDOW_WIDTH	512
#define WINDOW_HEIGHT	512

#define RESOLUTION_X	8
#define RESOLUTION_Y	8

#define LEFT 1
#define RIGHT 2
#define NONE 0

bool entryA = false;
int speedBall = 100;

int player = 1;
int count = 0;
int num = 0;
int xPad = 0;
int yPad = 0;
int score[5] = { 0,0,0,0,0 };
int gameover = 0;
unsigned char pixels[RESOLUTION_X][RESOLUTION_Y];
int pad[3][3]; 
int winner = 0;
int post;
int scoreA = 0;
int scoreB = 0;
int game = 1;
struct match {
	int winner = 0;
	int game = 1;
	unsigned char pixels[RESOLUTION_X][RESOLUTION_Y];
	unsigned int pad[3][3];

}; 
void initPixels()
{
	for (int i = 0; i < RESOLUTION_X; i++)
	{
		for (int j = 0; j < RESOLUTION_Y; j++)
		{ 		
			pixels[i][j] = 0;
								
		}
	}

}

//grid of 3*3 to store player selection
void initPad()
{
	int pad[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pad[i][j] = 0;

		}
	}
}

void updatePixels()
{
	for (int i = 0; i < RESOLUTION_X; i++)
	{
		for (int j = 0; j < RESOLUTION_Y; j++)
		{
			if (pixels[i][j] == 1.0)
				canvasSetPixel(i, j, 1.0);
			else
				canvasSetPixel(i, j, 0.25);
		}
	}
}
//check random available xpad /yPad position available
int checkPad() {
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (pixels[3 * i][3 * j] == 0 && pixels[(3 * i)+1][(3 * j)+1] == 0) {
				//arr[(8 - (3 * i) - j)] = 0;
				xPad = (3 * i);
				yPad = 3*j;
			}
		}
	}
return xPad, yPad;
}

int drawPad(int x) {
	if (entPressed==false) {
		pixels[xPad + 1][yPad] = 0;
		pixels[xPad][yPad] = 0;
		pixels[xPad + 1][yPad + 1] = 0;
		pixels[xPad][yPad + 1] = 0;
	}
	if (dPressed && xPad < RESOLUTION_X - 2)
		if (dPressed && (pixels[xPad+3][yPad])==0)
			xPad = xPad + 3;
		else if (dPressed && xPad == 0 && pixels[xPad + 3][yPad] == 1 && pixels[xPad + 6][yPad] == 0)
			xPad = xPad + 6;
		else if (dPressed && yPad != 6 && xPad == 0 && pixels[xPad + 3][yPad] == 1 && pixels[xPad + 6][yPad] == 1 && pixels[xPad][yPad+3] == 1 && pixels[xPad + 6][yPad + 3] == 0) {
			yPad = yPad + 3;
			xPad = xPad + 6;
		}
		else if (dPressed && yPad != 0 && xPad == 0 && pixels[xPad + 3][yPad] == 1 && pixels[xPad + 6][yPad] == 1 && pixels[xPad][yPad-3] == 1 && pixels[xPad + 6][yPad - 3] == 0) {
			yPad = yPad - 3;
			xPad = xPad + 6;
		}
		else if (dPressed && yPad != 0 && xPad != 6 && pixels[xPad + 3][yPad] == 1 && pixels[xPad + 3][yPad - 3] == 0) {
			yPad = yPad - 3;
			xPad = xPad + 3;
		}
		else if (dPressed && yPad != 6 && xPad != 6 && pixels[xPad + 3][yPad] == 1 && pixels[xPad + 3][yPad + 3] == 0) {
			yPad = yPad + 3;
			xPad = xPad + 3;
		}
	if (aPressed && xPad > 0 && xPad < RESOLUTION_X - 1)
		if (aPressed && pixels[xPad - 3][yPad] == 0) {
			xPad = xPad - 3;
		}
		else if (aPressed && xPad == 6 && pixels[xPad - 3][yPad] == 1 && pixels[xPad - 6][yPad] == 0){
			xPad = xPad -6;
		}
		else if (aPressed && yPad != 6 && xPad == 6 && pixels[xPad-3][yPad] == 1 && pixels[xPad-6][yPad] == 1 && pixels[xPad][yPad + 3] == 1 && pixels[xPad - 6][yPad + 3] == 0) {
			yPad = yPad + 3;
			xPad = xPad - 6;
		}
		else if (aPressed && yPad != 0 && xPad == 6 && pixels[xPad - 3][yPad] == 1 && pixels[xPad - 6][yPad] == 1 && pixels[xPad][yPad - 3] == 1 && pixels[xPad - 6][yPad - 3] == 0) {
			yPad = yPad - 3;
			xPad = xPad - 6;
		}
		else if (aPressed && yPad != 0 && xPad != 0 && pixels[xPad-3][yPad] == 1 && pixels[xPad - 3][yPad - 3] == 0) {
			yPad = yPad - 3;
			xPad = xPad - 3;
		}
		else if (aPressed && yPad != 6 && xPad != 0 && pixels[xPad - 3][yPad] == 1 && pixels[xPad - 3][yPad + 3] == 0) {
			yPad = yPad + 3;
			xPad = xPad - 3;
		}
	if (wPressed && yPad < RESOLUTION_Y - 2) 
		//yPad = yPad + 3;
		if (wPressed && pixels[xPad][yPad + 3] == 0 )
			yPad = yPad + 3;
	    else if (wPressed && yPad != 6 && xPad != 6 && pixels[xPad][yPad + 3] == 1 && pixels[xPad + 3][yPad + 3] == 0){
			yPad = yPad + 3;
			xPad = xPad + 3;		
		}
		else if (wPressed && yPad != 6 && xPad != 0 && pixels[xPad][yPad + 3] == 1 && pixels[xPad - 3][yPad + 3] == 0) {
			yPad = yPad + 3;
			xPad = xPad - 3;
		}
		else if (wPressed && yPad ==0  && xPad != 0 && pixels[xPad][yPad + 3] == 1 && pixels[xPad][yPad + 6] == 1 && pixels[xPad - 3][yPad + 3] == 1 &&  pixels[xPad - 3][yPad + 6] == 0) {
			yPad = yPad + 6;
			xPad = xPad - 3;
		}
		else if (wPressed && yPad == 0 && xPad != 6 && pixels[xPad][yPad + 3] == 1 && pixels[xPad][yPad + 6] == 1 && pixels[xPad + 3][yPad + 3] == 1 && pixels[xPad + 3][yPad + 6] == 0) {
			yPad = yPad + 6;
			xPad = xPad + 3;
		}
		else if (wPressed && yPad == 0 && pixels[xPad][yPad + 3] == 1 && pixels[xPad][yPad + 6] == 0)
			yPad = yPad + 6;

	if (sPressed && yPad > 0 && yPad < RESOLUTION_Y - 1)
		//yPad = yPad - 3;
		if (sPressed && pixels[xPad][yPad-3] == 0)
			yPad = yPad - 3;
		
		else if (sPressed && yPad != 0 && xPad != 0 && pixels[xPad][yPad - 3] == 1 && pixels[xPad - 3][yPad - 3] == 0) {
			yPad = yPad - 3;
			xPad = xPad - 3;
		}
		else if (sPressed && yPad != 0 && xPad != 6 && pixels[xPad][yPad - 3] == 1 && pixels[xPad + 3][yPad - 3] == 0 ) {
			yPad = yPad - 3;
			xPad = xPad + 3;
		}
		else if (sPressed && yPad == 6 && pixels[xPad][yPad - 3] == 1 && pixels[xPad][yPad - 6] == 0)
			yPad = yPad - 6;
		else if (sPressed && yPad == 6  && xPad != 0 && pixels[xPad][yPad-3] == 1 && pixels[xPad][yPad-6] == 1 && pixels[xPad-3][yPad-3] == 1 && pixels[xPad - 3][yPad - 6] == 0) {
			yPad = yPad - 6;
			xPad = xPad - 3;
		}
		else if (sPressed && yPad == 6 && xPad != 6 && pixels[xPad][yPad - 3] == 1 && pixels[xPad][yPad - 6] == 1 && pixels[xPad + 3][yPad - 3] == 1 && pixels[xPad + 3][yPad - 6] == 0) {
			yPad = yPad - 6;
			xPad = xPad + 3;
		}

	if (x == 1) {
		pixels[xPad + 1][yPad] = 1;
		pixels[xPad][yPad] = 1;
		pixels[xPad + 1][yPad + 1] = 1;
		pixels[xPad][yPad + 1] = 1;
	}

	if (x == 2) {
		pixels[xPad + 1][yPad] = 0;
		pixels[xPad][yPad] = 1;
		pixels[xPad + 1][yPad + 1] = 1;
		pixels[xPad][yPad + 1] = 0;
	}
	return  yPad,xPad;
}

void blink(int x) {
	if (entPressed == false) {
		pixels[xPad + 1][yPad] = 0;
		pixels[xPad][yPad] = 0;
		pixels[xPad + 1][yPad + 1] = 0;
		pixels[xPad][yPad + 1] = 0;
		updatePixels();
		repaint();
		Sleep(250);
		if (x == 1) {
			pixels[xPad + 1][yPad] = 1;
			pixels[xPad][yPad] = 1;
			pixels[xPad + 1][yPad + 1] = 1;
			pixels[xPad][yPad + 1] = 1;
		}
		if (x == 2) {
			pixels[xPad + 1][yPad] = 0;
			pixels[xPad][yPad] = 1;
			pixels[xPad + 1][yPad + 1] = 1;
			pixels[xPad][yPad + 1] = 0;
		}
		updatePixels();
		repaint();
	}
}

//store and print player selections
void PrintPlayerGrid(int x) {
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (pad[i][j] == x)
				printf("%d %d\n", 3*i, 3*j);
		}
	}
}


void cleanup()
{
	canvasClose();
}

void showPost(int x,int y) {
	initPixels();
	if ( y==1 && gameover == 1) {
		
		if (x <= 3) {
			for (int i = 0; i < 3; i++)

			{
				for (int j = 0; j < 3; j++)
				{
					if (j == (x)-1) {
						pixels[3 * i][3 * j] = 1;
						pixels[(3 * i) + 1][(3 * j) + 1] = 1;
						pixels[(3 * i) + 1][3 * j] = 1;
						pixels[(3 * i)][(3 * j) + 1] = 1;
					}
				}
			}
		}
		else if (x >= 3 && x<=6) {
			for (int i = 0; i < 3; i++)

			{
				for (int j = 0; j < 3; j++)
				{ 
					if (i == (x - 4)) {
						pixels[3 * i][3 * j] = 1;
						pixels[(3 * i) + 1][(3 * j) + 1] = 1;
						pixels[(3 * i) + 1][(3) * j] = 1;
						pixels[(3 * i)][((3) * j) + 1] = 1;
					}
				}
			}
		}
		else if (x ==7) {
			for (int i = 0; i < 3; i++)

			{
				for (int j = 0; j < 3; j++)
				{
					if (i == j) {
						pixels[3 * i][3 * j] = 1;
						pixels[(3 * i) + 1][(3 * j) + 1] = 1;
						pixels[(3 * i) + 1][3 * j] = 1;
						pixels[(3 * i)][(3 * j) + 1] = 1;
					}
					
				}
			}
		}
		else if (x == 8) {
			for (int i = 0; i < 3; i++)

			{
				for (int j = 0; j < 3; j++)
				{
					if (i == 2 - j) {
						pixels[3 * i][3 * j] = 1;
						pixels[(3 * i) + 1][(3 * j) + 1] = 1;
						pixels[(3 * i) + 1][3 * j] = 1;
						pixels[(3 * i)][(3 * j) + 1] = 1;
					}
				}
			}
		}
	}
	else if ( y == 2 && gameover == 1) {
		if (x <= 3) {
			for (int i = 0; i < 3; i++)

			{
				for (int j = 0; j < 3; j++)
				{
					if (j ==(x-1)) {

						pixels[3 * i][3 * j] = 1;
						pixels[( 3 * i) + 1][(3 * j) + 1] = 1;
						pixels[(3  * i) + 1][3 * j] = 0;
						pixels[(3 * i)][(3 * j) + 1] = 0;

					}
				}
			}
		}
		else if (x >= 3 && x <= 6) {
			for (int i = 0; i < 3; i++)

			{
				for (int j = 0; j < 3; j++)
				{
					if (i == (x - 4)) {
						pixels[3 * i][(3* j)] = 1;
						pixels[(3 * i) + 1][((3) * j) + 1] = 1;
						pixels[(3 * i) + 1][(3) * j] = 0;
						pixels[(3 * i)][((3) * j) + 1] = 0;
					}
				}
			}
		}
		else if (x == 7) {
			for (int i = 0; i < 3; i++)

			{
				for (int j = 0; j < 3; j++)
				{
					if (i == j) {
						pixels[3 * i][3 * j] = 1;
						pixels[(3 * i) + 1][(3 * j) + 1] = 1;
						pixels[(3 * i) + 1][3 * j] = 0;
						pixels[(3 * i)][(3 * j) + 1] = 0;
					}

				}
			}
		}
		else if (x == 8) {
			for (int i = 0; i < 3; i++)

			{
				for (int j = 0; j < 3; j++)
				{
					if (j == 2 - i) {
						pixels[3 * i][3 * j] = 1;
						pixels[(3 * i) + 1][(3 * j) + 1] = 1;
						pixels[(3 * i) + 1][3 * j] = 0;
						pixels[(3 * i)][(3 * j) + 1] = 0;
					}
				}
			}
		}
		
	}
	else if (x == 0) {
		Sleep(0);
	}
	updatePixels();
	repaint();
	Sleep(500);
}

void showWinner(int x) {
	initPixels();
	if (x == 1 && gameover==1) {
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				pixels[3 * i][3 * j] = 1;
				pixels[(3 * i) + 1][(3 * j) + 1] = 1;
				pixels[(3 * i) + 1][3 * j] = 1;
				pixels[(3 * i)][(3 * j) + 1] = 1;
			}			
		}		
	}
	else if (x == 2 && gameover == 1) {
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				pixels[3 * i][3 * j] = 1;
				pixels[(3 * i) + 1][(3 * j) + 1] = 1;
				pixels[(3 * i) + 1][3 * j] = 0;
				pixels[(3 * i)][(3 * j) + 1] = 0;
			}			
		}		
	}
	else if (x == 0) {
		Sleep(0);
	}	
	updatePixels();
	repaint();
	Sleep(1000);
	initPad();
	for (int i = 0; i < RESOLUTION_X; i++)
	{
		for (int j = 0; j < RESOLUTION_Y; j++)
		{
			canvasSetPixel(i, j, 0);

		}
	}

	
}

void showScore() {
	initPixels();
	pixels[0][7] = 1;
	pixels[0][6] = 1;
	pixels[0][5] = 1;
	pixels[2][7] = 1;
	pixels[2][6] = 1;
	pixels[2][5] = 1;
	pixels[1][7] = 1;
	pixels[1][5] = 1;


	pixels[7][7] = 1;
	pixels[5][5] = 1;
	pixels[7][5] = 1;
	pixels[5][7] = 1;
	pixels[6][6] = 1;

	score[game - 1] = winner;
	if (game<=5){
		for (int i = 0; i <= (game - 1); i++) {
			if (score[i] == 1) {
				pixels[3][4 - i] = 1;
			}
			else if (score[i] == 2) {
				pixels[4][4 - i] = 1;
			}
			else if (score[i] == 0) {
				pixels[3][4 - i] = 1;
				pixels[4][4 - i] = 1;
			}

			printf("%d", score[i]);
		}
	}
	else if (game > 5) {
		for (int i = 0; i <= (5 - 1); i++) {
			if (score[i] == 1) {
				pixels[3][4 - i] = 1;
			}
			else if (score[i] == 2) {
				pixels[4][4 - i] = 1;
			}
			else if (score[i] == 0) {
				pixels[3][4 - i] = 1;
				pixels[4][4 - i] = 1;
			}
		}
	}
	
	updatePixels();
	repaint();
	Sleep(1000);

	if (gameover == 1){
		count = 0;
		winner = 0;
		xPad = 0;
		yPad = 0;
		num = 0;
		entryA = false;
		initPixels();
		initPad();
		updatePixels();
		for (int i = 0; i < RESOLUTION_X; i++)
		{
			for (int j = 0; j < RESOLUTION_Y; j++)
			{
				canvasSetPixel(i, j, 0);

			}
		}
		repaint();
		gameover = 0;
		Sleep(250);
		
	}
}

int checkWinner(){	
	if (count >= 5) {
		for (int i = 0; i < 3; i++) {
			if (pad[i][0] == 1 && pad[i][1] == 1 && pad[i][2] == 1) {
				winner = 1;
				post = (i+4);
			}
			else if (pad[0][i] == 1 && pad[1][i] == 1 && pad[2][i] == 1) {
				winner = 1;
				post = (i+1);
			}
			else if (pad[i][0] == 2 && pad[i][1] == 2 && pad[i][2] == 2) {
				winner = 2;
				post = (i+4);
			}
			else if (pad[0][i] == 2 && pad[1][i] == 2 && pad[2][i] == 2) {
				winner = 2;
				post = (i + 1);

			}
		}

		if (pad[0][0] == 1 && pad[1][1] == 1 && pad[2][2] == 1) {
			winner = 1;
			post = 7;
		}
		else if (pad[0][2] == 1 && pad[1][1] == 1 && pad[2][0] == 1) {
			winner = 1;
			post = 8;
		}
		if (pad[0][0] == 2 && pad[1][1] == 2 && pad[2][2] == 2) {
			winner = 2;
			post = 7;
		}
		else if (pad[0][2] == 2 && pad[1][1] == 2 && pad[2][0] == 2) {
			winner = 2;
			post = 8;
		}
	}
	if (winner != 0) {
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				pad[i][j] = 0;

			}
		}
	}
	return winner, post;
	
}

int scoreKepper() {
	if (winner == 1){
		scoreA++;
	}
	if (winner == 2) {
		scoreB++;
	}
	if (winner == 0) {
		Sleep(0);
	}
	return scoreA, scoreB;

}

int main(int argc, char* argv[])
{
	if (canvasInit(WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, RESOLUTION_X, RESOLUTION_Y))
	{
		printf("ERROR: init failed (invalid argument?)\n");
		return EXIT_FAILURE;
	}

	initPixels();				
	initPad();
	
	while (canvasUpdate() == 0)
	{  	
		if (gameover == 0) {
			if (game > 5) {				
				showScore();
				if (entPressed == false) {
					break;
				}
			}
			if (count <= 9 && gameover == 0) {
				//post = 0;
				checkWinner();
				
				
				if (winner != 0 && count >=5) {
					//draw display for 10 sec
					
					printf("winner = %d,post = %d, Count = %d\n", winner, post, count);
					//printf(pad);
					gameover = 1;
					for (int i = 0; i <= 3; i++) {
						showPost(post, winner);
						showWinner(winner);
					}
				    Sleep(1000);
					
					scoreKepper();
					printf("count = %d,game = %d, player A = %d player B = %d\n", count, game, scoreA, scoreB);
					printf("gameover = %d,game = %d, player A = %d player B = %d\n", gameover, game, scoreA, scoreB);
					
					count = 0;
					initPad();
					showScore();
					game++;
					winner = 0;
					
					repaint();
					
				}
				else if (count == 9 && winner == 0) {
					scoreKepper();
					//draw display for 22 sec
					gameover = 1;
					for (int i = 0; i < 3; i++) {
						showWinner(1);
						showWinner(2);
					}
					printf("gameover = %d,game = %d, player A = %d player B = %d\n", gameover, game, scoreA, scoreB);
					count = 0;
					winner = 0;

					initPad();
					showScore();
					game++;
					repaint();
				}
				//player declaration
				if (entryA == false && count % 2 == 0) {
					player = 1;
				}
				else if (entryA == true && count % 2 == 1 && count < 9) {
					player = 2;
				}
				//move gameplay		
				if (entPressed == false)
					drawPad(player);
				if (entPressed == false && aPressed == false && sPressed == false && dPressed == false && wPressed == false && num % 5 == 0)
					blink(player);
				//set tile 
				while (entPressed == true) {

					if (player == 2 && count % 2 == 1 && count < 9) {
						pixels[xPad][yPad] = 1.0;
						pixels[xPad + 1][yPad] = 0;
						pixels[xPad + 1][yPad + 1] = 1.0;
						pixels[xPad][yPad + 1] = 0;
						updatePixels();
						entryA = false;
						count++;

						pad[xPad / 3][yPad / 3] = 2;
						printf("Player = %d, Count = %d\n", player, count);
						//xpad = % d, ypad = % d\nxPad, yPad
						PrintPlayerGrid(2);
						if (count != 0)
							checkPad();
						Sleep(speedBall);
						entPressed = false;
						break;
					}
					else if (player == 1 && count % 2 == 0) {
						entryA = true;
						pixels[xPad][yPad] = 1.0;
						pixels[xPad + 1][yPad] = 1.0;
						pixels[xPad + 1][yPad + 1] = 1.0;
						pixels[xPad][yPad + 1] = 1.0;
						updatePixels();
						count++;
						pad[xPad / 3][yPad / 3] = 1;
						printf("Player = %d, Count = %d\n", player, count);
						//PrintPlayerGrid(1);
						if (count != 0)
							checkPad();
						Sleep(speedBall);
						entPressed = false;
						break;
					}
					//printf("Player = %d, Count = %d\n", player, count);
				}
				updatePixels();
				repaint();
				Sleep(speedBall);
				num++;
			}
			continue;
		}
		showScore();
		gameover = 0;
					
	}
	//repaint();
	cleanup();

	return EXIT_SUCCESS;
}