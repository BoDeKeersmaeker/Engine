#pragma once
struct Color
{
	float R{ 255.f };
	float G{ 255.f };
	float B{ 255.f };
};

enum class ComponentType
{
	invalid,
	Texture,
	Text,
	FPS
};