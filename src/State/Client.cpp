#include "State/Client.hpp"

Client::Client(sf::RenderWindow& window,
			   std::unique_ptr<State>& main_state)
	: State(window, main_state)
{
}

void Client::updateFrame()
{
	if (mWasSendPressed)
	{
		//Create the socket.
		sf::TcpSocket socket;
		//Try to connect.
		if (socket.connect(mIp, mPort) != sf::Socket::Done)
		{
			mLastReceived = "Error: Could not connect to server.";
			return;
		}

		//Try to send data.
		if (socket.send(mToSend, std::strlen(mToSend)) != sf::Socket::Done)
		{
			mLastReceived = "Error: Could not send data!";
			return;
		}

		//Try to receieve the result.
		char res[100];
		size_t rec;
		if (socket.receive((void*)res, 100, rec) != sf::Socket::Done)
		{
			mLastReceived = "Error: Could not recieve result!";
			return;
		}

		//Set the result.
		mLastReceived = std::string(res);
	}
}

void Client::drawSFML()
{
	//nothing...
}

void Client::drawImGui()
{
	ImGui::SetNextWindowSize(window().getSize());
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("ScriptableTCP Client",
				 nullptr,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	//Ip & Port choice.
	ImGui::Text("IP Address:");
	ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##ip", mIp, 20);
	ImGui::SameLine();
	ImGui::Text("Port:");
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputInt("##port", &mPort, 0, 0);

	//Input to send to the server.
	ImGui::Text("String to send: ");
	ImGui::SameLine();
	ImGui::PushItemWidth(425);
	ImGui::InputText("##data", mToSend, 150);
	mWasSendPressed = ImGui::Button("<> Send");

	ImGui::Text("Output: %s", mLastReceived.c_str());

	ImGui::End();
}