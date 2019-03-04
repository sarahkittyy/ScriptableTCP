#include "State/HostInfo.hpp"

HostInfo::HostInfo(sf::RenderWindow& window,
				   std::unique_ptr<State>& main_state)
	: State(window, main_state)
{
}

void HostInfo::updateFrame()
{
	if (wasHostPressed)
	{
		//Switch to the actual hosting interface.
		state().reset(new Host(window(), state(), (unsigned short)port));
	}
	else if (wasBackPressed)
	{
		//Switch back to the main menu.
		state().reset(new MainMenu(window(), state()));
	}
}

void HostInfo::drawSFML()
{
}

void HostInfo::drawImGui()
{
	//Display hosting info.
	ImGui::SetNextWindowSize(window().getSize());
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Hosting",
				 nullptr,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	//Prompt for the port.
	ImGui::Text("Port:");
	ImGui::InputInt("", &port, 0, 0);

	ImGui::NewLine();

	//Host button.
	wasHostPressed = ImGui::Button("Host!");

	ImGui::End();
}