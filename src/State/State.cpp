#include "State/State.hpp"

State::State(sf::RenderWindow& window,
			 std::unique_ptr<State>& main_state)
	: mWindow(window),
	  mMainState(main_state)
{
}

State::~State()
{
}

sf::RenderWindow& State::window()
{
	return mWindow;
}

std::unique_ptr<State>& State::state()
{
	return mMainState;
}