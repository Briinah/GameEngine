#include "InputManager.h"
#include <iostream>
#include <SDL\SDL.h>

namespace Palico
{

	InputManager::InputManager() : mousePosition(0)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		SDL_Event evnt;

		keyPressed = 0;

		while (SDL_PollEvent(&evnt))
		{
			switch (evnt.type)
			{
			case SDL_MOUSEMOTION:
				setMousePosition(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_KEYDOWN:
				pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				pressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				releaseKey(evnt.button.button);
				break;
			}
		}
	}

	void InputManager::pressKey(unsigned int keyID)
	{
		keyMap[keyID] = true;
		keyPressed = keyID;
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		keyMap[keyID] = false; 
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		auto it = keyMap.find(keyID);
		if (it != keyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		return keyPressed == keyID;
	}

	void InputManager::setMousePosition(float x, float y)
	{
		mousePosition.x = x;
		mousePosition.y = y;
	}
}