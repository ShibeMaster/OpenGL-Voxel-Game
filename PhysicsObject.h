#pragma once
#include "Terrain.h"
#include "PhysicsCache.h"
#include "PhysicsExtensions.h"
#include "Mesh.h"
#include "Renderer.h"
#include "BoundingBox.h"

class PhysicsObject
{
public:
	PhysicsCache cache;
	glm::vec3 velocity = glm::vec3(0.0f);
	BoundingBox boundingBox;
	Mesh mesh;
	bool applyDrag;
	bool useCollisions;
	bool useGravity;
	bool grounded = false;
	bool initialized = false;
	float velocityLimitY = 12.0f;

	PhysicsObject() {}
	PhysicsObject(bool useGravity, bool useCollisions, bool applyDrag, glm::vec3 boundingBoxOffset = glm::vec3(0.0f), float boundingBoxHeight = 0.0f, float boundingBoxRadius = 0.0f) {
		this->applyDrag = applyDrag;
		this->useCollisions = useCollisions;
		this->useGravity = useGravity;
		std::cout << "initialized" << std::endl;
		this->boundingBox = BoundingBox(boundingBoxOffset, boundingBoxHeight, boundingBoxRadius);
		this->initialized = true;
	}
	void CheckCollisions(glm::vec3 position) {
		glm::vec3 yCheckingPosition = boundingBox.GetPosition(position);
		yCheckingPosition.y -= boundingBox.height;
		if (velocity.y < 0) {
			if (Terrain::GetPositionValue(yCheckingPosition) != 0) {
				velocity.y = 0;
				grounded = true;
				std::cout << "collided y" << std::endl;
			}
			else {
				grounded = false;
			}
		}

		glm::vec3 checkingPosition = boundingBox.GetPosition(position);
		checkingPosition.y -= boundingBox.height / 2;
		glm::vec3 xCheckingPosition = checkingPosition;
		xCheckingPosition.x = xCheckingPosition.x + velocity.x * Time::fixedDeltaTime;
		glm::vec3 xCheckingPositionOffsetPosition = xCheckingPosition;
		xCheckingPositionOffsetPosition.x = velocity.x < 0 ? floor(xCheckingPosition.x) : ceil(xCheckingPosition.x);
		glm::vec3 zCheckingPosition = checkingPosition;
		zCheckingPosition.z = zCheckingPosition.z + velocity.z * Time::fixedDeltaTime;

		glm::vec3 zCheckingPositionOffsetPosition = zCheckingPosition;
		zCheckingPositionOffsetPosition.z = velocity.z < 0 ? floor(zCheckingPositionOffsetPosition.z) : ceil(zCheckingPositionOffsetPosition.z);
		glm::vec3 checkingBoth = checkingPosition;
		checkingBoth.x = xCheckingPositionOffsetPosition.x;
		checkingBoth.z = zCheckingPositionOffsetPosition.z;
		bool xcollided = false;
		bool zcollided = false;
		if (Terrain::GetPositionValue(xCheckingPosition) != 0 || Terrain::GetPositionValue(xCheckingPositionOffsetPosition) != 0) {
			velocity.x = 0.0f;
			xcollided = true;
			std::cout << "collided x" << std::endl;
		}
		if (Terrain::GetPositionValue(zCheckingPosition) != 0 || Terrain::GetPositionValue(zCheckingPositionOffsetPosition) != 0) {
			velocity.z = 0.0f;
			zcollided = true;
			std::cout << "collided z" << std::endl;
		}
	}
	glm::vec3 PhysicsFixedUpdate(glm::vec3 position) {
		if (applyDrag)
			velocity = PhysicsExtensions::ApplyDrag(velocity);

		if (useGravity)
			velocity = PhysicsExtensions::ApplyGravity(velocity);


		// if (useCollisions)
			// CheckCollisions(position);

		// if (velocity.y > velocityLimitY)
			// velocity.y = velocityLimitY;
		cache.CacheVariables(position, velocity);

		return position + velocity * Time::fixedDeltaTime;
	}
};

