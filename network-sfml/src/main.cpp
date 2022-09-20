#include <format>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "Const.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    bool IsConnected = false;
    bool ConnectPending = false;
    bool ConnectFail = false;
    float TimeTryConnecting = 0;
    float deltaTime = 0;

    char NameInputBuffer[SENDER_BUFFER_SIZE]{};
    char portInputBuffer[PORT_BUFFER_SIZE] = "8008";
    char ipInputBuffer[IP_BUFFER_SIZE] = "localhost";

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowDemoWindow();


        if (IsConnected) {
            ImGui::Begin("Combat Log");
            ImGui::Text("Hello {} !");
            ImGui::Button("Look at this pretty button");
            ImGui::End();
        }
        if (!IsConnected && ConnectPending)
        {
            TimeTryConnecting += deltaTime;
            ImGui::Begin("Connecting");
            ImGui::Text("Waiting for the link");
        	ImGui::End();
            if(TimeTryConnecting > 10)
            {
                ConnectFail = true;
                ConnectPending = false;
            }
        }
        if(!IsConnected && !ConnectPending)
        {
            ImGui::Begin("Name and Ip");
            ImGui::Text("Hello!");
            ImGui::Text("Can you tell me your name?");
            ImGui::InputTextWithHint(
                "Name", 
                "Your name",
                NameInputBuffer,
                MESSAGE_BUFFER_SIZE,
                ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::Text("Tell me the ip where we need to connect");
            ImGui::InputTextWithHint(
                "IP",
                "Ex: 172.16.1.199",
                ipInputBuffer,
                IP_BUFFER_SIZE,
                ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::InputText(
                "Ports",
                portInputBuffer,
                PORT_BUFFER_SIZE,
                ImGuiInputTextFlags_EnterReturnsTrue);
            if(ImGui::Button("Next"))
            {
                ConnectPending = true;
                TimeTryConnecting = 0;
            }
            ImGui::End();
        }
        if (ConnectFail)
        {
            ImGui::Begin("Connection fail");
            ImGui::Text("We have fail to connect to the server");
            ImGui::Text("Please check the ip and retry later");
            if (ImGui::Button("OK"))
            {
                ConnectFail = false;
            }
            ImGui::End();
        }

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
        deltaTime = deltaClock.getElapsedTime().asSeconds();
    }

    ImGui::SFML::Shutdown();

    return EXIT_SUCCESS;
}