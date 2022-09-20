#pragma once
#include <Package.h>
#include <Player.h>


class MovePack : public Package
{
public:
	
	MovePack();

	Player::ActionType _action;

private:


};

inline MovePack::MovePack()
{
	_type = Package::move;
}

inline sf::Packet& operator <<(sf::Packet& packet, const MovePack package)
{
	const auto type = static_cast<int>(package._type);
	const auto player = static_cast<int>(package._player);
	const auto action = static_cast<int>(package._action);
	return packet << type << player << action;
}

inline sf::Packet& operator >>(sf::Packet& packet, MovePack& package)
{
	int type = 0;
	packet >> type;
	package._type = static_cast<Package::PackageType>(type);
	int player = 0;
	packet >> player;
	package._player = static_cast<Package::PlayerNumber>(type);
	int action = 0;
	packet >> action;
	package._action = static_cast<Player::ActionType>(action);

	return packet;
}
