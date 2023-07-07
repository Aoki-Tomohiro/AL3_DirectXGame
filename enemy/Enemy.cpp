#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include "MathFunction.h"
#include "player/Player.h"
#include "collider/CollisionConfig.h"

Enemy::~Enemy() {
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {10.0f, 0.0f, 50.0f};
	char fname[] = "enemy/enemySpeed.txt";
	err_ = fopen_s(&fp, fname, "r");
	if (fp != 0  && err_ == 0)
	{
		fscanf_s(fp, "%f", &enemySpeed_);
		fclose(fp);
	}
	state_ = new EnemyStateApproach();
	state_->Initialize(this);
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
	SetRadius(1.0f);
}

void Enemy::ChangeState(BaseEnemyState* newState) { 
	state_ = newState;
}

void Enemy::EnemyMove(Vector3 move) {
	Move(worldTransform_.translation_, move);
}

void Enemy::OnCollision(){};

void Enemy::Update() {
	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->isDead()) {
			bullet.reset();
			return true;
		}
		return false;
	});

	// 弾の削除
	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished() == true) {
			delete timedCall;
			return true;
		}
		return false;
	});

	state_->Update(this);

	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Fire() {
	assert(player_);

	//弾の速度
	const float kBulletSpeed = 1.0f;

	//自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得する
	Vector3 enemyPos = worldTransform_.translation_;
	Vector3 targetPos = Subtract(playerPos,enemyPos);
	Vector3 n = Normalize(targetPos);
	Vector3 velocity = {n.x * kBulletSpeed, n.y * kBulletSpeed, n.z * kBulletSpeed};

	//弾の生成
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_,velocity);
	newBullet->SetPlayer(player_);

	//弾を登録する
	bullets_.push_back(std::unique_ptr<EnemyBullet>(newBullet));
}

void Enemy::FireReset() {
	Fire();
	// メンバ関数と呼び出し元をbindしてstd::functionに代入
	std::function<void(void)> callback = std::bind(&Enemy::FireReset, this);
	//時限発動イベントを生成
	TimedCall* timedCall = new TimedCall(callback, 60);
	//時限発動イベントを時限発動イベントリストに追加
	timedCalls_.push_back(timedCall);
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void EnemyStateApproach::Initialize(Enemy* pEnemy) { 
	pEnemy->SetFireTimer(pEnemy->kFireInterval);
	pEnemy->FireReset();
}

void EnemyStateApproach::Update(Enemy* pEnemy) {
	// 移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	 const float kEnemySpeed = pEnemy->GetEnemySpeed();

	// 移動(ベクトルを加算)
	 move.z -= kEnemySpeed;
	// 座標移動(ベクトルの加算)
	 pEnemy->EnemyMove(move);
	// 既定の位置に到達したら離脱
	if (pEnemy->GetWorldTransform().translation_.z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}

	//範囲forでリストの全要素について回す
	for (TimedCall* timedCall : pEnemy->GetTimedCall()) {
		timedCall->Update();
	}

	//弾の発射
	/*pEnemy->SetFireTimer(pEnemy->GetFireTimer() - 1);
	if (pEnemy->GetFireTimer() < 0) {
		pEnemy->Fire();
		pEnemy->SetFireTimer(pEnemy->kFireInterval);
	}*/

	//弾更新
	const std::list<std::unique_ptr<EnemyBullet>>& bullets = pEnemy->GetEnemyBullets();
	for (const std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}
}

void EnemyStateLeave::Initialize(Enemy* pEnemy) { 
	pEnemy->SetFireTimer(pEnemy->kFireInterval);
}

void EnemyStateLeave::Update(Enemy* pEnemy) {
	// 移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kEnemySpeed = pEnemy->GetEnemySpeed();

	// 移動(ベクトルを加算)
	move.x -= kEnemySpeed;
	move.y += kEnemySpeed;
	move.z -= kEnemySpeed;
	// 移動(ベクトルを加算)
	pEnemy->EnemyMove(move);

	// 弾更新
	const std::list<std::unique_ptr<EnemyBullet>>& bullets = pEnemy->GetEnemyBullets();
	for (const std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
};