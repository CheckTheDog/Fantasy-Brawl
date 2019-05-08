#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
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
			if (SDL_GameControllerGetAttached(controllers[i].id_ptr) == SDL_TRUE) // If it is opened correctly
			{
				//Check all button states basically, ame process as keyboard but with gamepads
				for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; ++j) 
				{
					if (SDL_GameControllerGetButton(controllers[i].id_ptr, (SDL_GameControllerButton)j) == 1)
					{
						if (controllers[i].buttons[j] == BUTTON_IDLE)
							controllers[i].buttons[j] = BUTTON_DOWN;
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
			}
			else if (controllers[i].id_ptr != nullptr) // Controller disattached, close (and set to nullptr)
			{
				SDL_GameControllerClose(controllers[i].id_ptr);
			    controllers[i].id_ptr = nullptr;

				/*SDL_HapticClose(controllers[i].haptic_ptr);
				controllers[i].haptic_ptr = nullptr;*/
			}
	}

	//Testing
	if(GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	LoadConfigBinding();

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


GP_BUTTON_STATE j1Input::GetButton(PLAYER p, BUTTON_BIND id) const
{
	if (controllers[(int)p].id_ptr != nullptr && controllers[(int)p].binded_buttons[(int)id].bindType == SDL_CONTROLLER_BINDTYPE_BUTTON)
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

void j1Input::LoadConfigBinding()
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
	customized_controllers[0] = customs.attribute("P1").as_bool();
	customized_controllers[1] = customs.attribute("P2").as_bool();
	customized_controllers[2] = customs.attribute("P3").as_bool();
	customized_controllers[3] = customs.attribute("P4").as_bool();

	for (int i = 0; i < MAX_GAMEPADS; ++i)
	{
		// If the controller has customized input in the xml
		if (customized_controllers[i] == true)
		{

		}
		else // Default binding!
		{
			/*config = config.child("default_binding");*/
			int bind = 0; // We start with the first button from BUTTON_BIND enum
			BindButton((PLAYER)i, (BUTTON_BIND)bind, config.child("default_binding").child("basic_attack").attribute("SDL_BUTTON").as_int());
			bind++;

			BindButton((PLAYER)i, (BUTTON_BIND)bind, config.child("default_binding").child("super_attack").attribute("SDL_BUTTON").as_int());
			bind++;

			BindButton((PLAYER)i, (BUTTON_BIND)bind, config.child("default_binding").child("shield").attribute("SDL_BUTTON").as_int());
		}
	}

}
