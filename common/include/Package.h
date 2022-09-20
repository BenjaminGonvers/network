#pragma once
#include <SFML/Network.hpp>

class Package
{
private:

public:

	enum PackageType
	{
		message,
		move,
		connect,
		firstConnect
	};
	enum PlayerNumber
	{
		Player1,
		Player2
	};

	PackageType _type;
	PlayerNumber _player;

	Package() = default;
	Package(PackageType, PlayerNumber);

	
};

inline sf::Packet& operator <<(sf::Packet& packet, const Package package)
{
	const auto type = static_cast<int>(package._type);
	const auto player = static_cast<int>(package._player);
	return packet << type << player;
}

inline sf::Packet& operator >>(sf::Packet& packet, Package& package)
{
	int type = 0;
	packet >> type;
	package._type = static_cast<Package::PackageType>(type);
	int player = 0;
	packet >> player;
	package._player = static_cast<Package::PlayerNumber>(type);
	return packet;
}

inline Package::Package(PackageType type,PlayerNumber player)
{
	_type = type;
	_player = player;
}