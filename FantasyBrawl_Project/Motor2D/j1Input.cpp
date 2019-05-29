#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_gamecontroller.h"
#include "SDL/include/SDL_haptic.h"


#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name.assign("input");

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);

	for (int i = 0; i < MAX_GAMEPADS; ++i)
	{
		controllers[i].buttons = new GP_BUTTON_STATE[SDL_CONTROLLER_BUTTON_MAX];
		memset(controllers[i].buttons, BUTTON_IDLE, sizeof(GP_BUTTON_STATE) * SDL_CONTROLLER_BUTTON_MAX);

		controllers[i].axis = new int[SDL_CONTROLLER_AXIS_MAX];
		memset(controllers[i].axis, 0, sizeof(int) * SDL_CONTROLLER_AXIS_MAX);

		//Allocate space for button binders
		controllers[i].binded_buttons = new SDL_GameControllerButtonBind[uint(BUTTON_BIND::MAX_BUTTON_BIND)];
		memset(controllers[i].binded_buttons, SDL_CONTROLLER_BUTTON_MAX, sizeof(SDL_GameControllerButtonBind) * (int)BUTTON_BIND::MAX_BUTTON_BIND);

		controllers[i].triggers_state = new GP_BUTTON_STATE[2]; // There are only 2 Triggers in a gamepad
		memset(controllers[i].triggers_state, BUTTON_IDLE, sizeof(GP_BUTTON_STATE) * 2);

		controllers[i].multidirection_axis_state = new GP_AXIS_STATE[4]; // There are only 4 multidirectional axis in a gamepad, left X & Y, Right X & Y
		memset(controllers[i].multidirection_axis_state, (int)GP_AXIS_STATE::AXIS_IDLE, sizeof(GP_AXIS_STATE) * 4);
	}
}

