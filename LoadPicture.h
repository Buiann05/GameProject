#ifndef LOAD_PICTURE_H_
#define LOAD_PICTURE_H_

#include "BaseFunction.h"

class LoadPicture{
public:
    LoadPicture();
    ~LoadPicture();
    void SetRect(const int& x, const int& y) {rect_.x=x, rect_.y=y;}
    SDL_Rect GetRect() const{return rect_;}
    SDL_Texture* GetPicture() const{return p_picture_;}

    bool loadImg(std::string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* des, const SDL_Rect* clip = nullptr);
protected:
    SDL_Texture* p_picture_;
    SDL_Rect rect_;
};

#endif // LOAD_PICTURE_H_
