#include <player.hpp>

Player::Player(int x, int y, int spd, int acc){
    this->x = x;
    this->y = y;
    this->spd = spd;
    this->acc = acc;
}

void Player::setPosition(int x, int y){
    this->x = x;
    this->y = y;
    std::cout << this->x << " || " << this->y << std::endl;
}

std::tuple<int, int> Player::getPosition(){
    return  {x,y};
}