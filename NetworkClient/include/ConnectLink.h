#pragma once
#include <SFML/Network.hpp>

#include "Const.h"


class ConnectLink
{
private:

	bool _isConnected = false;
	bool _connectPending = false;
	bool _connectFail = false;

	char nameInputBuffer[SENDER_BUFFER_SIZE]{};
	char portInputBuffer[PORT_BUFFER_SIZE] = "58008";
	char ipInputBuffer[IP_BUFFER_SIZE] = "localhost";

public:

	ConnectLink(sf::TcpSocket& socket);
	std::string takeName();

	/**
	 * \brief Starts the connection to the server, and control it if already started.
	 * \return Returns true if the connection is good, otherwise, false.
	 */
	bool connect();

private:
	sf::TcpSocket& _socket;

};

 
inline ConnectLink::ConnectLink(sf::TcpSocket& socket)
	:_socket(socket)
{
	
}

inline std::string ConnectLink::takeName()
{
	return nameInputBuffer;
}