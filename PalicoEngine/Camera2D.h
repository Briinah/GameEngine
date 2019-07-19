#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace Palico
{
	class Camera2D
	{
	public:
		Camera2D(int width, int height);
		~Camera2D();

		void update();

		glm::vec2 getWorldPosition(glm::vec2 screenPosition);

		void setPosition(const glm::vec2& newPosition)
		{
			this->position = newPosition;
			needsUpdate = true;
		}
		void setScale(float newScale)
		{
			if (newScale >= minScale && newScale < maxScale)
			{
				this->scale = newScale;
				needsUpdate = true;
			}
		}
		void setScaleDimensions(float newMinScale, float newMaxScale) 
		{ 
			this->minScale = newMinScale; 
			this->maxScale = newMaxScale; 
		}

		glm::vec2 getPosition() { return position; }
		float getScale() { return scale; }
		glm::mat4 getCameraMatrix() { return cameraMatrix; }

	private:
		glm::vec2 position;
		glm::mat4 cameraMatrix;
		glm::mat4 orthoMatrix;

		float scale;
		bool needsUpdate;

		int width;
		int height;

		float minScale;
		float maxScale;
	};

}