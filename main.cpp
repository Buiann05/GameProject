#include "LoadPicture.h"
#include "BaseFunction.h"
#include "Time.h"
#include "Dangerous.h"
#include "TextObject.h"
#include "PlayPower.h"

TTF_Font* font_time = NULL;
LoadPicture menuBackground;
SDL_Texture* gameOverBackground = NULL;

bool loadMenuBackground(const std::string& imagePath) {
    if (!menuBackground.loadIMG(imagePath, gScreen)) {
        std::cout << "Failed to load menu background image!" << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

const int mapData[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

enum MenuOption {
    START_GAME,
    QUIT_GAME,
    REPLAY,
    EXIT
};

void renderMenu(SDL_Renderer* renderer, TTF_Font* font, int selectedOption) {
    menuBackground.Render(renderer);

    const std::string menuOptions[] = {"Play", "Exit"};

    for (int i = 0; i < 2; ++i) {
        TextObject optionText;
        optionText.SetText(menuOptions[i]);

        if (i == selectedOption) {
            optionText.SetColor(255, 0, 0);
        } else {
            optionText.SetColor(255, 255, 255);
        }

        optionText.LoadFromRenderText(font, renderer);
        optionText.RenderText(renderer, SCREEN_WIDTH / 2 - optionText.GetWidth() / 2, SCREEN_HEIGHT / 2 + i * 40);
        optionText.Free();
    }
}

void handleMenuInput(bool& quit, int& selectedOption, bool& startGame) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    selectedOption--;
                    if (selectedOption < 0) selectedOption = 1;
                    break;
                case SDLK_DOWN:
                    selectedOption++;
                    if (selectedOption > 1) selectedOption = 0;
                    break;
                case SDLK_RETURN:
                    if (selectedOption == START_GAME) {
                        startGame = true;
                    } else if (selectedOption == QUIT_GAME) {
                        quit = true;
                    }
                    break;
            }
        }
    }
}

