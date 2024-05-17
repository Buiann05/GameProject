#ifndef LOAD_PICTURE_H_
#define LOAD_PICTURE_H_

#include "BaseFunction.h"

class LoadPicture {
public:
    LoadPicture();
    ~LoadPicture();
    void SetRect(const int& x, const int& y) {rect_.x = x, rect_.y = y;}
    SDL_Rect GetRect() const {return rect_;}
    SDL_Texture* GetObject() const {return p_object_;}

    virtual bool loadIMG(std::string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();
    int getWidth() const {return width_;}
    int getHeight() const {return height_;}
protected:
    SDL_Texture* p_object_;
    SDL_Rect rect_;
    int width_;
    int height_;
};

#endif // LOAD_PICTURE_H_
