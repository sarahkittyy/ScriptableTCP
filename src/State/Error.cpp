#include "State/Error.hpp"

Error::Error(sf::RenderWindow& window,
			 std::unique_ptr<State>& main_state,
			 std::string error_msg)
	: State(window, main_state)
{
	mErrorMessage = error_msg;
}

void Error::updateFrame()
{
	if (wasMenuPressed)
	{
		//Reset to the main menu state.
		state().reset(new MainMenu(window(), state()));
	}
}

void Error::drawSFML()
{
}

void Error::drawImGui()
{
	//Draw the main window.
	ImGui::SetNextWindowSize(window().getSize());
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("An error has occured.",
				 nullptr,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	//Draw the error message.
	ImGui::TextWrapped("An error has occured: %s", mErrorMessage.c_str());

	//Draw the return button.
	wasMenuPressed = ImGui::Button("Return to Main Menu");

	ImGui::End();
}