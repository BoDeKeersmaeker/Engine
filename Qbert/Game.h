#pragma once
#include <Minigin.h>

class Game final : public engine::Minigin
{
protected:
	virtual void LoadGame() const override;

private:
};