#include "State/Host.hpp"

Host::Host(sf::RenderWindow& window,
		   std::unique_ptr<State>& main_state,
		   unsigned short port)
	: State(window, main_state),
	  mServer(port)
{
	//Initialize the script box.
	TextEditor::LanguageDefinition lua_lang = TextEditor::LanguageDefinition::Lua();
	lua_lang.mAutoIndentation				= true;
	mScriptBox.SetLanguageDefinition(lua_lang);

	//Set the default code.
	mScriptBox.SetText("-- A simple echo script.\n\nx = arg\nprint(x)");
}

void Host::updateFrame()
{
	//Button states.
	if (wasMenuPressed)
	{
		//Return to the main menu.
		state().reset(new MainMenu(window(), state()));
		return;
	}
	///////////
	//Toggle read-only depending on the state of isRunning.
	mScriptBox.SetReadOnly(mServer.isRunning());

	//Update the socket data string.
	socket_data = stringifyInfo();

	//Socket logic//
	mServer.updateListener();

	//Receive data that's ready to be received.
	mServer.updateClients();

	//If the server has data...
	while (mServer.isData())
	{
		auto data			= mServer.popData();
		std::string bytes   = data.second;
		Server::Socket sock = data.first;

		//If we're running...
		if (mServer.isRunning())
		{
			//Split the string, and evaluate the lua.
			std::string lua_result = evaluateLua(splitString(bytes));

			//Send the result back through the socket.
			mServer.sendData(lua_result, sock);
		}
		else
		{
			//Send a filler message.
			mServer.sendData("Server is online but not handing incoming data.\n", sock);
		}
	}
}

std::string Host::evaluateLua(std::vector<std::string> args)
{
	//The stream to be printed to by the lua state.
	std::stringstream output;

	//Create the print function.
	auto print = sol::overload(
		//THE STRING OVERLOAD
		[&output](std::string str) { output << str; },
		//THE NUMBER OVERLOAD
		[&output](double val) { output << val; },
		//THE LIST OVERLOAD
		[&output](sol::table table) {
			bool isFirst = true;
			for (auto& i : table)
			{
				//Deduce the type of the item in the table.
				std::string item;
				sol::type type = i.second.get_type();
				if (type == sol::type::string)
				{
					item = i.second.as<std::string>();
				}
				else if (type == sol::type::number)
				{
					item = std::to_string(i.second.as<double>());
				}

				//Render the first opening brace if it's the first elem.
				if (isFirst)
				{
					isFirst = false;
					output << "[" << item;
					continue;
				}
				//Otherwise, output the item.
				output << ", " << item;
			}
			//Finish outputting.
			output << "]";
		});

	//To number function.
	auto tonumber = [&](std::string arg) { return std::stod(arg); };
	auto tostring = [&](double number) { return std::to_string(number); };

	//Create the lua state.
	sol::state lua;

	//Bind the standard functions.
	lua.set_function("print", print);
	lua.set_function("tonumber", tonumber);
	lua.set_function("tostring", tostring);

	//Bind the argument array.
	lua.set("arg", sol::as_table(args));

	//Evaluate the script.
	try
	{
		lua.script(mScriptBox.GetText());
	}
	catch (const std::exception& e)   //If we throw an exception
	{
		//Return an error message.
		output.str("");
		output << "Error: Lua exception in server -- perhaps your input is wrong?\n";
		output << "Error message >> " << e.what();
	}

	output << "\n";
	//Return the output.
	return output.str();
}

std::vector<std::string> Host::splitString(std::string data)
{
	//Create the vector to append each string to.
	std::vector<std::string> ret;

	//For appending characters as we iterate.
	std::string current_string = "";
	//Iterate over the string.
	for (auto& i : data)
	{
		//If it is a space..
		if (std::isspace(i))
		{
			//Append current_string and clear it. (unless the string is empty).
			if (!current_string.empty())
			{
				ret.push_back(current_string);
				current_string.clear();
			}
		}
		else   //Otherwise..
		{
			//Append the character to the current string.
			current_string.push_back(i);
		}
	}
	//Append the last bits of the argument.
	if (!current_string.empty())
	{
		ret.push_back(current_string);
	}

	return ret;
}

void Host::error(std::string msg)
{
	//Reset to the error state.
	state().reset(new Error(window(),
							state(),
							msg));
}

std::string Host::stringifyInfo()
{
	//Create a new stringstream.
	std::stringstream stream;

	//The header..
	stream << "Connected Clients:\n";
	for (auto& sock : mServer.getClients())
	{
		//Append each connected client to the stream.
		stream << sock->getRemoteAddress()
			   << ":"
			   << sock->getRemotePort()
			   << "\n";
	}

	//Return the stringified stream.
	return stream.str();
}

void Host::drawSFML()
{
	//Nothing to do...
}

void Host::drawImGui()
{
	//Draw the main window.
	ImGui::SetNextWindowSize(window().getSize());
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("ScriptableTCP Server",
				 nullptr,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	//Render the scripting box.
	mScriptBox.Render("Script", ImVec2(480, 250), false);

	//Render a clear button.
	if (ImGui::Button("<> Clear Script"))
	{
		mScriptBox.SetText("");
		mServer.setRunning(false);
	}

	//Render the pause/play button.
	std::string running_button = (mServer.isRunning()) ? ("|| Pause") : ("> Start");
	if (ImGui::Button(running_button.c_str()))
	{
		//Toggle
		mServer.setRunning(!mServer.isRunning());
	}

	//Render the back button.
	wasMenuPressed = ImGui::Button("<- Main Menu");

	ImGui::Text("--------\n");

	//Render the connection info.
	ImGui::TextWrapped("%s", socket_data.c_str());

	ImGui::End();
}