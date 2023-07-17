#ifndef FCPP_ADVMATH_H
#define FCPP_ADVMATH_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cassert>
#include <random>

inline int random(const int range = INT32_MAX, const int start = 0)
{
	std::random_device rd;
	std::uniform_int_distribution<int>dist(start, range);
	int val(dist(rd));
	return val;
}

inline float randomF(float range = INT32_MAX, float start = 0.0)
{
	std::random_device rd;
	std::uniform_real_distribution<double>dist(start, range);
	float val(dist(rd));
	return val;
}

/// <summary>
/// converts a 360 degree angle to equal number of radians(IE 180 = PI)
/// </summary>
/// <param name="deg">the angle to convert</param>
/// <returns>the input angle output in radians</returns>
inline const float& Deg2Rad(const float& deg)
{
	float val(deg * M_PI / 180);
	return val;
}

inline const float& Cos(const float& ang) { const float val(cos(Deg2Rad(ang))); return val; }
inline const float& Sin(const float& ang) { const float val(sin(Deg2Rad(ang))); return val; }
inline const float& Tan(const float& ang) { const float val(tan(Deg2Rad(ang))); return val; }


inline const bool isEven(int& val)
{
	return ((val % 2) == 0);
}

inline const bool& TakeChance(int chance)
{
	int x(random(INT16_MAX));
	bool r((x % chance) == 0);
	return r;
}

inline const float& InvertF(float& value)
{
	value = -value;
	return value;
}

inline const float& Percent(float num, float tot)
{
	float r((num / tot) * 100);
	return r;
}

inline const int& SDL_GetDistanceX(const int& p1, const int& p2)
{
	int num(pow(p2 - p1, 2));
	int val(sqrt(num));
	return val;
}

inline const float& SDL_GetDistanceXf(float& p1, float& p2)
{
	float num(pow(p2 - p1, 2));
	float val(sqrt(num));
	return val;
}

#define SQX (p2.x - p1.x) * (p2.x - p1.x) //pow(p2.x - p1.x, 2)
#define SQY (p2.y - p1.y) * (p2.y - p1.y) //pow(p2.y - p1.y, 2)
inline const int& SDL_GetDistance(const SDL_Point& p1, const SDL_Point& p2)
{
	int val(sqrt(SQX + SQY));
	return val;
}

inline const float& SDL_GetDistanceF(const SDL_FPoint& p1, const SDL_FPoint& p2)
{
	float _x(SQX), _y(SQY);
	float val = sqrtf(_x + _y);
	return val;
}

inline const SDL_Point& AddPoints(SDL_Point& p1, SDL_Point& p2)
{
	SDL_Point R{ 0, 0 };
	R.x += p1.x + p2.x;
	R.y += p1.y + p2.y;
	return R;
}

inline const SDL_FPoint& AddFPoints(SDL_FPoint& p1, SDL_FPoint& p2)
{
	SDL_FPoint R{ 0, 0 };
	R.x += p1.x + p2.x;
	R.y += p1.y + p2.y;
	return R;
}

inline const SDL_Point& SDL_MinPoint(SDL_Point& p1, SDL_Point& p2)
{
	SDL_Point r{};
	r.x = p1.x < p2.x ? p1.x : p2.x;
	r.y = p1.y < p2.y ? p1.y : p2.y;
	return r;
}

const bool& SDL_PointEquals(const SDL_Point& p1, const SDL_Point& p2)
{
	bool _x(p1.x == p2.x);
	bool _y(p1.y == p2.y);
	bool val(_x && _y);
	return val;
}

const bool& SDL_FPointEquals(SDL_FPoint& fp1, SDL_FPoint& fp2)
{
	bool _x(fp1.x == fp2.x);
	bool _y(fp1.y == fp2.y);
	bool val(_x && _y);
	return val;
}

#endif