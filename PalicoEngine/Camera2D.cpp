#include "Camera2D.h"

namespace Palico
{
	Camera2D::Camera2D(int width, int height) :
		position(0.0f, 0.0f),
		cameraMatrix(1.0f),
		orthoMatrix(1.0f),
		scale(1),
		needsUpdate(true),
		width(width),
		height(height),
		minScale(0.5f),
		maxScale(4.0f)
	{
		this->orthoMatrix = glm::ortho(0.0f, (float)this->width, 0.0f, (float)this->height);
	}


	Camera2D::~Camera2D()
	{
	}


	void Camera2D::update()
	{
		if (needsUpdate)
		{
			// camera translation
			glm::vec3 translate(-position.x + width / 2, -position.y + height / 2, 0.0f);
			cameraMatrix = glm::translate(orthoMatrix, translate);

			// camera scale
			glm::vec3 scale(scale, scale, 0.0f);
			cameraMatrix = glm::scale(glm::mat4(1), scale) * cameraMatrix;

			needsUpdate = false;
		}
	}
	glm::vec2 Camera2D::getWorldPosition(glm::vec2 screenPosition)
	{
		screenPosition.y = height - screenPosition.y;
		screenPosition -= glm::vec2(width / 2, height / 2);
		screenPosition /= scale;
		screenPosition += position;

		return screenPosition;
	}
}