#ifndef PLAYER_POWER_H_
#define PLAYER_POWER_H_

#include "BaseFunction.h"
#include "LoadPicture.h"

class PlayerPower : public LoadPicture {
public:
    PlayerPower();
    ~PlayerPower();

    void SetNum(const int& num) {number_ = num;}
    void AddPos(const int& xPos);
    void Show(SDL_Renderer* screen);
    void Init(SDL_Renderer* screen);
    void InitCrease();
    void Decrease();
private:
    int number_;
    std::vector<int> pos_list_;
};
#endif // PLAYER_POWER_H_
