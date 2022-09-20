#include <iostream>
#include <server.h>
#include <Package.h>
#include <SFML/Network.hpp>


Server::Server()
{ 
	// bind the listener to the port 
	if (_listener.listen(58008) != sf::Socket::Done)
	{
		// error...
	}

}


void Server::run()
{
	std::cout << "waiting p1\n";
	waitClientP1();
	std::cout << "p1 connected\n";
	std::cout << "waiting p2\n";
	waitClientP2();
	std::cout << "p2 connected\n";
	std::cout << "start game\n";

	sendPlayerStart();
	ConfirmePlayerStart();
	std::cout << "game started\n";

	while (true)
	{
		waitToRecieve();
	}

}

void Server::waitClientP1()
{
	if (_listener.accept(clientP1) != sf::Socket::Done)
	{
		// error...
	}
	_selector.add(clientP1);
}

void Server::waitClientP2()
{
	if (_listener.accept(clientP2) != sf::Socket::Done)
	{
		// error...
	}
	_selector.add(clientP2);
}

void Server::sendPlayerStart()
{
	Package packageP1(Package::PackageType::firstConnect,Package::PlayerNumber::Player1);
	sf::Packet packetP1;
	packetP1 << &packageP1;
	clientP1.send(packetP1);

	Package packageP2(Package::PackageType::firstConnect, Package::PlayerNumber::Player2);
	sf::Packet packetP2;
	packetP2 << &packageP2;
	clientP2.send(packetP2);

}

void Server::ConfirmePlayerStart()
{
	bool player1Confirm = false;
	bool player2Confirm = false;

	while (!player1Confirm || !player2Confirm)
	{
		if (_selector.wait(sf::seconds(10.0f))) {
			if (_selector.isReady(clientP1)&& !player1Confirm)
			{
				std::cout << "receive client p1\n";
				sf::Packet packet;
				clientP1.receive(packet);
				std::cout << "send client p2\n";
				clientP2.send(packet);
				std::cout << "send client succesfull p2\n";
				packet.clear();
				player1Confirm = true;
			}
			if (_selector.isReady(clientP2) && !player2Confirm)
			{
				std::cout << "receive client p2\n";
				sf::Packet packet;
				clientP2.receive(packet);
				std::cout << "send client p1\n";
				clientP1.send(packet);
				std::cout << "send client succesfull p1\n";
				packet.clear();
				player2Confirm = true;
			}

		}
	}
}


void Server::waitToRecieve()
{

	if (_selector.wait()) {
		if (_selector.isReady(clientP1))
		{
			sf::Packet packet;
			clientP1.receive(packet);
			clientP2.send(packet);
			packet.clear();
		}
		if (_selector.isReady(clientP2))
		{
			sf::Packet packet;
			clientP2.receive(packet);
			clientP1.send(packet);
			packet.clear();
		}

	}
}
