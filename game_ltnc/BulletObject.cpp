#include "BulletObject.h"

BulletObject::BulletObject()
{
	x_val_ = BULLET_SPEED;
	y_val_ = BULLET_SPEED;
	is_move_ = false;
	bullet_type_ = SPHERE_BULLET;
	bullet_dir_ = 0; //chua chac
}
BulletObject::~BulletObject()
{

}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
	bool ret = false;
	if (bullet_type_ == LASER_BULLET)
	{
		ret = LoadImg("img//laser_bullet.png", des);
	}
	else
	{
		ret = LoadImg("img//sphere_bullet.png", des);
	}
	return ret;
}
void BulletObject::HandleMove(const int& x_border, const int& y_border)
{
	if (bullet_dir_ == DIR_RIGHT)
	{
		rect_.x += x_val_;
		if (rect_.x > x_border)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP)
	{
		rect_.y -= y_val_;
		if (rect_.y < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_LEFT)
	{
		rect_.x -= x_val_;
		rect_.y -= y_val_;
		if (rect_.y < 0 || rect_.x < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_RIGHT)
	{
		rect_.x += x_val_;
		rect_.y -= y_val_;
		if (rect_.y < 0 || rect_.x > x_border)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_DOWN_LEFT)
	{
		rect_.x -= x_val_;
		rect_.y += y_val_;
		if (rect_.y > y_border || rect_.x < 0)
		{
			is_move_ = false;
		}
	}

}