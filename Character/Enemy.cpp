#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"
#include "Score/Score.h"
#include "MathFunction.h"
#include "CollisionManager/CollisionConfig.h"
#include "GlobalVariables/GlobalVariables.h"

void Enemy::Initialize(
    const std::vector<Model*>& models, const std::vector<uint32_t*>& textureHandles, int32_t level){
	BaseCharacter::Initialize(models, textureHandles, level);
	worldTransform_.translation_.x = float(rand() % 201 - 100);
	worldTransform_.translation_.z = float(rand() % 201 - 100);
	//ワールドトランスフォームの初期化
	worldTransformHead_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	//親子関係
	worldTransformHead_.parent_ = &worldTransform_;
	worldTransformHead_.translation_.y = 1.5f;
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.translation_.x = -0.6f;
	worldTransformL_arm_.translation_.y = 1.3f;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.translation_.x = 0.6f;
	worldTransformR_arm_.translation_.y = 1.3f;
	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);

	//ライフの初期化
	if (level_ == 1) {
		life_ = 1;
	} else if (level_ == 2) {
		life_ = 2;
	} else if (level_ == 3) {
		life_ = 3;
	}

	//グローバル変数の追加
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";
	//グループを追加
	globalVariables->CreateGroup(groupName);
}

void Enemy::Update() { 
	//敵の速度
	const float kEnemySpeed = 0.2f;
	//自キャラのワールド座標を取得する
	Vector3 playerPosition = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得する
	Vector3 enemyPosition = Enemy::GetWorldPosition();
	//自機へのベクトルを算出
	Vector3 targetPosition = Subtract(playerPosition, enemyPosition);
	//正規化する
	targetPosition = Normalize(targetPosition);
	//速度
	Vector3 velocity = Multiply(targetPosition, kEnemySpeed);
	//移動処理
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
	//進行方向に向きを変更
	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);
	//ワールドトランスフォームの更新
	BaseCharacter::Update();
	worldTransformHead_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	//3Dモデルを描画
	if (level_ == 1) {
		models_[0]->Draw(worldTransformHead_, viewProjection, *textureHandles_[0]);
		models_[1]->Draw(worldTransformBody_, viewProjection, *textureHandles_[0]);
		models_[2]->Draw(worldTransformL_arm_, viewProjection, *textureHandles_[0]);
		models_[3]->Draw(worldTransformR_arm_, viewProjection, *textureHandles_[0]);
	} else if (level_ == 2) {
		models_[0]->Draw(worldTransformHead_, viewProjection, *textureHandles_[1]);
		models_[1]->Draw(worldTransformBody_, viewProjection, *textureHandles_[1]);
		models_[2]->Draw(worldTransformL_arm_, viewProjection, *textureHandles_[1]);
		models_[3]->Draw(worldTransformR_arm_, viewProjection, *textureHandles_[1]);
	} else if (level_ == 3) {
		models_[0]->Draw(worldTransformHead_, viewProjection, *textureHandles_[2]);
		models_[1]->Draw(worldTransformBody_, viewProjection, *textureHandles_[2]);
		models_[2]->Draw(worldTransformL_arm_, viewProjection, *textureHandles_[2]);
		models_[3]->Draw(worldTransformR_arm_, viewProjection, *textureHandles_[2]);
	}
}

void Enemy::OnCollision() {
	//ライフを減らす
	life_--;
	//ライフがo以下になったらデスフラグを立てる
	if (life_ <= 0) {
		isDead_ = true;
	}

	//デスフラグがtrueだったら破壊パーティクルを出す
	if (isDead_) {
		//爆発音を再生
		sAudio_->PlayWave(soundDataHandleExplosion_, false, 0.5f);

		//破壊パーティクルを出す
		Vector3 position = Enemy::GetWorldPosition();
		gameScene_->CreateParticleEmitter(ParticleType::Destruction, position, level_);
		// ゲームシーンのスコア加算する
		score_->AddScore(10 * level_);
		score_->AddKills();
	}

	//ヒットパーティクルを出す
	Vector3 position = Enemy::GetWorldPosition();
	gameScene_->CreateParticleEmitter(ParticleType::Hit, position, level_);
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}