bool loadBackground(const std::string& imagePath) {
    SDL_Surface* loadedSurface = IMG_Load(imagePath.c_str());
    if (loadedSurface == nullptr) {
        std::cout << "Unable to load image " << imagePath << "! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gBackground = SDL_CreateTextureFromSurface(gScreen, loadedSurface);
    if (gBackground == nullptr) {
        std::cout << "Unable to create texture from " << imagePath << "! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_FreeSurface(loadedSurface);
    return true;
}

bool loadTileset(const std::string& path) {
    SDL_Surface* tilesetSurface = IMG_Load(path.c_str());
    if (tilesetSurface == nullptr) {
        std::cerr << "Failed to load tileset image! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gTilesetTexture = SDL_CreateTextureFromSurface(gScreen, tilesetSurface);
    SDL_FreeSurface(tilesetSurface);

    if (gTilesetTexture == nullptr) {
        std::cerr << "Failed to create texture from tileset image! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

bool loadGameOverBackground(const std::string& path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    gameOverBackground = SDL_CreateTextureFromSurface(gScreen, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (gameOverBackground == nullptr) {
        std::cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void close() {
    SDL_DestroyTexture(gBackground);
    gBackground = NULL;

    SDL_DestroyTexture(gTilesetTexture);
    gTilesetTexture = NULL;

    SDL_DestroyRenderer(gScreen);
    gScreen=NULL;

    SDL_DestroyTexture(gameOverBackground);
    gameOverBackground = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow=NULL;

    IMG_Quit();
    SDL_Quit();
}

void renderMap() {
    SDL_RenderClear(gScreen);

    SDL_Rect destRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(gScreen, gBackground, nullptr, &destRect);

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            SDL_Rect srcRect = { mapData[y][x] * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
            SDL_Rect destRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderCopy(gScreen, gTilesetTexture, &srcRect, &destRect);
        }
    }
}

class  Player : public LoadPicture {
public:
    Player() {
        frame_ = 0;
        x_pos_ =0;
        y_pos_ =0;
        x_val_ = 0;
        y_val_ = 0;
        status_ = -1;
        width_frame_ = 0;
        height_frame_ = 0;
        input_type_.down_ = 0;
        input_type_.jump_ = 0;
        input_type_.left_ = 0;
        input_type_.right_ = 0;
        input_type_.up_ = 0;
        on_ground_=false;
        is_falling_=false;
    }

    ~Player() {
        Free();
    }

    enum WalkType {
        WALK_RIGHT = 0,
        WALK_LEFT = 1,
    };

    bool loadIMG(std::string path, SDL_Renderer* screen) {
        bool ret = LoadPicture::loadIMG(path,screen);

        if (ret == true) {
            width_frame_ = rect_.w/6;
            height_frame_ = rect_.h;
        } else {
            ret = false;
        }

        return ret;
    }
    void set_clips() {
        if (width_frame_>0 && height_frame_>0) {
            frame_clip_[0].x = 0;
            frame_clip_[0].y = 0;
            frame_clip_[0].w = width_frame_;
            frame_clip_[0].h = height_frame_;

            frame_clip_[1].x = width_frame_;
            frame_clip_[1].y = 0;
            frame_clip_[1].w = width_frame_;
            frame_clip_[1].h = height_frame_;

            frame_clip_[2].x = 2*width_frame_;
            frame_clip_[2].y = 0;
            frame_clip_[2].w = width_frame_;
            frame_clip_[2].h = height_frame_;

            frame_clip_[3].x = 3*width_frame_;
            frame_clip_[3].y = 0;
            frame_clip_[3].w = width_frame_;
            frame_clip_[3].h = height_frame_;

            frame_clip_[4].x = 4*width_frame_;
            frame_clip_[4].y = 0;
            frame_clip_[4].w = width_frame_;
            frame_clip_[4].h = height_frame_;

            frame_clip_[5].x = 5*width_frame_;
            frame_clip_[5].y = 0;
            frame_clip_[5].w = width_frame_;
            frame_clip_[5].h = height_frame_;
        }
    }
    void ShowPlayer(SDL_Renderer* des) {
       if (status_ == WALK_LEFT) {
            loadIMG("image/player_left.png",des);
        } else {
            loadIMG("image/player_right.png",des);
        }

        if (input_type_.left_ == 1 || input_type_.right_ ==1) {
            frame_++;
        } else {
            frame_=0;
        }

        if (frame_ >= 6) {
            frame_ = 0;
        }

        rect_.x=x_pos_;
        rect_.y=y_pos_;

        SDL_Rect* current_clip = &frame_clip_[frame_];

        SDL_Rect renderQuad = {rect_.x,rect_.y,width_frame_,height_frame_};

        SDL_RenderCopy(des,p_object_,current_clip,&renderQuad);
    }
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen) {
        if (events.type == SDL_KEYDOWN) {
            switch (events.key.keysym.sym) {
                case SDLK_RIGHT:
                    {
                        status_ = WALK_RIGHT;
                        input_type_.right_ = 1;
                        input_type_.left_ = 0;
                    }
                    break;
                case SDLK_LEFT:
                    {
                        status_ = WALK_LEFT;
                        input_type_.left_ = 1;
                        input_type_.right_ = 0;
                    }
                    break;

                case SDLK_SPACE:
                    {
                        input_type_.jump_ = 1;
                    }
            }
        } else if (events.type == SDL_KEYUP) {
            switch (events.key.keysym.sym) {
                case SDLK_RIGHT:
                    {
                        input_type_.right_ = 0;
                    }
                    break;
                case SDLK_LEFT:
                    {
                        input_type_.left_ = 0;
                    }
                    break;
            }
        }
    }
    void DoPlayer() {
        x_val_ = 0;
        y_val_ += 0.8;

        if (y_val_ >= MAX_FALL_SPEED) {
            y_val_ = MAX_FALL_SPEED;
        }
        if (input_type_.right_ == 1) {
                x_val_+=PLAYER_SPEED;
        } else if (input_type_.left_ == 1) {
            x_val_-=PLAYER_SPEED;
        }
        if (input_type_.jump_ == 1 && on_ground_ && !is_falling_) {
            y_val_ = -PLAYER_JUMP;
            on_ground_ = false;
            is_falling_ = true;
            input_type_.jump_=0;
        }
        CheckToMap();
    }
    void CheckToMap() {
        int x1, x2;
        int y1, y2;

        int new_x_pos = x_pos_ + x_val_;
        x1 = new_x_pos / TILE_SIZE;
        x2 = (new_x_pos + width_frame_ - 1) / TILE_SIZE;
        y1 = y_pos_ / TILE_SIZE;
        y2 = (y_pos_ + height_frame_ - 1) / TILE_SIZE;

        if (x1 >= 0 && x2 < MAP_WIDTH && y1 >= 0 && y2 < MAP_HEIGHT) {
            if (x_val_ > 0) {
                if (mapData[y1][x2] == 0 || mapData[y2][x2] == 0) {
                    x_pos_ = x2 * TILE_SIZE;
                    x_pos_ -= width_frame_ + 1;
                    x_val_ = 0;
                }
            } else if (x_val_ < 0) {
                if (mapData[y1][x1] == 0 || mapData[y2][x1] == 0) {
                    x_pos_ = (x1 + 1) * TILE_SIZE;
                    x_val_ = 0;
                }
            }
        }

        x_pos_ += x_val_;

        int new_y_pos = y_pos_ + y_val_;
        x1 = x_pos_ / TILE_SIZE;
        x2 = (x_pos_ + width_frame_ - 1) / TILE_SIZE;
        y1 = new_y_pos / TILE_SIZE;
        y2 = (new_y_pos + height_frame_ - 1) / TILE_SIZE;

        if (x1 >= 0 && x2 < MAP_WIDTH && y1 >= 0 && y2 < MAP_HEIGHT) {
            if (y_val_ > 0) { // Moving down
                if (mapData[y2][x1] == 0 || mapData[y2][x2] == 0) {
                    y_pos_ = y2 * TILE_SIZE;
                    y_pos_ -= height_frame_ + 1;
                    y_val_ = 0;
                    on_ground_ = true;
                    is_falling_ = false;
                }
            } else if (y_val_ < 0) { // Moving up
                if (mapData[y1][x1] == 0 || mapData[y1][x2] == 0) {
                    y_pos_ = (y1 + 1) * TILE_SIZE;
                    y_val_ = 0;
                }
            }
        }

        y_pos_ += y_val_;


        if (x_pos_ < 0) {
            x_pos_ = 0;
        } else if (x_pos_ + width_frame_ > SCREEN_WIDTH) {
            x_pos_ = SCREEN_WIDTH - width_frame_;
        }

        if (y_pos_ < 0) {
            y_pos_ = 0;
        } else if (y_pos_ + height_frame_ > SCREEN_HEIGHT) {
            y_pos_ = SCREEN_HEIGHT - height_frame_;
        }


        if (y_val_ > 0 && !on_ground_) {
            is_falling_ = true;
        }
    }

    SDL_Rect getRect() const {
        SDL_Rect rect;
        rect.x = x_pos_;
        rect.y = y_pos_;
        rect.w = width_frame_;
        rect.h = height_frame_;
        return rect;
    }

    void setPosition(float x, float y) {
        x_pos_ = x;
        y_pos_ = y;
    }

    void set_comeback_time (const int& cb_time) {come_back_time_ = cb_time;}
private:
    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[6];
    Input input_type_;
    int frame_;
    int status_;
    bool on_ground_;
    bool is_falling_;

    int come_back_time_;
};

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;

    if (bottomA <= topB) {
        return false;
    }
    if (topA >= bottomB) {
        return false;
    }
    if (rightA <= leftB) {
        return false;
    }
    if (leftA >= rightB) {
        return false;
    }

    return true;
}

void renderGameOver(SDL_Renderer* renderer, TTF_Font* font, int selectedOption) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (gameOverBackground != nullptr) {
        SDL_RenderCopy(renderer, gameOverBackground, nullptr, nullptr);
    }

    TextObject gameOverText;
    gameOverText.SetText("Game Over");
    gameOverText.SetColor(0, 0, 0);
    gameOverText.LoadFromRenderText(font, renderer);
    gameOverText.RenderText(renderer, SCREEN_WIDTH / 2 - gameOverText.GetWidth() / 2, SCREEN_HEIGHT / 2 - 250);
    gameOverText.Free();

    const std::string menuOptions[] = {"Replay", "Exit"};

    for (int i = 0; i < 2; ++i) {
        TextObject optionText;
        optionText.SetText(menuOptions[i]);

        if (i == selectedOption) {
            optionText.SetColor(255, 255, 0);
        } else {
            optionText.SetColor(255, 255, 255);
        }

        optionText.LoadFromRenderText(font, renderer);
        optionText.RenderText(renderer, SCREEN_WIDTH / 2 - optionText.GetWidth() / 2, SCREEN_HEIGHT / 2 + i * 40);
        optionText.Free();
    }

    SDL_RenderPresent(renderer);
}

void handleGameOverInput(bool& quit, bool& replay, int& selectedOption) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    selectedOption--;
                    if (selectedOption < 0) selectedOption = 1;
                    break;
                case SDLK_DOWN:
                    selectedOption++;
                    if (selectedOption > 1) selectedOption = 0;
                    break;
                case SDLK_RETURN:
                    if (selectedOption == 0) {
                        replay = true;
                    } else if (selectedOption == 1) {
                        quit = true;
                    }
                    break;
            }
        }
    }
}

