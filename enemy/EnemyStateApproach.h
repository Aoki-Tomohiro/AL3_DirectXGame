#pragma once
#include "IEnemyState.h"

class EnemyStateApproach : public IEnemyState {
public:
	void Initialize(Enemy* pEnemy);
	void Update(Enemy* pEnemy);
};