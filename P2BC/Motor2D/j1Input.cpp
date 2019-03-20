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
	}
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;

	for (int i = 0; i < MAX_GAMEPADS; ++i)
	{
		delete[] controllers[i].buttons;
		delete[] controllers[i].axis;
	}
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
	
	/*if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0)
	{
		LOG("SDL_HAPTICS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}*/

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
			/*case SDL_CONTROLLERDEVICEADDED:
				total_controllers++;
				if (total_controllers > 4)
				{
					total_controllers = 4;
				}

				if (SDL_IsGameController(curr_controllers))
				{
					controllers[index_Addition_controllers].id_ptr = SDL_GameControllerOpen(index_Addition_controllers);
					curr_controllers++;
					index_Addition_controllers++;
				}*/


			break;

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
				int scale = App->win->GetScale();
				mouse_motion_x = event.motion.xrel / scale;
				mouse_motion_y = event.motion.yrel / scale;
				mouse_x = event.motion.x / scale;
				mouse_y = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;

		}
	}

	//Checking Gamepad input
	int nGameControllers = 0;
	int g_index = -1;
	int nJoysticks = SDL_NumJoysticks();

	SDL_GameController* g[MAX_GAMEPADS] = { nullptr };

	//Check number of joysticks
	for (int i = 0; i < nJoysticks; i++)
	{
		if (SDL_IsGameController(i)) 
		{
			nGameControllers++;
			g_index++;
			g[g_index] = SDL_GameControllerOpen(g_index); // If game controller, open it

			if (SDL_GameControllerGetAttached(g[g_index]) == SDL_TRUE) // If it is opened correctly
			{
				//Check all button states
				for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; ++j) 
				{
					if (SDL_GameControllerGetButton(g[g_index], (SDL_GameControllerButton)j) == 1)
					{
						if (controllers[g_index].buttons[j] == BUTTON_IDLE)
							controllers[g_index].buttons[j] = BUTTON_DOWN;
						else
							controllers[g_index].buttons[j] = BUTTON_REPEAT;
					}
					else
					{
						if (controllers[g_index].buttons[j] == BUTTON_REPEAT || controllers[g_index].buttons[j] == BUTTON_DOWN)
							controllers[g_index].buttons[j] = BUTTON_UP;
						else
							controllers[g_index].buttons[j] = BUTTON_IDLE;
					}
				}

				// Check all Axis & Triggers
				for (int j = 0; j < SDL_CONTROLLER_AXIS_MAX; ++j)
				{
					controllers[g_index].axis[j] = SDL_GameControllerGetAxis(g[g_index], (SDL_GameControllerAxis)j);
				}
			}
		}
	}

	/*SDL_Event event2;
	while (SDL_PollEvent(&event2) != 0)
	{
		switch (event2.type)
		{
		case SDL_CONTROLLERDEVICEADDED:
			int i = 0;
			break;
		}
	}*/


	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
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