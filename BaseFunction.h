#ifndef BASE_FUNCTION_H_
#define BASE_FUNCTION_H_

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>


static SDL_Window* gWindow = NULL;
static SDL_Renderer* gScreen = NULL;
static SDL_Event gEvent;
static SDL_Texture* gTilesetTexture = NULL;
static SDL_Texture* gBackground = NULL;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;
const int FRAME_PER_SECOND = 30;

const int COLOR_KEY_R = 255;
const int COLOR_KEY_G = 255;
const int COLOR_KEY_B = 255;

const int MAP_HEIGHT = 10;
const int MAP_WIDTH = 20;

const int TILE_SIZE = 64;

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 10
#define PLAYER_JUMP 15

typedef struct Input {
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
};

#endif // BASE_FUNCTION_H_
