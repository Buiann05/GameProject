#include <SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 32;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;

// Mảng hai chiều đại diện cho map
int mapData[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, ...}, // Dòng 1
    {1, 0, 0, 0, 1, ...}, // Dòng 2
    {1, 0, 1, 0, 1, ...}, // Dòng 3
    // ...
};

// Hàm để tải hình ảnh từ tệp và tạo texture
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) {
        std::cerr << "Unable to load image " << path << "! SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Hàm để vẽ map lên màn hình
void drawMap(SDL_Renderer* renderer, SDL_Texture* obstacleTexture) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            // Xác định loại ô trong map và vẽ tương ứng
            if (mapData[y][x] == 1) {
                // Vẽ ô có chướng ngại vật
                SDL_Rect obstacleRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderCopy(renderer, obstacleTexture, nullptr, &obstacleRect);
            } else {
                // Vẽ ô trống
                // SDL_Rect emptyRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                // SDL_RenderFillRect(renderer, &emptyRect);
            }
        }
    }
}

int main() {
    // Khởi tạo SDL và tạo cửa sổ game
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Map Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Tải hình ảnh chướng ngại vật
    SDL_Texture* obstacleTexture = loadTexture(renderer, "Vatcan.bmp");
    if (!obstacleTexture) {
        return 1;
    }

    // Game loop
    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Vẽ map lên màn hình
        drawMap(renderer, obstacleTexture);

        // Cập nhật màn hình
        SDL_RenderPresent(renderer);
    }

    // Giải phóng bộ nhớ và thoát SDL
    SDL_DestroyTexture(obstacleTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
