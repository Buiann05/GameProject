#ifndef BASE_FUNCTION_H_
#define BASE_FUNCTION_H_

#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

SDL_Window* gWindow = nullptr;
SDL_Renderer* gScreen = nullptr;
SDL_Event* gEvent;

// Các thông số môi trường cơ bản của game
// Thông số màn hình
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int SCREEN_BPP = 32;

// Thông số màu sắc
const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;


#endif // BASE_FUNCTION_H_
