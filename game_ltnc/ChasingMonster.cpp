#include "ChasingMonster.h"
#include "MainObject.h"
#include "GameMap.h"
#include "CommonFunc.h"

ChasingMonster::ChasingMonster() {
	width_frame_ = 0;
	height_frame_ = 0;
	speed_ = 0;
	last_move_time_ = 0;
	frame_ = 0;
}

ChasingMonster::~ChasingMonster() {}

void ChasingMonster::SetSpeed(int speed) {
    speed_ = speed;
}
void ChasingMonster::Move(MainObject& player, const GameMap& game_map) {
	const int chasing_monster_speed = 2; // Set the speed of the chasing monster

	// Calculate the distance between the monster and the player
	int diff_x = player.GetRect().x - rect_.x;
	int diff_y = player.GetRect().y - rect_.y;

	// Calculate the normalized direction vector towards the player
	float distance = std::sqrt(diff_x * diff_x + diff_y * diff_y);
	float direction_x = diff_x / distance;
	float direction_y = diff_y / distance;
	// Move the monster in the direction of the player with the constant speed
	rect_.x += static_cast<int>(direction_x * chasing_monster_speed);
	rect_.y += static_cast<int>(direction_y * chasing_monster_speed);
}



void ChasingMonster::Show(SDL_Renderer* des) {
	const int FRAME_DELAY = 100; // Delay in milliseconds between each frame
	static Uint32 frame_start_time = SDL_GetTicks();
	Uint32 current_time = SDL_GetTicks();

	if (current_time - frame_start_time >= FRAME_DELAY) {
		frame_++;
		if (frame_ >= NUM_FRAME_MONSTER) {
			frame_ = 0;
		}

		frame_start_time = current_time;
	}

	SDL_Rect* currentClip = &frame_clip_[frame_];
	SDL_Rect rendQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
	SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
}
bool ChasingMonster::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);

	if (ret == true)
	{
		width_frame_ = rect_.w / NUM_FRAME_MONSTER;
		height_frame_ = rect_.h;
	}
	return ret;
}
SDL_Rect ChasingMonster::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;

	rect.w = width_frame_;
	rect.h = height_frame_;
	return rect;
}
void ChasingMonster::set_clips() {
	if (width_frame_ > 0 && height_frame_ > 0) {
		for (int i = 0; i < NUM_FRAME_MONSTER; i++) {
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}