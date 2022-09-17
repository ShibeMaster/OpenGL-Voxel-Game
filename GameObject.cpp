#include "GameObject.h"
#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GameObject::GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca) : position(pos), rotation(rot), scale(sca){}