#include "LoadPicture.h"

LoadPicture::LoadPicture() {
    p_picture_ = nullptr;
    rect_.x=0;
    rect_.y=0;
    rect_.w=0;
    rect_.h=0;
}

~LoadPicture::LoadPicture() {

}

bool LoadPicture::loadImg(std::string path, SDL_Renderer* screen) {
    SDl_Texture* new_texture = nullptr;
    SDl_Surface* load_surface = IMG_Load(path.c_str());
    if (load_surface != nullptr) {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);\
        if (new_texture != nullptr) {
            rect_.w=load_surface->w;
            rect_.h=load_surface->h;
        }
    }
}
