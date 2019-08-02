#include "Camera2D.h"

namespace Palico
{
	Camera2D::Camera2D(int width, int height) :
		position(0.0f, 0.0f),
		cameraMatrix(1.0f),
		orthoMatrix(1.0f),
		width(width),
		height(height)
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
			glm::vec3 newScale(scale, scale, 0.0f);
			cameraMatrix = glm::scale(glm::mat4(1), newScale) * cameraMatrix;

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

	bool Camera2D::isInViewPort(const glm::vec2& position, const glm::vec2& dimensions)
	{
		const float scaledWidth = width / scale;
		const float scaledHeight = height / scale;

		const float MIN_WIDTH = dimensions.x / 2.0f + scaledWidth / 2.0f;
		const float MIN_HEIGHT = dimensions.y / 2.0f + scaledHeight / 2.0f;
		
		// AABB
		glm::vec2 objectPosition = position + dimensions / 2.0f;
		glm::vec2 distance = objectPosition - this->position;

		float xDepth = MIN_WIDTH - abs(distance.x);
		float yDepth = MIN_HEIGHT - abs(distance.y);

		if (xDepth > 0 && yDepth > 0)
		{
			return true;
		}

		return false;
	}
}