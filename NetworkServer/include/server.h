#pragma once
#include <SFML/Network.hpp>

class Server
{
private:

	sf::TcpListener _listener;
	sf::SocketSelector _selector;

	sf::TcpSocket clientP1;
	sf::TcpSocket clientP2;
	
	void waitClientP1();
	void waitClientP2();

	void waitToRecieve();
	void sendPlayerStart();
	void ConfirmePlayerStart();


public:

	Server();
	void run();

};
