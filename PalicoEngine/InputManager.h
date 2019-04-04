#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace Palico
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		bool isKeyDown(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		void setMousePosition(float x, float y);
		glm::vec2 getMousePosition() const { return mousePosition; }

	private:

		std::unordered_map<unsigned int, bool> keyMap;
		std::unordered_map<unsigned int, bool> keyPressedMap;

		glm::vec2 mousePosition;

		unsigned int keyPressed;
	};

}