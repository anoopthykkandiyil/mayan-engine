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
    if (sdlResult!=0) {
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
    if(!mWindow) {
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
    while(mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;

    // While there are still events in the queue
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }

    }
    /* Get the state of the keyboard */
    const Uint8* state = SDL_GetKeyboardState(NULL);
    /* If escape is pressed, also end loop */
    if(state[SDL_SCANCODE_ESCAPE]) {
        SDL_Log("Inside the SDL SCANCODE ESCAPE function success region");

//        SDL_Delay(1000);
//        mIsRunning = false;
    }
}


void Game::UpdateGame() {
    mPaddlePos.y++;
    mBallPos.x++;
    mBallPos.y++;
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer, 0, 123, 255, 0xff);
    SDL_RenderClear(mRenderer);
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_Rect topWall{0, 0, 1024, thickness};
    SDL_Rect bottomWall{0, 768-thickness, 1024, thickness};
    SDL_Rect rightWall{1024-thickness, 0, 1024, 768};
    SDL_Rect ball {
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),
        thickness,
        thickness
    };
    SDL_Rect paddle {
        static_cast<int>(mPaddlePos.x - thickness/2),
        static_cast<int>(mPaddlePos.y - 40),
        thickness,
        80
    };
    SDL_RenderFillRect(mRenderer, &topWall);
    SDL_RenderFillRect(mRenderer, &bottomWall);
    SDL_RenderFillRect(mRenderer, &rightWall);
    SDL_RenderFillRect(mRenderer, &ball);
    SDL_RenderFillRect(mRenderer, &paddle);
    SDL_RenderPresent(mRenderer);
}
