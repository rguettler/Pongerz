#include "AIE.h"
#include <iostream>
using namespace::std;

const char* donger = "./images/Pongers.png"; //Ball Image location
const char* paddle = "./images/paddlez.png"; // Paddle Image Location
const char* titleLoad = "./images/PongersTitle.png"; //Title Image Location
const char* bunk = "./images/bunk.png"; //High Score Image location
const char* highScore = "Todays High Score is:"; //High score text
const char* title = "RAISE UR PONGERS"; //Window Title
const char* gameStartText = "Press enter to start"; 
const char* highScoreStart = "Press H for high scores";
const char* hScoreReturn = "Press escape to return";
const char* exitText = "Press X to exit";


const int screenWidth = 1000;
const int screenHeight = 600;

void fakeasscollision(); //Collision Func
void gameloop(float deltaTime); //Gameplay Function
void mainmenu(); //Menu Function
void highscores(); //High Score Func

int bunkSprite; //High Score Sprite
int titleSplash; //Title Sprite
int scores[2]; //Score array



struct Ponger //Ball Structure
{
	unsigned int spriteID;
	float width = 60;
	float height = 50;
	float x = screenWidth * .5f;
	float y = screenHeight * .5f;
	float baseX = 500;
	float baseY = 750;
	float xSpeed = 500;
	float ySpeed = 750;

	void Move(float a_timeStep)
	{
		y += a_timeStep * ySpeed;
		if (y > screenHeight *.97f)
		{
			ySpeed = -ySpeed;
		}
		if (y < screenHeight * 0.03f)
		{
			ySpeed = ySpeed*-1;
		}
		x += a_timeStep * xSpeed;
		
	}


};
Ponger ponger;

struct Playerone //Left Paddle Structure
{
	char score[2];
	unsigned int spriteID;
	float width = 60;
	float height = 200;
	float x = screenWidth * .04f;
	float y = screenHeight * .5f;
	float speed;

	unsigned int moveUpKey;
	unsigned int moveDownKey;
	void SetMovementKeys(unsigned int a_moveUp, unsigned int a_moveDown)
	{
		moveUpKey = a_moveUp;
		moveDownKey = a_moveDown;
	}
	
	void Move(float a_timeStep, float a_speed)
	{
		if (IsKeyDown(moveUpKey))
		{
			y += a_timeStep * a_speed;
			if (y >  screenHeight*.83f)
			{
				y =  screenHeight*.83f;
			}
		}
		if (IsKeyDown(moveDownKey))
		{
			y -= a_timeStep * a_speed;
			if (y < screenHeight*.15f)
			{
				y = screenHeight*.15f;
			}
		}
		MoveSprite(spriteID, x, y);
	}

};
Playerone player1;

struct Playertwo // Right Paddle Structure
{
	char score[2];
	unsigned int spriteID;
	float x = screenWidth * .95f;
	float y = screenHeight * .5f;
	float speed;

	unsigned int moveUpKey;
	unsigned int moveDownKey;
	void SetMovementKeys(unsigned int a_moveUp, unsigned int a_moveDown)
	{
		moveUpKey = a_moveUp;
		moveDownKey = a_moveDown;
	}

	void Move(float a_timeStep, float a_speed)
	{
		if (IsKeyDown(moveUpKey))
		{
			y += a_timeStep * a_speed;
			if (y > screenHeight*.83f)
			{
				y = screenHeight*.83f;
			}
		}
		if (IsKeyDown(moveDownKey))
		{
			y -= a_timeStep * a_speed;
			if (y < screenHeight*.15f)
			{
				y = screenHeight*.15f;
			}
		}
		MoveSprite(spriteID, x, y);
	}
	
};
Playertwo player2;

enum GAMESTATES
{
	main_menu,
	gameplay,
	highscore,
};

