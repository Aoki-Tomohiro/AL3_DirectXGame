#pragma once
#include "IEnemyState.h"

class EnemyStateLeave : public IEnemyState {
public:
	void Initialize(Enemy* pEnemy);
	void Update(Enemy* pEnemy);
};