#pragma once
#include <Minigin.h>

class Game final : public engine::Minigin
{
public:
	Game() = default;

private:
	void LoadGame() const override;
};