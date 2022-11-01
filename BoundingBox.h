#pragma once
#include "Terrain.h"
class BoundingBox
{
public:
	glm::vec3 offsetPosition;
	float height;
	float radius;

	glm::vec3 GetPosition(glm::vec3 position) {
		return position + offsetPosition;
	}

	BoundingBox(){}
	BoundingBox(glm::vec3 offsetPosition, float height, float radius) {
		this->offsetPosition = offsetPosition;
		this->height = height;
		this->radius = radius;
	}
};

