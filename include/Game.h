#ifndef GAME_H
#define GAME_H

#include<SDL.h>

class Game
{
public:
    Game();
    //Initialize the Game
    bool Initialize();
    /* Runs the game loop */
    void RunLoop();
    /* Shutdown the game */
    void Shutdown();
private:
    /* Helper functions for the game loop */
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    /* Window Created by SDL */
    SDL_Window* mWindow;
    /* Game should continue to run */
    bool mIsRunning;
    /* Game renderer */
    SDL_Renderer* mRenderer;
    /* Wall thickness */
    const int thickness = 15;
    struct Vector2 {
        float x;
        float y;
    };
    Vector2 mPaddlePos = {30, 768/2};
    Vector2 mBallPos = {1024/2, 768/2};
};

#endif // GAME_H
