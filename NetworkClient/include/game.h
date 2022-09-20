#pragma once
#include <SFML/Network.hpp>
#include <ConnectLink.h>
#include <SFML/Network/TcpSocket.hpp>
#include <Player.h>
#include <Package.h>
#include <ConnectionPack .h>
#include <iostream>

#include "imgui-SFML.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "MovePack.h"

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
	bool AdversaryHavePlayed = false;
	bool SendYourMove = false;

	Player::ActionType playerAction = Player::nothing;
	Player::ActionType AdversaryAction = Player::nothing;
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
	sf::Packet sendingPacket;
	sf::Packet incomingPacket;

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
						if(_playerNumber == Package::Player1)
						{
							std::cout << "je suis player1\n";
						}
						else if(_playerNumber == Package::Player2)
						{
							std::cout << "je suis player2\n";
						}else
						{
							std::cout << "packet de merde";
						}
						packet.clear();
						serverConfirm = true;

						if (_playerNumber == Package::Player1) {
							ConnectionPack ConPackage;
							ConPackage._player = _playerNumber;
							ConPackage._playerName = player._name;
							ConPackage._type = ConnectionPack::connect;
							packet << ConPackage;
						}

					}

				}else
				{
					if (!playerConfirm) {
						if (_playerNumber == Package::Player1)
						{
							if (_socket.send(packet) == sf::Socket::Done)
							{
								packet.clear();
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
								packet.clear();

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
								packet.clear();
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
								packet.clear();
								alreadyConfirm = true;
							}
						}
					}
				}
			}

			if (alreadyConfirm)
			{
				ImGui::TextWrapped(player._name.data());
				ImGui::Text(std::to_string(player.PV).data());
				ImGui::TextWrapped(Adversary._name.data());
				ImGui::Text(std::to_string(Adversary.PV).data());

				if(AdversaryHavePlayed)
				{
					ImGui::Text("your adversary have played");
				}

				if (!playerHaveAction)
				{

					playerAction = player.printAction();
					if (playerAction != Player::nothing)
					{
						playerHaveAction = true;

						MovePack package;
						package._player = _playerNumber;
						package._action = playerAction;

						sendingPacket << package;

					}

				}else
				{
					ImGui::Text("you have played");

					if(!SendYourMove)
					{
						if(_socket.send(sendingPacket) == sf::Socket::Done)
						{
							sendingPacket.clear();
							SendYourMove = true;
						}
					}
				}

				if(!AdversaryHavePlayed)
				{
					if(_socket.receive(incomingPacket)== sf::Socket::Done)
					{
						MovePack package;
						incomingPacket >> package;
						AdversaryAction = package._action;
						incomingPacket.clear();
						AdversaryHavePlayed = true;
					}
				}

				if(AdversaryHavePlayed  && SendYourMove)
				{
					switch (playerAction)
					{
					case Player::strike:
						if (AdversaryAction == Player::strike) {
							player.PV -= 25;
							Adversary.PV -= 25;
						}

						if (AdversaryAction == Player::block)
							Adversary.PV -= 10;
						
						if (AdversaryAction == Player::counterStrike)
							player.PV -= 25;
						break;
					case Player::block:
						if (AdversaryAction == Player::strike)
							player.PV -= 10;
						if (AdversaryAction == Player::counterStrike) {
							Adversary.PV -= 25;
							player.PV -= 10;
						}
						break;
					case Player::counterStrike:
						if (AdversaryAction == Player::strike)
							Adversary.PV -= 25;

						if (AdversaryAction == Player::block) {
							player.PV -= 25;
							Adversary.PV -= 10;
						}

						if (AdversaryAction == Player::counterStrike) {
							Adversary.PV -= 10;
							player.PV -= 10;
						}
					}

					AdversaryHavePlayed = false;
					playerHaveAction = false;
					SendYourMove = false;
					playerAction = Player::nothing;
					AdversaryAction = Player::nothing;


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

