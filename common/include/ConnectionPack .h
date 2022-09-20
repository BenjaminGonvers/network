#pragma once
#include <Package.h>
#include <string>

class ConnectionPack : public Package
{
public:
	std::string _playerName;
	ConnectionPack() = default;
	ConnectionPack(PlayerNumber,std::string);
	~ConnectionPack();
private:


};

inline sf::Packet& operator <<(sf::Packet& packet, const ConnectionPack package)
{
	const auto type = static_cast<int>(package._type);
	const auto player = static_cast<int>(package._player);
	return packet << type << player << package._playerName;
}

inline sf::Packet& operator >>(sf::Packet& packet, ConnectionPack& package)
{
	int type = 0;
	packet >> type;
	package._type = static_cast<Package::PackageType>(type);
	packet >> type;
	package._player = static_cast<Package::PlayerNumber>(type);
	packet >> package._playerName;
	return packet;
}

ConnectionPack::ConnectionPack(PlayerNumber player,std::string playerName )
{
	_type = connect;
	_player = player;
	_playerName = playerName;
}

ConnectionPack::~ConnectionPack()
{

}