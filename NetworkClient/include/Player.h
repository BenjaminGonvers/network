#pragma once
#include <string>
#include <imgui.h>

class Player
{
public:
	std::string _name;
	int PV = 100;

	Player() = default;
	Player(std::string);
	~Player();

	enum ActionType
	{
		nothing,
		strike,
		block,
		cunterStrike,
		avoid
	};

	ActionType printAction();
private:

};

inline Player::Player(std::string name)
{
	_name = name;
}

inline Player::~Player()
{
}


inline Player::ActionType Player::printAction()
{
	if(ImGui::Button("Strike"))
	{
		return strike;
	}

	if(ImGui::Button("Block"))
	{
		return block;
	}

	if(ImGui::Button("CunterStrike"))
	{
		return cunterStrike;
	}

	if(ImGui::Button("Avoid"))
	{
		return avoid;
	}

	return nothing;
}