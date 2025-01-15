#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#define HARDWARE_TYPE MD_MAX72XX::DR0CR1RR0_HW
#define MAX_DEVICES 1
#define CLK_PIN 13
#define CS_PIN 10
#define DATA_PIN 11
#define RESOLUTION_X	8
#define RESOLUTION_Y	8
#define	BUF_SIZE	75


uint8_t scrollSpeed = 50;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_RIGHT;
textPosition_t scrollAlign = PA_CENTER;
uint16_t scrollPause = 2000; // in milliseconds
//inout loactions
int PinYellow = 9; 
int PinRed = 7; 
int PinGreen = 8; 
int ButtonEnter = 6;
int ButtonLeft = 2;
int ButtonUp = 3;
int ButtonRight = 4; 
int ButtonDown= 5;
int buzzerPin =12;

//input variables
bool Left ;
bool Up ;
bool Right;
bool Enter ;
bool Down ;

//game variables
bool startGame;
bool entryA = false;
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
bool gameDone=false;
bool playerselected=false;
int playerA=1;
int playerB=2;
bool do_once = true;
char curMsg[BUF_SIZE]={ "" };
char NewMsg[BUF_SIZE]={ "" };

//output
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE,DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES );
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);


void setup() { // put your setup code here, to run once:
  pinMode(PinYellow, OUTPUT);
  pinMode(PinRed, OUTPUT);
  pinMode(PinGreen, OUTPUT);
  pinMode(ButtonLeft, INPUT);
  pinMode(ButtonRight, INPUT);
  pinMode(ButtonDown, INPUT);
  pinMode(ButtonUp, INPUT);
  pinMode(ButtonEnter, INPUT);
  pinMode(buzzerPin, OUTPUT);
  mx.begin();
  Serial.begin(9600);
  P.begin();
   P.setZoneEffect(0, true, PA_FLIP_UD);
  P.setZoneEffect(0, true, PA_FLIP_LR);
  P.displayText( curMsg, PA_CENTER, 50, 2000, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);

}
// initializing functions
void gameinit(){//reset everything for restarting new tournament
  startGame=false;
  entryA = false;
  player = 1;
  count = 0;
  num = 0;
  xPad = 0;
  yPad = 0;
  int score[5] = { 0,0,0,0,0 };
  gameover = 0;
  initPixels();
  initPad(); 
  winner = 0;
  scoreA = 0;
  scoreB = 0;
  game = 1;
  gameDone=false;
  do_once = true;
  //bool playerselected=false;
}
void initPixels(){//initializing 8*8 pixel pad to store led info
	for (int i = 0; i < RESOLUTION_X; i++)
	{
		for (int j = 0; j < RESOLUTION_Y; j++)
		{
			pixels[i][j] = 0;
		}
	}
}

void initPad(){//initializing 3*3 game pad to store player selections

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pad[i][j] = 0;
		}
	}
}

void updatePixels(){//refreshes the pixels
	for (int i = RESOLUTION_X-1; i >=0 ; i--)
	{
		for (int j = RESOLUTION_Y-1; j >=0 ; j--)
		{
			if (pixels[i][j] == 1.0)
				mx.setPoint(j,i , true);
			else
				mx.setPoint(j,i, false);
		}
	}
}

void updateIntensity(int scale) {//changes the pixel brightness
  mx.control( MD_MAX72XX::INTENSITY, MAX_INTENSITY*scale);
	for (int i = RESOLUTION_X-1; i >=0 ; i--)
	{
		for (int j = RESOLUTION_Y-1; j >=0 ; j--)
		{
			if (pixels[i][j] == 1.0)
				mx.setPoint(i,j , true);
			else
				mx.setPoint(i,j, false);
		}
	}
}

