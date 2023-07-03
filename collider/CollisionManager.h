#pragma once
#include "player/Player.h"
#include <list>

class CollisionManager{
public:
	void SetColliderList(Collider* collider) { colliders_.push_back(collider); };
	void ClearColliderList();
	void CheckAllCollisions();
	
private:
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

private:
	std::list<Collider*> colliders_;
};