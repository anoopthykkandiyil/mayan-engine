#include "Game.h"
#include <SDL_scancode.h>
#include <SDL_keyboard.h>

Game::Game()
{
	//ctor
}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("[ERROR] Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 01) ", // Window Title
		100, // Top left X Coordiate of window
		100, // Top left Y Coordinate of window
		1024, // Width of window
		768,  // Height of window
		0
	);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow, /* Window to create the renderer for */
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	return true;
}


void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;

	// While there are still events in the queue
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}

	}
	/* Get the state of the keyboard */
	const Uint8* state = SDL_GetKeyboardState(NULL);
	/* If escape is pressed, also end loop */
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}
	/* If W key is pressed move paddle up */
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir -= 1;
	}
	/* If S key is pressed move paddle down */
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir += 1;
	}

}


void Game::UpdateGame()
{
	/* Wait until 16ms has elapsed since last frame */
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;
	/*
		Delta Time is the difference in ticks from last frame
		(converted to seconds)
	*/
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	/* update tick counts */
	mTicksCount = SDL_GetTicks();
	/* Clamp maximum delta time value */
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}


	/* Update objects in game world as function of delta time */
	if (mPaddleDir != 0) {
		mPaddlePos.y += mPaddleDir * 30.0f * deltaTime;
		/* Make sure that paddle doesnt move off screen ! */
		if (mPaddlePos.y < (paddleHeight / 2.0f + thickness)) {
			mPaddlePos.y = paddleHeight / 2.0f + thickness;
			mPaddleDir = 0;
		} else if (mPaddlePos.y > (768.0f - paddleHeight / 2.0f - thickness)) {
			mPaddlePos.y = 768.0f - paddleHeight / 2.0f - thickness;
			mPaddleDir = 0;
		}
	}
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1;
	}
	if (mBallPos.y > (768.0f) - thickness) {
		mBallVel.y *= -1;
	}
	if (mBallPos.x >= 1024 - thickness && mBallVel.x > 0.0f) {
		mBallVel.x *= -1;
	}
	diff = (mPaddlePos.y - mBallPos.y);
	if (diff < 0) {
		diff *= -1.0;
	}
	if (
		/* Our y-difference is small enough */
		diff <= paddleHeight / 2.0f &&
		/* Ball is at the correct x-position */
		mBallPos.x <= (25.0f + thickness) && mBallPos.x >= (2.0f + thickness) &&
		/* The ball is moving to the left */
		mBallVel.x < 0.0f) {
		mBallVel.x *= -1.0f;
	}
	/* if mBallPos.x goes negative pause and then stop the game */
	if (mBallPos.x < -1) {
		SDL_Delay(1500);
		mIsRunning = false;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 123, 255, 0xff);
	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_Rect topWall{ 0, 0, 1024, thickness };
	SDL_Rect bottomWall{ 0, 768 - thickness, 1024, thickness };
	SDL_Rect rightWall{ 1024 - thickness, 0, 1024, 768 };
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x - thickness / 2),
		static_cast<int>(mPaddlePos.y - paddleHeight / 2),
		thickness,
		paddleHeight
	};
	SDL_RenderFillRect(mRenderer, &topWall);
	SDL_RenderFillRect(mRenderer, &bottomWall);
	SDL_RenderFillRect(mRenderer, &rightWall);
	SDL_RenderFillRect(mRenderer, &ball);
	SDL_RenderFillRect(mRenderer, &paddle);
	SDL_RenderPresent(mRenderer);
}
