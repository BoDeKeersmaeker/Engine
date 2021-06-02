#pragma once
struct Color
{
	float R{ 255.f };
	float G{ 255.f };
	float B{ 255.f };
};

struct Float2
{
	float x{ 0 };
	float y{ 0 };
};

enum class MoveDirection
{
	TOPLEFT = 0,
	TOPRIGHT = 1,
	BOTTOMRIGHT = 2,
	BOTTOMLEFT = 3,
};
