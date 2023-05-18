#include "FlyingEnemy.h"

FlyingEnemy::FlyingEnemy() : ThreatObject() {
    // Initialize specific properties of FlyingEnemy
    set_x_val(5); // This will make the enemy move from left to right
}

FlyingEnemy::~FlyingEnemy() {
    for (int i = 0; i < bomb_list_.size(); i++) {
        delete bomb_list_[i];
    }
}

void FlyingEnemy::HandleMove(const int& x_border, const int& y_border) {
    float x_pos = get_x_pos();
    set_x_pos(x_pos + get_x_val());
    if (get_x_pos() > x_border) {
        set_x_pos(0); // Restart from left when it reaches the right border
    }
}

void FlyingEnemy::Show(SDL_Renderer* des) {
    // Draw the enemy and its bombs
    ThreatObject::Show(des);
    for (auto& bomb : bomb_list_) {
        if (bomb->is_move) {
            bomb->Render(des);
        }
    }
}

FlyingEnemy::Bomb::Bomb() {
    x = 0;
    y = 0;
    is_move = false;
    bomb_surface = nullptr;
    bomb_texture = nullptr;
}

FlyingEnemy::Bomb::~Bomb() {
    Free();
}

void FlyingEnemy::Bomb::HandleMove(const int& x_border, const int& y_border) {
    y += 5; // Bombs fall downwards
    if (y > y_border) {
        is_move = false; // Stop moving when it hits the ground
    }
}

void FlyingEnemy::Bomb::Render(SDL_Renderer* des) {
    // Code to render bomb
}

void FlyingEnemy::Bomb::Free() {
    if (bomb_texture != nullptr) {
        SDL_DestroyTexture(bomb_texture);
        bomb_texture = nullptr;
        x = 0;
        y = 0;
    }
}

void FlyingEnemy::DropBomb(SDL_Renderer* des) {
    // Drop a bomb when over a certain position
    if (get_x_pos() > 200 && get_x_pos() < 300) {
        Bomb* bomb = new Bomb();
        bomb->x = get_x_pos();
        bomb->y = get_y_pos();
        bomb->is_move = true;
        bomb_list_.push_back(bomb);
    }

    // Move bombs
    for (auto& bomb : bomb_list_) {
        if (bomb->is_move) {
            bomb->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
        }
    }
}