void Boardinit(){//reset everything for new game
  	count = 0;
		winner = 0;
		xPad = 0;
		yPad = 0;
		num = 0;
		entryA = false;
		initPixels();
		initPad();		
		for (int i = 0; i < RESOLUTION_X; i++)
		{
			for (int j = 0; j < RESOLUTION_Y; j++)
			{
				mx.setPoint(i, j, 0);
			}
		}
  for (int i = 2; i < RESOLUTION_X; i=i+3)
    {
      for (int j = 0; j < RESOLUTION_Y; j++)
      {
        pixels[i][j] = 1;
      }
    }
  for (int i = 0; i < RESOLUTION_X; i++)
    {
      for (int j = 2; j < RESOLUTION_Y; j=j+3)
      {
        pixels[i][j] = 1;
      }
    }
}
//input and gameplay
void buzzer(int frequency,int cycles){//takes frquency and no of time period to set buzzer to high and low
     for (int i=0;i<cycles;i++){
      digitalWrite(buzzerPin,HIGH);
      delay(frequency);
      digitalWrite(buzzerPin,LOW);
      delay(frequency);
     
     }
}
void buttonDebug(bool Left, bool Up, bool Right, bool Down, bool Enter) {//led and sound for button press
  if (Left==HIGH){
    digitalWrite(PinYellow,Left);
    buzzer(10,1);
    delay(200);
  }
  else if (Up==HIGH) {
    digitalWrite(PinYellow,Up);
    buzzer(15,1);
  }
  else if (Right==HIGH) {
    digitalWrite(PinYellow,Right);
    buzzer(20,1);
  }
  else if (Down==HIGH) {
    digitalWrite(PinYellow,Down);
    buzzer(30,1);
  }
  else if (Enter==HIGH) {
    digitalWrite(PinYellow,Enter);
    buzzer(50,2);
  }
  else {
    digitalWrite(PinYellow,LOW);
  }   
}