bool initData() {
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret<0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");

    gWindow = SDL_CreateWindow("Death Jump", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (gWindow==NULL) {
        std::cout << "Failed to create window" << SDL_GetError() << std::endl;
        success = false;
    } else {
        gScreen = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
        if (gScreen==NULL) {
            std::cout << "Failed to create screen" << SDL_GetError() << std::endl;
            success = false;
        } else {
            SDL_SetRenderDrawColor(gScreen,0xFF,0xFF,0xFF,0xFF);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags)) {
                std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
                success = false;
            }
        }

        if (TTF_Init()==-1) {
            success=false;
        }

        font_time = TTF_OpenFont("font/fontpixel.ttf",20);
        if (font_time==NULL) {
            success=false;
        }
    }

    return success;
}

int main(int argc,char* argv[]) {
    ImpTimer fps_timer;
    if (!initData()) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

   if (!loadBackground("image/9.png")) {
        std::cout << "Failed to load background image!" << std::endl;
        close();
        return -1;
    }

    if (!loadTileset("map/93.png")) {
        std::cout << "Failed to load tileset image!" << std::endl;
        close();
        return -1;
    }

     if (!loadGameOverBackground("image/9.png")) {
        std::cout << "Failed to load game over background image!" << std::endl;
        close();
        return -1;
    }

    Player p_player;
    if (!p_player.loadIMG("image/player_right.png", gScreen)) {
        std::cout << "Failed to load player image!" << std::endl;
        close();
        return -1;
    }

    if (!loadMenuBackground("image/9.png")) {
        std::cout << "Failed to load menu background image!" << std::endl;
        close();
        return -1;
    }

    p_player.set_clips();

    PlayerPower Player_power;
    Player_power.Init(gScreen);

    Dangerous obstacles[7];
    for (int i = 0; i < 7; ++i) {
        obstacles[i].loadIMG("image/obstacle.png", gScreen);
        obstacles[i].setPosition(rand() % (SCREEN_WIDTH - 100), rand() % (SCREEN_HEIGHT - 100));
        obstacles[i].setSpeed(3);
    }

    int collisionCount = 0;
    const int maxCollisions = 3;

    TextObject time_game;
    time_game.SetColor(TextObject::WHITE_TEXT);

    bool quit = false;
    bool startGame = false;
    int selectedOption = 0;

    while (!quit) {
        if (!startGame) {
            handleMenuInput(quit, selectedOption, startGame);

            SDL_SetRenderDrawColor(gScreen, 0, 0, 0, 255);
            SDL_RenderClear(gScreen);

            renderMenu(gScreen, font_time, selectedOption);

            SDL_RenderPresent(gScreen);
        } else {
            fps_timer.start();
            while (SDL_PollEvent(&gEvent) != 0) {
                if (gEvent.type == SDL_QUIT) {
                    quit = true;
                }

                p_player.HandleInputAction(gEvent,gScreen);

            }
            SDL_SetRenderDrawColor(gScreen,0Xff,0Xff,0Xff,0Xff);
            SDL_RenderClear(gScreen);

            p_player.DoPlayer();
            renderMap();

            p_player.ShowPlayer(gScreen);

            Player_power.Show(gScreen);

            for (int i = 0; i < 7; ++i) {
                obstacles[i].dangerousMove();
                obstacles[i].render(gScreen);

                if (checkCollision(p_player.getRect(), obstacles[i].GetRect())) {
                    collisionCount++;
                    if (collisionCount >= maxCollisions) {
                        bool gameOver = true;
                        bool replay = false;
                        int selectedGameOverOption = 0;
                        while (gameOver && !quit) {
                            handleGameOverInput(quit, replay, selectedGameOverOption);

                            renderGameOver(gScreen, font_time, selectedGameOverOption);

                            if (replay) {
                                collisionCount = 0;
                                startGame = false;
                                gameOver = false;
                                p_player.setPosition(0, 0);
                                Player_power.Init(gScreen);
                            }
                        }
                    } else {
                        p_player.setPosition(0, 0);
                        p_player.set_comeback_time(60);
                        SDL_Delay(500);
                        Player_power.Decrease();
                        Player_power.Render(gScreen);
                        for (int j = 0; j < 5; ++j) {
                            obstacles[j].setPosition(rand() % (SCREEN_WIDTH - 100), rand() % (SCREEN_HEIGHT - 100));
                        }
                    }
                }
            }

            SDL_RenderPresent(gScreen);

            int real_imp_time = fps_timer.get_ticks();
            int time_one_frame = 1000/FRAME_PER_SECOND;

            if (real_imp_time<time_one_frame) {
                int delay_time = time_one_frame - real_imp_time;
                if (delay_time>=0) {
                    SDL_Delay(delay_time);
                }
            }
        }
    }

    close();
    return 0;
}
