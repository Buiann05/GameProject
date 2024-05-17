#ifndef DANGEROUS_H_
#define DANGEROUS_H_

#include "BaseFunction.h"
#include "LoadPicture.h"

class Dangerous : public LoadPicture {
public:
    Dangerous();
    ~Dangerous();

    void dangerousMove();
    void render(SDL_Renderer* screen);
    void setPosition(int x, int y);
    void setSpeed(int speed);
private:
    int x_pos_;
    int y_pos_;
    int speed_;
};
#endif // DANGEROUS_H_
