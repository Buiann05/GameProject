#include "Dangerous.h"


Dangerous::Dangerous() : x_pos_(0), y_pos_(0), speed_(0) {}

Dangerous::~Dangerous() {
    Free();
}

void Dangerous::dangerousMove() {
    x_pos_ += speed_;
    if (x_pos_ > SCREEN_WIDTH) {
        x_pos_ = -rect_.w;
    }
}

void Dangerous::render(SDL_Renderer* screen) {
    rect_.x = x_pos_;
    rect_.y = y_pos_;
    SDL_RenderCopy(screen, p_object_, nullptr, &rect_);
}

void Dangerous::setPosition(int x, int y) {
    x_pos_ = x;
    y_pos_ = y;
}

void Dangerous::setSpeed(int speed) {
    speed_ = speed;
}