int main( int argc, char* argv[] )
{	
    Initialise(screenWidth, screenHeight, false, "Raise Ur Pongers");
	
    
    SetBackgroundColour(SColour(0, 0, 0, 255));
	
	titleSplash = CreateSprite(titleLoad, 664, 119, true);
	bunkSprite = CreateSprite(bunk, 188, 128, true);
	ponger.spriteID = CreateSprite(donger, ponger.width, ponger.height, true);
	player1.spriteID = CreateSprite(paddle, player1.width, player1.height, true);
	player2.spriteID = CreateSprite(paddle, player1.width, player1.height, true);
	player1.SetMovementKeys('W', 'S');
	player2.SetMovementKeys(265,264);

	GAMESTATES currentState = main_menu;
    
	//Game Loop
	do
    {
		float deltaTime = GetDeltaTime();
		switch (currentState)
		{
		case main_menu:
			mainmenu();
			if (IsKeyDown(257))
			{
				currentState = gameplay;
			}
			if (IsKeyDown('H'))
			{
				currentState = highscore;
			}
			if (IsKeyDown('X'))
			{
				Shutdown();
				return 0;
			}
			break;



		case gameplay:
			gameloop(deltaTime);
			if (IsKeyDown(256))
			{
				currentState = main_menu;
			}
			break;

		case highscore:
			highscores();
			if (IsKeyDown(256))
			{
				currentState = main_menu;
			}
		}
		
        

        ClearScreen();

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
void gameloop(float deltaTime)
{

	itoa (scores[0], player1.score, 10);
	itoa (scores[1], player2.score, 10);
	MoveSprite(player1.spriteID, player1.x, player1.y);
	MoveSprite(player2.spriteID, player2.x, player2.y );
	MoveSprite(ponger.spriteID, ponger.x, ponger.y);
	
	DrawSprite(ponger.spriteID);
	DrawSprite(player1.spriteID);
	DrawSprite(player2.spriteID);
	DrawString(player1.score, screenWidth*.25, screenHeight - 25);
	DrawString(player2.score, screenWidth*.75, screenHeight - 25);
	
	player1.Move(deltaTime, 1000);
	player2.Move(deltaTime, 1000);
	ponger.Move(deltaTime);
		
	fakeasscollision();

	if (ponger.x > screenWidth) // Player 1 Win Con
	{
		scores[0] +=1;
		ponger.x = screenWidth *.5f;
		ponger.y = screenHeight*.5f;
		ponger.xSpeed = ponger.baseX*-1;
		

	}
	if (ponger.x < 0) //Player 2 Win Con
	{
		scores[1] +=1;
		ponger.x = screenWidth*.5f;
		ponger.y = screenHeight*.5f;
		ponger.xSpeed = ponger.baseX;

	}

}

void fakeasscollision()
{
	if (ponger.x + ponger.width >= player2.x && ((ponger.y <= player2.y + 100) && (ponger.y >= player2.y - 100))) //Right Paddle Collision
	{
		ponger.x -= 5;
		ponger.xSpeed += 75;
		ponger.xSpeed = ponger.xSpeed *-1;
		
	};
	if (ponger.x - ponger.width <= player1.x && ((ponger.y <= player1.y + 100) && (ponger.y >= player1.y - 100))) //Left Paddle Collision
	{
		ponger.x += 5;
		ponger.xSpeed -= 75;
		ponger.xSpeed = ponger.xSpeed *-1;

	}
}

void mainmenu()
{
	DrawSprite(titleSplash);
	MoveSprite(titleSplash, screenWidth*.5f, screenHeight*.85f);
	DrawString(gameStartText, screenWidth*.375f, screenHeight *.55f);
	DrawString(highScoreStart, screenWidth *.35f, screenHeight *.35f);
	DrawString(exitText, screenWidth*.41f, screenHeight*.25f);

}

void highscores()
{
	DrawString(highScore, screenWidth*.37f, screenHeight*.8f);
	DrawSprite(bunkSprite);
	MoveSprite(bunkSprite, screenWidth*.5f, screenHeight*.6f);

	DrawString(hScoreReturn, screenWidth*.36f, screenHeight*.4f);
}