#include "EnemyStateApproach.h"
#include "Enemy.h"

void EnemyStateApproach::Update(Enemy* pEnemy) {
	// 移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kEnemySpeed = 0.2f;

	// 移動(ベクトルを加算)
	move.z -= kEnemySpeed;
	// 座標移動(ベクトルの加算)
	pEnemy->EnemyMove(move);
	// 既定の位置に到達したら離脱
	if (pEnemy->GetWorldTransform().translation_.z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}
}