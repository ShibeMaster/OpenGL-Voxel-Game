#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer.h"
#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
class GameObject
{
public:
	glm::vec3 position, rotation, scale, velocity;

	GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca);
};
#endif;
