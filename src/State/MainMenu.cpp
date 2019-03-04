#include "State/MainMenu.hpp"

MainMenu::MainMenu(sf::RenderWindow& window,
				   std::unique_ptr<State>& main_state)
	: State(window, main_state)
{
}

void MainMenu::updateFrame()
{
	if (wasConnectPressed)
	{
		//Switch to the ConnectionInfo state.
	}
	else if (wasHostPressed)
	{
		//Switch to the HostInfo state.
		state().reset(new HostInfo(window(), state()));
	}
	else if (wasQuitPressed)
	{
		window().close();
	}
}

void MainMenu::drawSFML()
{
}

void MainMenu::drawImGui()
{
	ImGui::SetNextWindowSize(window().getSize());
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("ScriptableTCP",
				 nullptr,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	//Render the prompt text.
	ImGui::TextWrapped("Would you like to connect to a TCP socket, or host a script?");

	ImGui::NewLine();

	//Render the two buttons.
	wasConnectPressed = ImGui::Button("Connect");
	wasHostPressed	= ImGui::Button("Host");

	//Quit button.
	ImGui::NewLine();
	wasQuitPressed = ImGui::Button("Quit");

	ImGui::End();
}