int checkPad() {//find empty location to move from
	
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

int drawPad(int x,bool Left, bool Up, bool Right, bool Down, bool Enter) {//move player in the game
	if (Enter==false) {
		pixels[xPad + 1][yPad] = 0;
		pixels[xPad][yPad] = 0;
		pixels[xPad + 1][yPad + 1] = 0;
		pixels[xPad][yPad + 1] = 0;
	}
	if (Right==HIGH && xPad < RESOLUTION_X - 2){
		if (Right==HIGH && (pixels[xPad+3][yPad])==0){
			xPad = xPad + 3;
    }
		else if (Right==HIGH && xPad == 0 && pixels[xPad + 3][yPad] == 1 && pixels[xPad + 6][yPad] == 0){
			xPad = xPad + 6;
    }
		else if (Right==HIGH && yPad != 6 && xPad == 0 && pixels[xPad + 3][yPad] == 1 && pixels[xPad + 6][yPad] == 1 && pixels[xPad][yPad+3] == 1 && pixels[xPad + 6][yPad + 3] == 0) {
			yPad = yPad + 3;
			xPad = xPad + 6;
		}
		else if (Right==HIGH && yPad != 0 && xPad == 0 && pixels[xPad + 3][yPad] == 1 && pixels[xPad + 6][yPad] == 1 && pixels[xPad][yPad-3] == 1 && pixels[xPad + 6][yPad - 3] == 0) {
			yPad = yPad - 3;
			xPad = xPad + 6;
		}
		else if (Right==HIGH && yPad != 0 && xPad != 6 && pixels[xPad + 3][yPad] == 1 && pixels[xPad + 3][yPad - 3] == 0) {
			yPad = yPad - 3;
			xPad = xPad + 3;
		}
		else if (Right==HIGH && yPad != 6 && xPad != 6 && pixels[xPad + 3][yPad] == 1 && pixels[xPad + 3][yPad + 3] == 0) {
			yPad = yPad + 3;
			xPad = xPad + 3;
		}
  }

	if (Left==HIGH && xPad > 0 && xPad < RESOLUTION_X - 1){
		if (Left==HIGH && pixels[xPad - 3][yPad] == 0) {
			xPad = xPad - 3;
		}
		else if (Left==HIGH && xPad == 6 && pixels[xPad - 3][yPad] == 1 && pixels[xPad - 6][yPad] == 0){
			xPad = xPad -6;
		}
		else if (Left==HIGH && yPad != 6 && xPad == 6 && pixels[xPad-3][yPad] == 1 && pixels[xPad-6][yPad] == 1 && pixels[xPad][yPad + 3] == 1 && pixels[xPad - 6][yPad + 3] == 0) {
			yPad = yPad + 3;
			xPad = xPad - 6;
		}
		else if (Left==HIGH && yPad != 0 && xPad == 6 && pixels[xPad - 3][yPad] == 1 && pixels[xPad - 6][yPad] == 1 && pixels[xPad][yPad - 3] == 1 && pixels[xPad - 6][yPad - 3] == 0) {
			yPad = yPad - 3;
			xPad = xPad - 6;
		}
		else if (Left==HIGH && yPad != 0 && xPad != 0 && pixels[xPad-3][yPad] == 1 && pixels[xPad - 3][yPad - 3] == 0) {
			yPad = yPad - 3;
			xPad = xPad - 3;
		}
		else if (Left==HIGH && yPad != 6 && xPad != 0 && pixels[xPad - 3][yPad] == 1 && pixels[xPad - 3][yPad + 3] == 0) {
			yPad = yPad + 3;
			xPad = xPad - 3;
		}
  }

	if (Up==HIGH && yPad < RESOLUTION_Y - 2){ 
		//yPad = yPad + 3;
		if (Up==HIGH && pixels[xPad][yPad + 3] == 0 ){ 
			yPad = yPad + 3;
    }
	  else if (Up==HIGH && yPad != 6 && xPad != 6 && pixels[xPad][yPad + 3] == 1 && pixels[xPad + 3][yPad + 3] == 0){
			yPad = yPad + 3;
			xPad = xPad + 3;		
		}
		else if (Up==HIGH && yPad != 6 && xPad != 0 && pixels[xPad][yPad + 3] == 1 && pixels[xPad - 3][yPad + 3] == 0) {
			yPad = yPad + 3;
			xPad = xPad - 3;
		}
		else if (Up==HIGH && yPad ==0  && xPad != 0 && pixels[xPad][yPad + 3] == 1 && pixels[xPad][yPad + 6] == 1 && pixels[xPad - 3][yPad + 3] == 1 &&  pixels[xPad - 3][yPad + 6] == 0) {
			yPad = yPad + 6;
			xPad = xPad - 3;
		}
		else if (Up==HIGH && yPad == 0 && xPad != 6 && pixels[xPad][yPad + 3] == 1 && pixels[xPad][yPad + 6] == 1 && pixels[xPad + 3][yPad + 3] == 1 && pixels[xPad + 3][yPad + 6] == 0) {
			yPad = yPad + 6;
			xPad = xPad + 3;
		}
		else if (Up==HIGH && yPad == 0 && pixels[xPad][yPad + 3] == 1 && pixels[xPad][yPad + 6] == 0){
			yPad = yPad + 6;
    }
  }

	if (Down==HIGH && yPad > 0 && yPad < RESOLUTION_Y - 1){
		//yPad = yPad - 3;
		if (Down==HIGH && pixels[xPad][yPad-3] == 0){
			yPad = yPad - 3;
    }
		else if (Down==HIGH && yPad != 0 && xPad != 0 && pixels[xPad][yPad - 3] == 1 && pixels[xPad - 3][yPad - 3] == 0) {
			yPad = yPad - 3;
			xPad = xPad - 3;
		}
		else if (Down==HIGH && yPad != 0 && xPad != 6 && pixels[xPad][yPad - 3] == 1 && pixels[xPad + 3][yPad - 3] == 0 ) {
			yPad = yPad - 3;
			xPad = xPad + 3;
		}
		else if (Down==HIGH && yPad == 6 && pixels[xPad][yPad - 3] == 1 && pixels[xPad][yPad - 6] == 0)
			yPad = yPad - 6;
		else if (Down==HIGH && yPad == 6  && xPad != 0 && pixels[xPad][yPad-3] == 1 && pixels[xPad][yPad-6] == 1 && pixels[xPad-3][yPad-3] == 1 && pixels[xPad - 3][yPad - 6] == 0) {
			yPad = yPad - 6;
			xPad = xPad - 3;
		}
		else if (Down==HIGH && yPad == 6 && xPad != 6 && pixels[xPad][yPad - 3] == 1 && pixels[xPad][yPad - 6] == 1 && pixels[xPad + 3][yPad - 3] == 1 && pixels[xPad + 3][yPad - 6] == 0) {
			yPad = yPad - 6;
			xPad = xPad + 3;
		}
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
	return  xPad,yPad;
}

void blink(int x) {//blinks player when not moving 
	if (Enter == LOW) {
		pixels[xPad + 1][yPad] = 0;
		pixels[xPad][yPad] = 0;
		pixels[xPad + 1][yPad + 1] = 0;
		pixels[xPad][yPad + 1] = 0;
		updatePixels();
		delay(100);
		if (x == 1) {
			pixels[xPad + 1][yPad] = 1;
			pixels[xPad][yPad] = 1;
			pixels[xPad + 1][yPad + 1] = 1;
			pixels[xPad][yPad + 1] = 1;
		}
		else if (x == 2) {
			pixels[xPad + 1][yPad] = 0;
			pixels[xPad][yPad] = 1;
			pixels[xPad + 1][yPad + 1] = 1;
			pixels[xPad][yPad + 1] = 0;
		}
		updatePixels();
	}
}

//score and credits

void showPost(int x,int y) {//shows the winning sequence given winner and sequnece out of the 8 possibilities 3 horizontal ,3 vertical, 2 diag(Lefttop2rightbottom,righttop2leftbottom)
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
		delay(0);
	}
	updatePixels();
	
	delay(500);
}

void showWinner(int x) {//displays the winner given player [1,2] 
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
		delay(0);
	}	
	updatePixels();
	delay(250);
	initPad();
	for (int i = 0; i < RESOLUTION_X; i++)
	{
		for (int j = 0; j < RESOLUTION_Y; j++)
		{
			mx.setPoint(i, j, 0);

		}
	}

	
}

