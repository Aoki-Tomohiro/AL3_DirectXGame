#include "EnemyStateLeave.h"
#include "Enemy.h"

void EnemyStateLeave::Update(Enemy* pEnemy) {
	// 移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kEnemySpeed = 0.2f;

	// 移動(ベクトルを加算)
	move.x -= kEnemySpeed;
	move.y += kEnemySpeed;
	move.z -= kEnemySpeed;
	// 移動(ベクトルを加算)
	pEnemy->EnemyMove(move);
}