// Destructor
j1Input::~j1Input()
{
	//Keyboard
	delete[] keyboard;

}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
	
	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0)
	{
		LOG("SDL_HAPTICS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	SDL_GameControllerEventState(SDL_ENABLE);

	if (SDL_GameControllerEventState(SDL_QUERY) != 1)
	{
		LOG("SDL_GAME_CONTROLLER_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	for (int i = 0; i< MAX_GAMEPADS; ++i)
	LoadConfigBinding((PLAYER)i);

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	static SDL_Event event;
	
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if(mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{

			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
			{
				float scale = App->win->GetScale();
				mouse_motion_x = event.motion.xrel / scale;
				mouse_motion_y = event.motion.yrel / scale;
				mouse_x = event.motion.x / scale;
				mouse_y = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			}
			break;

			//When a controller is plugged in
			case SDL_CONTROLLERDEVICEADDED:
			{
				int n_joys = SDL_NumJoysticks();

				if (SDL_IsGameController(n_joys - 1))
				{
					for (int i = 0; i < n_joys; ++i)
					{
						if (controllers[i].id_ptr == nullptr) // If there isn't a gamepad connected  already
						{
							if (controllers[i].index == -1) //First time a gamepad has been connected
							{
								controllers[i].id_ptr = SDL_GameControllerOpen(index_addition_controllers);

								controllers[i].haptic_ptr = SDL_HapticOpen(index_addition_controllers);
								SDL_HapticRumbleInit(controllers[i].haptic_ptr);

								controllers[i].index = index_addition_controllers;
								LoadConfigBinding((PLAYER)controllers[i].index);
							}
							else    //The gamepad was disconnected at some point and is now being reconnected
							{
								controllers[i].id_ptr = SDL_GameControllerOpen(controllers[i].index);
							}

							
							// This index will assign the proper index for a gamapd that has been connected once
							// in case it is disconnected and connected again it will use the value of the var 
							// at the moment of opening the gamepad
							if (index_addition_controllers < MAX_GAMEPADS - 1)
								index_addition_controllers++;

							break;
						}
					}
				}
			}
			break;

		}
	}

	//Check Gamepads
	for (int i = 0; i < MAX_GAMEPADS; i++)
	{
		controllers[i].any_button_down = false;
			if (SDL_GameControllerGetAttached(controllers[i].id_ptr) == SDL_TRUE) // If it is opened correctly
			{
				//Check all button states basically, ame process as keyboard but with gamepads
				for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; ++j) 
				{
					if (SDL_GameControllerGetButton(controllers[i].id_ptr, (SDL_GameControllerButton)j) == 1)
					{
						if (controllers[i].buttons[j] == BUTTON_IDLE)
						{
							controllers[i].buttons[j] = BUTTON_DOWN;
							controllers[i].last_button_pressed = j;
							controllers[i].any_button_down = true;
						}
						else
							controllers[i].buttons[j] = BUTTON_REPEAT;
					}
					else
					{
						if (controllers[i].buttons[j] == BUTTON_REPEAT || controllers[i].buttons[j] == BUTTON_DOWN)
							controllers[i].buttons[j] = BUTTON_UP;
						else
							controllers[i].buttons[j] = BUTTON_IDLE;
					}
				}

				// Check all Axis & Triggers
				for (int j = 0; j < SDL_CONTROLLER_AXIS_MAX; ++j)
				{
					controllers[i].axis[j] = SDL_GameControllerGetAxis(controllers[i].id_ptr, (SDL_GameControllerAxis)j);
				}

				// Check Triggers to work as states
				for (int j = 0; j < 2; ++j)
				{
					if (SDL_GameControllerGetAxis(controllers[i].id_ptr, SDL_GameControllerAxis(j + (int)SDL_CONTROLLER_AXIS_TRIGGERLEFT)) > 0.8f * AXISMAX)
					{
						if (controllers[i].triggers_state[j] == BUTTON_IDLE)
							controllers[i].triggers_state[j] = BUTTON_DOWN;
						else
							controllers[i].triggers_state[j] = BUTTON_REPEAT;
					}
					else
					{
						if (controllers[i].triggers_state[j] == BUTTON_REPEAT || controllers[i].triggers_state[j] == BUTTON_DOWN)
							controllers[i].triggers_state[j] = BUTTON_UP;
						else
							controllers[i].triggers_state[j] = BUTTON_IDLE;
					}
				}

				// Check Multidirection axis to work as states
				for (int j = 0; j < 4; ++j)
				{
					if (SDL_GameControllerGetAxis(controllers[i].id_ptr, SDL_GameControllerAxis(j)) > 0.8f * AXISMAX) //Stick all the way to the positive
					{
						if (controllers[i].multidirection_axis_state[j] == GP_AXIS_STATE::AXIS_IDLE || 
							IN_RANGE(controllers[i].multidirection_axis_state[j], GP_AXIS_STATE::AXIS_NEGATIVE_DOWN, GP_AXIS_STATE::AXIS_NEGATIVE_REPEAT))
							controllers[i].multidirection_axis_state[j] = GP_AXIS_STATE::AXIS_POSITIVE_DOWN;
						else
							controllers[i].multidirection_axis_state[j] = GP_AXIS_STATE::AXIS_POSITIVE_REPEAT;
					}
					else if ( IN_RANGE(controllers[i].multidirection_axis_state[j], GP_AXIS_STATE::AXIS_POSITIVE_DOWN, GP_AXIS_STATE::AXIS_POSITIVE_RELEASE))
					{
						if (controllers[i].multidirection_axis_state[j] == GP_AXIS_STATE::AXIS_POSITIVE_REPEAT || controllers[i].multidirection_axis_state[j] == GP_AXIS_STATE::AXIS_POSITIVE_DOWN)
							controllers[i].multidirection_axis_state[j] = GP_AXIS_STATE::AXIS_POSITIVE_RELEASE;
						else
							controllers[i].multidirection_axis_state[j] = GP_AXIS_STATE::AXIS_IDLE;
					}
					else if (SDL_GameControllerGetAxis(controllers[i].id_ptr, SDL_GameControllerAxis(j)) < -(0.8f * AXISMAX))// Stick All the way negative
					{
						if (controllers[i].multidirection_axis_state[j] == GP_AXIS_STATE::AXIS_IDLE ||
							IN_RANGE(controllers[i].multidirection_axis_state[j], GP_AXIS_STATE::AXIS_POSITIVE_DOWN, GP_AXIS_STATE::AXIS_POSITIVE_REPEAT))
							controllers[i].multidirection_axis_state[j] = GP_AXIS_STATE::AXIS_NEGATIVE_DOWN;
						else
							controllers[i].multidirection_axis_state[j] = GP_AXIS_STATE::AXIS_NEGATIVE_REPEAT;
					}
					else if (IN_RANGE(controllers[i].multidirection_axis_state[j], GP_AXIS_STATE::AXIS_NEGATIVE_DOWN, GP_AXIS_STATE::AXIS_NEGATIVE_RELEASE)) // Stick neither all the way negative or positive
					{
						if (controllers[i].multidirection_axis_state[j] == GP_AXIS_STATE::AXIS_NEGATIVE_REPEAT || controllers[i].multidirection_axis_state[j] == GP_AXIS_STATE::AXIS_NEGATIVE_DOWN)
							controllers[i].multidirection_axis_state[j] = GP_AXIS_STATE::AXIS_NEGATIVE_RELEASE;
						else
							controllers[i].multidirection_axis_state[j] = GP_AXIS_STATE::AXIS_IDLE;
					}
				}


			}
			else if (controllers[i].id_ptr != nullptr) // Controller disattached, close (and set to nullptr)
			{
				SDL_GameControllerClose(controllers[i].id_ptr);
			    controllers[i].id_ptr = nullptr;

				/*SDL_HapticClose(controllers[i].haptic_ptr);
				controllers[i].haptic_ptr = nullptr;*/
			}
	}

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	

	for (int i = 0; i < MAX_GAMEPADS; ++i)
	{
		if (controllers[i].id_ptr != nullptr)
		{
			if (SDL_GameControllerGetAttached(controllers[i].id_ptr))
			//SDL_GameControllerClose(controllers[i].id_ptr); //seems to have a very weird bug where it crashes the app

			controllers[i].id_ptr = nullptr;
		}

		if (controllers[i].haptic_ptr != nullptr)
		{
			SDL_HapticClose(controllers[i].haptic_ptr);
			controllers[i].haptic_ptr = nullptr;
		}

		delete[] controllers[i].buttons;
		delete[] controllers[i].axis;
		delete[] controllers[i].binded_buttons;
		
		/*controllers[i].buttons = nullptr;
		controllers[i].axis = nullptr;
		controllers[i].binded_buttons = nullptr;*/
		delete[] controllers[i].triggers_state;
		delete[] controllers[i].multidirection_axis_state;
	}

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	SDL_QuitSubSystem(SDL_INIT_HAPTIC);

	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::ForceKeyboardKeyState(int id, j1KeyState state)
{
	keyboard[id] = state;
}


GP_BUTTON_STATE j1Input::GetTriggerState(PLAYER p, int id) const
{
	if (id == SDL_CONTROLLER_AXIS_TRIGGERLEFT || SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
	{
		if (controllers[(int)p].id_ptr != nullptr)
			return controllers[(int)p].triggers_state[id - (int)SDL_CONTROLLER_AXIS_TRIGGERLEFT];
	}
	else
		return BUTTON_IDLE;
}

GP_AXIS_STATE j1Input::GetLRAxisState(PLAYER p, int id) const
{
	if (IN_RANGE(id,(int)SDL_CONTROLLER_AXIS_LEFTX, (int)SDL_CONTROLLER_AXIS_RIGHTY))
	{
		if (controllers[(int)p].id_ptr != nullptr)
			return controllers[(int)p].multidirection_axis_state[id - (int)SDL_CONTROLLER_AXIS_LEFTX];
	}
	else
		return GP_AXIS_STATE::AXIS_IDLE;
}

GP_BUTTON_STATE j1Input::GetButton(PLAYER p, BUTTON_BIND id) const
{
	if (controllers[(int)p].id_ptr != nullptr && controllers[(int)p].binded_buttons[(int)id].bindType == SDL_CONTROLLER_BINDTYPE_BUTTON)
		return controllers[(int)p].buttons[controllers[(int)p].binded_buttons[(int)id].value.button];
	else if (controllers[(int)p].id_ptr != nullptr && controllers[(int)p].binded_buttons[(int)id].bindType == SDL_CONTROLLER_BINDTYPE_HAT)
		return controllers[(int)p].buttons[controllers[(int)p].binded_buttons[(int)id].value.button];
	else
		return BUTTON_IDLE;
}

void j1Input::BindButton(PLAYER p, BUTTON_BIND bind, int button_to_bind)
{
	controllers[(int)p].binded_buttons[(int)bind] = SDL_GameControllerGetBindForButton(controllers[(int)p].id_ptr, (SDL_GameControllerButton)button_to_bind);
	controllers[(int)p].binded_buttons[(int)bind].value.button = (SDL_GameControllerButton)button_to_bind;
}

void j1Input::ShakeController(PLAYER p, float intensity, uint32 length)
{
	if (controllers[(int)p].id_ptr != nullptr)
		SDL_HapticRumblePlay(controllers[(int)p].haptic_ptr, intensity, length);
}

void j1Input::ForceButtonState(PLAYER p, int id, GP_BUTTON_STATE state)
{
	if (controllers[(int)p].id_ptr != nullptr)
		 controllers[(int)p].buttons[id] = state;
}

void j1Input::StopControllerShake(PLAYER p)
{
	if (controllers[(int)p].id_ptr != nullptr)
	SDL_HapticRumbleStop(controllers[(int)p].haptic_ptr);
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

int j1Input::GetBindRealButton(PLAYER p, BUTTON_BIND bind) const
{
		return int(controllers[(int)p].binded_buttons[(int)bind].value.button);
}

void j1Input::LoadConfigBinding(PLAYER p)
{
	//Load the config
	pugi::xml_document	config_file;
	pugi::xml_node config;

	pugi::xml_parse_result result = config_file.load_file("config_input.xml");

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		config = config_file.child("config_input");

	//Once the config is loaded iterate and get the data
	bool customized_controllers[MAX_GAMEPADS] = {false};
	pugi::xml_node customs = config.child("customized_controllers");
	switch (p)
	{
	case PLAYER::P1:
		customized_controllers[0] = customs.attribute("P1").as_bool();
		break;
	case PLAYER::P2:
		customized_controllers[1] = customs.attribute("P2").as_bool();
		break;
	case PLAYER::P3:
		customized_controllers[2] = customs.attribute("P3").as_bool();
		break;
	case PLAYER::P4:
		customized_controllers[3] = customs.attribute("P4").as_bool();
		break;

	}
	
	// If the controller has customized input in the xml
	if (customized_controllers[(int)p] == true)
	{

	}
	else // Default binding!
	{
		/*config = config.child("default_binding");*/
		int bind = 0; // We start with the first button from BUTTON_BIND enum
		BindButton((PLAYER)p, (BUTTON_BIND)bind, config.child("default_binding").child("basic_attack").attribute("SDL_BUTTON").as_int());
		bind++;

		BindButton((PLAYER)p, (BUTTON_BIND)bind, config.child("default_binding").child("special_attack").attribute("SDL_BUTTON").as_int());
		bind++;

		BindButton((PLAYER)p, (BUTTON_BIND)bind, config.child("default_binding").child("super_attack").attribute("SDL_BUTTON").as_int());
		bind++;

		BindButton((PLAYER)p, (BUTTON_BIND)bind, config.child("default_binding").child("shield").attribute("SDL_BUTTON").as_int());
	}
}

void j1Input::AddBindingToConfig(PLAYER p)
{
	//Load the config
	pugi::xml_document	config_file;
	pugi::xml_node config;

	pugi::xml_parse_result result = config_file.load_file("config_input.xml");

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		config = config_file.child("config_input");

	pugi::xml_node customs = config.child("customized_controllers");
	customs.append_child("MAMMA MIA!");

	switch (p)
	{
	case PLAYER::P1:
		customs.append_attribute("P1") = 1;
		break;
	case PLAYER::P2:
		customs.append_attribute("P2");
		customs.append_attribute("P2").set_value("1");
		break;
	case PLAYER::P3:
		customs.remove_attribute("P3");
		customs.append_attribute("P3").set_value("1");
		break;
	case PLAYER::P4:
		customs.remove_attribute("P4");
		customs.append_attribute("P4").set_value("1");
		break;
	default:
		break;
	}

	

}

bool j1Input::OnUIEvent(UI_element* element, event_type event_type)
{
	bool ret = true;

	switch (event_type)
	{
	case event_type::MOUSE_LEFT_CLICK:
		{
			if (element->function == element_function::POLLING_CUSTOMIZE)
			{
				element->function = CUSTOMIZE;
				element->is_locked = true;
			}
			break;
		}
	case event_type::MARKER_OVER_CUSTOMIZER:
		{
		if (element->element_type == MARKER)
		{
		}
			break;
		}
	case event_type::MARKER_OVER_LOCKED_ELEMENT:
		{
			break;
		}
	};
	return ret;
}

bool j1Input::OnUIEvent(UI_element* element, event_type event_type, int p)
{
	bool ret = true;
	
	switch (event_type)
	{
		case event_type::BUTTON_ANY:
		{
			if (element->function == CUSTOMIZE)
			{
				//Bind the button
				int button_bind = element->element_type - CUSTOMIZING_BUTTON_BASIC;
				
				BindButton(PLAYER::P1,(BUTTON_BIND)button_bind,controllers[p].last_button_pressed);
				element->section = App->gui->GetButtonRect(controllers[p].last_button_pressed);

				AddBindingToConfig((PLAYER)p);
				element->function = POLLING_CUSTOMIZE;
				element->is_locked = false;
			}
			break;
		}
	};
	return ret;
}