void showScore() {//displays the score after each game for player[1,2] at [3,3][4,3] to[3,7][4,7]
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
    for(int i=0;i<=(game-1);i++){
      if (score[i] == 1) {
        pixels[3][4-i] = 1;
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
  else if (game>5){
    for(int i=0;i<=4;i++){
      if (score[i] == 1) {
        pixels[3][4-i] = 1;
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
	if (gameover == 1 && game<=5){
     delay(2000); 
	   Boardinit();
     updatePixels();		
     gameover = 0;
  }
}

int checkWinner(){	//checks if the game is over after 5 selections and returns winner and winning sequence 
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

int scoreKepper() {//stores the scores for each player win is  1 draw/loss 0
	if (winner == playerA){
		scoreA++;
	}
	if (winner == playerB) {
		scoreB++;
	}
	if (winner == 0) {
		delay(0);
	}
	return scoreA, scoreB;
}
void gameCredits(){//displays the winner by player A/B;
  if (scoreA>scoreB){
    char winnerA[BUF_SIZE]={"Winner A - New Game ??"};
    strcpy(NewMsg,winnerA);
    Serial.print(NewMsg);
  }
  else if (scoreA<scoreB){    
    char winnerB[BUF_SIZE]={"Winner B - New Game ??"};
    strcpy(NewMsg,winnerB);
    Serial.print(NewMsg);   
  }
  else if (scoreA==scoreB){  
    char text[BUF_SIZE]={"Draw Game -- New Game ??"};
    strcpy(NewMsg,text);
    Serial.print(NewMsg);
  }
}

//not used
void SelectPlayer(bool Left, bool Up, bool Right, bool Down, bool Enter) {//selecting player before the game 
   xPad=3;
   yPad=3;
   if (playerselected==false){
    if (playerA==1 &&(Left||Up||Right||Down==HIGH)) {
      initPixels();
      playerA=2;
      playerB=1;
      drawPad(1, LOW,LOW,LOW,LOW,HIGH);
    }
    else if (playerA==2 &&(Left||Up||Right||Down==HIGH)) {
      initPixels();
      playerA=1;
      playerB=2;
      drawPad(2, LOW,LOW,LOW,LOW,HIGH);
    } 
    updatePixels();
    if (Enter==HIGH){
      xPad=0;
      yPad=0;
      playerselected=true;
       
    } 
   } 
}
void PrintPlayerGrid(int x) {//debug store and print player selections
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (pad[i][j] == x)
				printf("%d %d\n", 3*i, 3*j);
		}
	}
}

void loop() {
  digitalWrite(PinGreen, LOW);
  digitalWrite(PinRed, HIGH);
  delay(100);
  initPixels();
  Boardinit();
  updatePixels();
  Left = digitalRead(ButtonLeft);
  Up = digitalRead(ButtonUp);
  Right = digitalRead(ButtonRight);
  Enter = digitalRead(ButtonEnter);
  Down = digitalRead(ButtonDown);
  buttonDebug(Left, Up, Right, Down, Enter);
  if (Enter ==HIGH){
     gameinit();
     Boardinit();
     updateIntensity(0.25);
     startGame = true;
     Enter=LOW; 
     delay(100);        
  }
  while (startGame==true){
    digitalWrite(PinGreen, HIGH);
    digitalWrite(PinRed, LOW);
    Left = digitalRead(ButtonLeft);
    Up = digitalRead(ButtonUp);
    Right = digitalRead(ButtonRight);
    Enter = digitalRead(ButtonEnter);
    Down = digitalRead(ButtonDown);
    buttonDebug(Left, Up, Right, Down, Enter);
 		if (count <= 9 && gameover == 0 && game <=5) {
     checkWinner();    
     /*debug each selection
     for (int i = 0; i <3; i++) {
					Serial.print(pad[0][i]);
					Serial.print(pad[1][i]);
					Serial.print(pad[2][i]);
          if(i==2){
            	Serial.print("end");
          }
          	Serial.print('\n');
			} */ 
      if (winner != 0 && count >=5) {
          
         gameover = 1;
        for (int i = 0; i <= 2; i++) {
          showPost(post, winner);
          showWinner(winner);
        }
        scoreKepper();
        /* debug winner*/
        Serial.print("scoreA");
        Serial.print(scoreA);
        Serial.print("scoreB");
        Serial.print(scoreB);  
        showScore();
        game++;
        continue;
      }
      else if (count >= 9 && winner == 0) {
        scoreKepper();
        gameover = 1;
        for (int i = 0; i < 2; i++) {
          showWinner(1);
          showWinner(2);
        }
        showScore();
        game++;
        continue;
      }      
      //player declaration
      if (entryA == false && count % 2 == 0) {
        player = 1;
      }
      else if (entryA == true && count % 2 == 1 && count < 9) {
        player = 2;
      }
       if (game%2==0 && entryA == false && count % 2 == 1) {
        player = 1;
      }
      else if (game%2==0 && entryA == true && count % 2 == 0 && count < 9) {
        player = 2;
      }
      if (game%2 == 0 && count  == 0) {
        player = playerB;
      }
      else if (game%2 == 1 && count  == 0) {
        player = playerA;
      }

      if (Enter == LOW){
        drawPad(player,Left, Up, Right, Down, Enter);
        updatePixels();      
      }
      if (Enter == LOW && Left == LOW && Up == LOW && Right == LOW && Down == LOW ){
        blink(player);
      }

      while (Enter == HIGH && count >=0) {
        if (player == 2 &&  count < 9) {
          pixels[xPad][yPad] = 1.0;
          pixels[xPad + 1][yPad] = 0;
          pixels[xPad + 1][yPad + 1] = 1.0;
          pixels[xPad][yPad + 1] = 0;
          updatePixels();
          entryA = false;
          count++;
          pad[xPad / 3][yPad / 3] = 2;
          //PrintPlayerGrid(2);
          if (count != 0)
            checkPad();          
        }
        else if (player == 1 && count <= 9) {
          entryA = true;
          pixels[xPad][yPad] = 1.0;
          pixels[xPad + 1][yPad] = 1.0;
          pixels[xPad + 1][yPad + 1] = 1.0;
          pixels[xPad][yPad + 1] = 1.0;
          updatePixels();
          count++;
          pad[xPad / 3][yPad / 3] = 1;
          if (count != 0)
            checkPad();
        }
        Enter = LOW;
        delay(10);
        break;             
      }    
    }
    while (game > 5) {      
      showScore();
      //delay(10);
      Enter = digitalRead(ButtonEnter);
      if (Enter == HIGH) {
        digitalWrite(PinGreen, LOW);
        digitalWrite(PinRed, HIGH);
        delay(1000);
        scoreKepper();
        startGame=false;
        gameDone=true;
        break;
      }
    }
    while(gameDone==true){   
      if (P.displayAnimate()) { 
          do_once=true;
          if (do_once){                
            gameCredits();
            Serial.print(NewMsg);
            strcpy(curMsg, NewMsg);
            do_once=false;
          }          
         P.displayReset();
      }
      Enter=LOW;
      Enter = digitalRead(ButtonEnter);
      if (Enter == HIGH) {
        startGame=false;
        Enter=LOW;
        break;
      }
    }   	  
  }  
}