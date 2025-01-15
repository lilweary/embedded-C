# TicTacToe


## Game Design and Implementation for Arduino.

## Description
This repository is used to track and exchange project files for the embedded systems course. The repository contains following folders:<br/>  

>1)  *[10d-8x8BouncingPixel](https://gitlab.inf.unibz.it/2223-embedded_systems/tictactoe-mallavar-ghani/-/tree/main/10d-8x8BouncingPixel)* : Visual studio solution of the Game. For details see [VisualStudioCode](#visual-studio-code).
>2) *[Tic_Tac_Toe_Arduino](https://gitlab.inf.unibz.it/2223-embedded_systems/tictactoe-mallavar-ghani/-/tree/main/TIC_TAC_TOE_arduino)*  : Arduino solution of the Game. For details see [Arduino code](#arduino-code).
>3) *[fritzing](https://gitlab.inf.unibz.it/2223-embedded_systems/tictactoe-mallavar-ghani/-/tree/main/fritzing)*            : CAD of the circuit. For details see [Digital Model](#digital-model).
>4) *[Video Presentation](https://gitlab.inf.unibz.it/2223-embedded_systems/tictactoe-mallavar-ghani/-/tree/main/Video%20Presentation)*   : Video Description of the Gameplay. For details see [GamePlay](#gameplay).
>5) *[Power-Point Presentation](https://gitlab.inf.unibz.it/2223-embedded_systems/tictactoe-mallavar-ghani/-/tree/main/Powerpoint)*  : PPT files.


## Visual Studio Code
In the *[10d-8x8BouncingPixel](https://gitlab.inf.unibz.it/2223-embedded_systems/tictactoe-mallavar-ghani/-/tree/main/10d-8x8BouncingPixel)* Visual Studio solution, the project folder *[10d-8x8Game](https://gitlab.inf.unibz.it/2223-embedded_systems/tictactoe-mallavar-ghani/-/tree/main/10d-8x8BouncingPixel/10d-8x8Game/)* contains the program files. The developed game could be modified or examined through the following files:<br/>
   >- main.cpp , where the game logic is defined.
   >- canvas.cpp, where the game inputs and screen graphics is defined.
   >- canvas.h, where inputs for canvas.cpp are initialized.

The player inputs are **W-A-S-D** for moving UP, LEFT, DOWN and RIGHT, respectively. The '**Enter**'- key is used to record player selection. The game is built for 8 by 8 tiles LED matrix and the screen resembles such matrix. The [GamePlay](#gameplay) remains same for both VS and Arduino solutions. This code is further improved for used with Arduino. Refer to [Arduino code](#arduino-code) for information on functions used in the main.cpp. 



## Arduino Code
The Arduino solution for the TICTACTOE is found at *[Tic_Tac_Toe_Arduino](https://gitlab.inf.unibz.it/2223-embedded_systems/tictactoe-mallavar-ghani/-/tree/main/TIC_TAC_TOE_arduino)*. The game on arduino is designed to accept five inputs similar to the VS Solution, but it uses five outputs namely [3 LED's, 1 Buzzer, and a the LED Matrix]. the schematic could be found at [Digital Model](#digital-model).The 3 LED's and an Active buzzer are chosen to inform the player. for example when Red Led is on the game has not started but power is ON.and Green LED is on when the game is in progress. The YELLOW LED and Active Buzzer are active if any of the buttons are pressed. 

A typical Arduino program could be divided into 4 sections:<br />
   >- Importing dependencies and Declaring Variables.<br/>
   >- Setup() to call one time funtions.
   >- loop() is reccuring part of the solution.
   >- custom functions (optional).

The custom funtions used for the project could be divded into 3 Sections:<br/>
 >- functions that initialize or reset variables.
 >- functions that read the inputs, update Game.  and,
 >- finally the functions that display the game results  or **Set** (_a set of 5 games_) results.

 The code mainly depends upon two matrices namely the Pixels(8x8) and Pad(3x3). The **_pixles_** store boolean value for each led in the matrix, while the **_pad_** records stores a integer value one or two; given if the moves is performed by player A or B respectively. Both these matrices have their individual functions(**_initpixels_**, **_initPad_**) to set all value to zero.similarly **_boardinit_** and **_gameinit_** functions are used to reinitialize variable after each game and set.The pixels are read by an **_updatePixels_** function to set appropriate LED's on the matrix to HIGH.<br/>
<br/> 
   A **_buzzer_** function is created to set the buzzer to high for a given cycle time and number of cycles. The **_buttonDebug_** function which enable the YELLOW LED to HIGH and a distinct sound from the buzzer. The **_DrawPad_** function helps the players move their tiles on the gamepad and the **_checkPad_** is used to find an intial empty postion for a player to move from. Finally, the blink function blinks the current player when stationary.<br/>
<br/> 
The **_checkWinner_** isused to verfiy if any player has the winning moves after 5 plays and **_scoreKeeper_** functions increments the player wins by one given a winner provided by the checkWinner function.  
**_showPost_**,**_showWinner_**,**_ShowScore_**, functions are used to show the winning sequence,the player won and the scoreboard respectively. In the end, the **_gameCredits_** function is used to publish appropriate winning message.

## Digital Model
 ### Bill Of Materials:
  | Items | Quantity |
| ------ | ------ |
|   Buttons     |   5     |
|      Active Buzzer  |      1   |
|   Bread Board     |   1     |
|      Arduino uno  |      1   |
|   LED Matrix     |   1     |
|      Red LED  |      1   |
|      Green LED  |      1   |
|      Yellow LED  |      1   |
|      Resistors(250-330 ohms)  |     8   |
_optional potentiometer_ |1|

The Resistors are used Pull-down for Buttons and as voltage regulators for the Red,Green and Yellow LED's.A potentiometer could be used to regulate the volume of the buzzer, as the buzzer used takes digital inputs it could only be turn OFF/ON.
 
<br>

![The Circuit schematics for the Arduino](/fritzing/tic_tac_toe_circuit.svg)

*The Arduino Circuit for TicTacToe Game*
 


## GamePlay
The developed **_TICTACTOE_** is a multiplayer game, where two players are represented with a distinct style of tile each.On a 3-by-3 board the player with 3 consecutive tiles either horizontal, vertical or diagonal wins the game.<br/>
<br/> The players compete for a total of five games and the player with most wins; win the Set. The players A & B take turns for the first move, where even numbered games are started by player B. 

## Roadmap
### Future ideas
   >- Player vs Computer.
   >- Optimizing code for low memory and efficiency.

## Contributing
State if you are open to contributions and what your requirements are for accepting them.

For people who want to make changes to your project, it's helpful to have some documentation on how to get started. Perhaps there is a script that they should run or some environment variables that they need to set. Make these steps explicit. These instructions could also be useful to your future self.

You can also document commands to lint the code or run tests. These steps help to ensure high code quality and reduce the likelihood that the changes inadvertently break something. Having instructions for running tests is especially helpful if it requires external setup, such as starting a Selenium server for testing in a browser.

## Authors and acknowledgment
The base project Files from the *[10d-8x8Game](https://gitlab.inf.unibz.it/2223-embedded_systems/tictactoe-mallavar-ghani/-/tree/main/10d-8x8BouncingPixel/10d-8x8Game/)* are developed and supplied by Prof.Dr.Pretto.<br/>

The project is delivered for _Emmbeded Systems: Design and Implementation_, final Evaluation by Rabert Rajesh Mallavarapu.
## License
UNIBZ Educational License.
## Project status
NOT Open for Evaluation and critics.
