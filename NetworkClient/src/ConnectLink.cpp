#include <ConnectLink.h>

#include "Const.h"
#include "imgui.h"

bool ConnectLink::connect()
{
    if (!_isConnected) {
		ImGui::Begin("Name and Ip");
        if (!_connectPending && !_connectFail)
        {
            
            ImGui::Text("Hello!");
            ImGui::Text("Can you tell me your name?");
            ImGui::InputTextWithHint(
                "Name",
                "Your name",
                nameInputBuffer,
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
            if (ImGui::Button("Next"))
            {
                _connectPending = true;
            }
        }
        if (_connectPending && !_connectFail)
        {
        	ImGui::Text("Waiting for connection");
            const unsigned short inputPort = static_cast<unsigned short>(std::stoi(portInputBuffer));
            sf::Socket::Status status = _socket.connect(ipInputBuffer, inputPort);
            if (status == sf::Socket::Disconnected || status == sf::Socket::Error)
            {
                _connectFail = true;
                _connectPending = false;

            }

            if(status == sf::Socket::Done || status == sf::Socket::NotReady)
            {
                _isConnected = true;
                _connectPending = false;
                ImGui::End();
                return true;
            }



        }

        if(_connectFail)
        {
            ImGui::Text("The connection as fail");
            ImGui::Text("Check the ip address or the ports and retry later");
            if (ImGui::Button("OK"))
            {
                _connectFail = false;
            }
        }
        ImGui::End();
	return false;
    }
    return true;
}