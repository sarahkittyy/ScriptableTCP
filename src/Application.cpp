#include "Application.hpp"

Application::Application()
	: mWindow(sf::VideoMode(500, 500), "Scriptable TCP")
{
	//Initialization.
	mWindow.setFramerateLimit(60);

	ImGui::SFML::Init(mWindow);

	//Set the initial state.
	mCurrentState.reset(new MainMenu(mWindow, mCurrentState));
}

int Application::run()
{
	//ImGui timer.
	sf::Clock ImGuiClock;

	//Program loop.
	while (mWindow.isOpen())
	{
		//Process events
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			//Process imgui events.
			ImGui::SFML::ProcessEvent(event);

			//SFML events
			switch (event.type)
			{
			case sf::Event::Closed:
				mWindow.close();
				break;
			default:
				break;
			}
		}

		//Update the current state.
		mCurrentState->updateFrame();

		//Update ImGui.
		ImGui::SFML::Update(mWindow, ImGuiClock.restart());

		//Draw current state's ImGui info.
		mCurrentState->drawImGui();

		//End imgui drawing.
		ImGui::EndFrame();

		//Begin SFML drawing.
		mWindow.clear(sf::Color(127, 127, 255));

		//Draw current state's SFML info.
		mCurrentState->drawSFML();

		//Render ImGui.
		ImGui::SFML::Render(mWindow);
		//End SFML drawing.
		mWindow.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}