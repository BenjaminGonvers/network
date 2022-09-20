#pragma once
#include <SFML/Network.hpp>
#include <ConnectLink.h>
#include <SFML/Network/TcpSocket.hpp>
#include <Player.h>
#include <Package.h>
#include <ConnectionPack .h>

#include "imgui-SFML.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"

class Game
{
private:
	bool gameON = true;

public:
	Game();

	/**
	 * \brief loop where the game work.
	 * \return return true if a problem occur or false if it a normal exit(like the client close it manually)
	 */
	bool gameLoop();

	sf::TcpSocket _socket;
	ConnectLink _link;

	Player player;
	Player Adversary;
	Package::PlayerNumber _playerNumber;

	bool alreadyConfirm = false;
	bool serverConfirm = false;
	bool playerConfirm = false;
	bool haveTakeName = false;

	bool playerHaveAction = false;
	Player::ActionType playerAction = Player::nothing;
};


inline Game::Game()
	:_link(_socket)
{
	_socket.setBlocking(false);
}
inline bool Game::gameLoop()
{

	sf::RenderWindow window(sf::VideoMode(640, 480), "Network-SFML");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	sf::Clock deltaClock;
	
	sf::Packet packet;

	while (window.isOpen()) {
		sf::Event event;
		
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed) {
					window.close();
			}
			
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		if(_link.connect())
		{
			if(!haveTakeName)
			{
				player._name = _link.takeName();
				haveTakeName = true;
			}
			if(!alreadyConfirm)
			{
				if (!serverConfirm)
				{
					if (_socket.receive(packet) == sf::Socket::Done)
					{
						Package package;
						packet >> package;
						_playerNumber = package._player;
						serverConfirm = true;

						ConnectionPack ConPackage;
						ConPackage._player = _playerNumber;
						ConPackage._playerName = player._name;
						ConPackage._type = ConnectionPack::connect;
						packet << ConPackage;

					}

				}else
				{
					if (!playerConfirm) {
						if (_playerNumber == Package::Player1) {
							if (_socket.send(packet) == sf::Socket::Done)
							{
								playerConfirm = true;
							}
						}
						else
						{
							if (_socket.receive(packet) == sf::Socket::Done)
							{
								ConnectionPack package;
								packet >> package;
								Adversary._name = package._playerName;

								ConnectionPack ConPackage;
								ConPackage._player = _playerNumber;
								ConPackage._playerName = player._name;
								ConPackage._type = ConnectionPack::connect;
								packet << ConPackage;

								playerConfirm = true;
							}
						}
					}
					else
					{
						if (_playerNumber != Package::Player1) {
							if (_socket.send(packet) == sf::Socket::Done)
							{
								alreadyConfirm = true;
							}
						}
						else
						{
							if (_socket.receive(packet) == sf::Socket::Done)
							{
								ConnectionPack package;
								packet >> package;
								Adversary._name = package._playerName;
								alreadyConfirm = true;
							}
						}
					}
				}
			}

			if(!playerHaveAction && alreadyConfirm)
			{
				ImGui::TextWrapped(player._name.data());
				ImGui::Text(std::to_string(player.PV).data());
				ImGui::TextWrapped(Adversary._name.data());
				ImGui::Text(std::to_string(Adversary.PV).data());

				playerAction = player.printAction();
				if (playerAction != Player::nothing)
				{
					playerHaveAction = true;
				}

			}
			
		}

		window.clear();
		ImGui::SFML::Render(window);
		window.display();
		
	}
	
	ImGui::SFML::Shutdown();

	return EXIT_SUCCESS;
}

