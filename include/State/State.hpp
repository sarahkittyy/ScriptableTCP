#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

/**
 * @brief A program state.
 * Contains methods to draw what it needs to, as well as
 * update it per frame.
 * 
 */
class State
{
public:
	/**
	 * @brief Construct a new State object
	 * 
	 * @param window The SFML window to draw to. 
	 */
	State(sf::RenderWindow& window, std::unique_ptr<State>& main_state);

	/**
	 * @brief Destroy the State object
	 * 
	 */
	virtual ~State();

	/**
	 * @brief Update the state.
	 * 
	 */
	virtual void updateFrame() = 0;

	/**
	 * @brief Draw SFML components.
	 * 
	 */
	virtual void drawSFML() = 0;

	/**
	 * @brief Draw ImGui components.
	 * 
	 */
	virtual void drawImGui() = 0;

protected:
	/**
	 * @brief Return the internal window. Used for child objects.
	 * 
	 * @return sf::RenderWindow& A reference to mWindow.
	 */
	sf::RenderWindow& window();

	/**
	 * @brief Returns a reference to the program state, to change it.
	 * 
	 */
	std::unique_ptr<State>& state();

private:
	/**
	 * @brief The internal window to draw to.
	 * 
	 */
	sf::RenderWindow& mWindow;

	/**
	 * @brief A reference to the main program state.
	 * 
	 */
	std::unique_ptr<State>& mMainState;
};