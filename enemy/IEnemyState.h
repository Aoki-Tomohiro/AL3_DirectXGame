#pragma once

class Enemy;

class IEnemyState {
public:
	virtual void Initialize(Enemy* pEnemy) = 0;
	virtual void Update(Enemy* pEnemy) = 0;
};