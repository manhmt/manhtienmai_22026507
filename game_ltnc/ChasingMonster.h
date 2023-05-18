#ifndef CHASING_MONSTER_H_
#define CHASING_MONSTER_H_

#include "BaseObject.h"
#include "CommonFunc.h"
#include "MainObject.h"
#include "GameMap.h"

#define NUM_FRAME_MONSTER 6

class ChasingMonster : public BaseObject {
public:
    ChasingMonster();
    ~ChasingMonster();

    void Move(MainObject& player, const GameMap& game_map);
    void SetSpeed(int speed);
    void Show(SDL_Renderer* des);
    void set_clips();
    bool LoadImg(std::string path, SDL_Renderer* screen);
    SDL_Rect GetRectFrame();


private:
    int width_frame_;
    int height_frame_;
    int speed_;
    int last_move_time_;
    SDL_Rect frame_clip_[NUM_FRAME_MONSTER] = {};
    int frame_;


};


#endif // CHASING_MONSTER_H_
