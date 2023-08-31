#include "Player.h"
#include "GameScene.h"
#include "math/MathFunction.h"
#include "ImGuiManager.h"
#include "CollisionManager/CollisionConfig.h"
#include "GlobalVariables/GlobalVariables.h"
#include <cassert>

int32_t Player::sLife_;

void Player::Initialize(
    const std::vector<Model*>& models, const std::vector<uint32_t*>& textureHandles, int32_t level){
	//基底クラスの初期化
	BaseCharacter::Initialize(models, textureHandles, level);
	//ワールドトランスフォームの初期化
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_.x = 0.8f;
	worldTransformL_arm_.translation_.y = 1.0f;
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_.x = -0.8f;
	worldTransformR_arm_.translation_.y = 1.0f;
	worldTransformWeapon_.Initialize();
	worldTransformWeapon_.translation_.x = -0.8f;
	//親子関係
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformWeapon_.parent_ = &worldTransformL_arm_;
	//浮遊ギミックの初期化
	Player::InitializeFloatingGimmick();
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);

	//ライフの初期化
	sLife_ = kLifeMax;
	//スプライト生成
	for (int32_t i = 0; i < sLife_; i++) {
		spriteLifes_[i].reset(Sprite::Create(*textureHandles_[2], {i * 50.0f + 30.0f, 30.0f}));
	}

	//発射間隔の初期化
	bulletTime = 20;

	//無敵時間の初期化
	invincibleTimer_ = kInvincibleTime;

	//グローバル変数の追加
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_arm_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_arm_.translation_);
	globalVariables->AddItem(groupName, "floatingCycle", cycle_);
	globalVariables->AddItem(groupName, "floatingAmplitude", amplitude_);
}

void Player::Update() { 
	//Behaviorの初期化
	if (behaviorRequest_) {
		//振るまいを変更する
		behavior_ = behaviorRequest_.value();
		//各振るまいごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			Player::BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			Player::BehaviorAttackInitialize();
			break;
		}
		//振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	//Behaviorの更新
	switch (behavior_){
		//通常行動
	case Behavior::kRoot:
	default:
		Player::BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		Player::BehaviorAttackUpdate();
		break;
	}

	//壁の外に出ないようにする
	const float kMoveLimitX = 99.0f;
	const float kMoveLimitZ = 99.0f;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.z = max(worldTransform_.translation_.z, -kMoveLimitZ);
	worldTransform_.translation_.z = min(worldTransform_.translation_.z, kMoveLimitZ);

	//ライフのテクスチャをシェイクする
	Player::ShakeLifeTexture();

	//無敵時間処理
	if (isInvincible_) {
		invincibleTimer_--;
		if (invincibleTimer_ < 0) {
			invincibleTimer_ = kInvincibleTime;
			isInvincible_ = false;
		}
	}

	//強化状態の処理
	if (isStrong_) {
		// 強化タイマーをデクリメント
		strongTimer_--;
		// 弾の発射間隔を短くする
		bulletTime = 10;

		// 強化タイマーが0以下になったら発射間隔を戻す
		if (strongTimer_ <= 0) {
			isStrong_ = false;
			bulletTime = 20;
		}
	}

	//ライフが０になったらゲームオーバー
	if (sLife_ == 0) {
		isDead_ = true;
	}

	//調整項目
	ApplyGlobalVariables();
}

void Player::Draw(const ViewProjection& viewProjection) {
	//3Dモデルを描画
	if (isInvincible_) {
		if (invincibleTimer_ % 2 == 1) {
			models_[0]->Draw(worldTransformBody_, viewProjection, *textureHandles_[0]);
			models_[1]->Draw(worldTransformL_arm_, viewProjection, *textureHandles_[0]);
			models_[2]->Draw(worldTransformR_arm_, viewProjection, *textureHandles_[0]);
		} else {
			models_[0]->Draw(worldTransformBody_, viewProjection, *textureHandles_[1]);
			models_[1]->Draw(worldTransformL_arm_, viewProjection, *textureHandles_[1]);
			models_[2]->Draw(worldTransformR_arm_, viewProjection, *textureHandles_[1]);
		}
	} else {
		models_[0]->Draw(worldTransformBody_, viewProjection, *textureHandles_[0]);
		models_[1]->Draw(worldTransformL_arm_, viewProjection, *textureHandles_[0]);
		models_[2]->Draw(worldTransformR_arm_, viewProjection, *textureHandles_[0]);
	}
}

void Player::DrawUI() {
	for (int32_t i = 0; i < sLife_; i++) {
		spriteLifes_[i]->Draw();
	}
}

void Player::OnCollision() { 
	//無敵状態ではないとき
	if (isInvincible_ == false) {
		// ライフを減らす
		sLife_--;
		//ダメージの音を鳴らす
		sAudio_->PlayWave(soundDataHandleDamage_, false, 0.8f);
	}
	//ライフの表示をシェイクする
	if (isShake_ == false) {
		isShake_ = true;
		shakeTimer_ = kShakeTime;
	}
	//無敵状態にする
	isInvincible_ = true;
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::BehaviorRootInitialize() { 
	Player::InitializeFloatingGimmick(); 
	worldTransformR_arm_.rotation_.x = 0.0f;
	worldTransformL_arm_.rotation_.x = 0.0f;
}

void Player::BehaviorRootUpdate() {
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//しきい値
		const float threshold = 0.7f;
		//移動フラグ
		bool isMoving = false;
		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX
		};
		//スティックの押し込みが遊び範囲を超えていたら移動フラグをtrueにする
		if (Length(move) > threshold) {
			isMoving = true;
		}
		//移動フラグがtrueの時に移動する
		if (isMoving) {
			// 速さ
			const float speed = 0.3f;
			// 移動量に速さを反映
			move = Multiply(Normalize(move), speed);
			// 移動ベクトルをカメラの角度だけ回転する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
			move = TransformNormal(move, rotateMatrix);
			// 移動方向に見た目を合わせる
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
			// 移動
			worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		}
	}

	// ゲームパッド未接続なら何もせず抜ける
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//攻撃行動を予約
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}

	// 行列の更新
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();

	// 浮遊ギミックの更新
	UpdateFloatingGimmick();
}

void Player::BehaviorAttackInitialize(){
	//すぐに発射できるようにする
	bulletTimer_ = 0;
	worldTransformL_arm_.rotation_.x = -0.785f;
	worldTransformL_arm_.rotation_.y = -0.5f;
	worldTransformR_arm_.rotation_.x = -0.785f;
	worldTransformR_arm_.rotation_.y = 0.5f;
};

void Player::BehaviorAttackUpdate() {
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// カメラ方向に見た目を合わせる
		worldTransform_.rotation_.y = viewProjection_->rotation_.y;

		// RTを押してるとき
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			// しきい値
			const float threshold = 0.7f;
			// 移動フラグ
			bool isMoving = false;
			// 移動量
			Vector3 move = {
			    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
			// スティックの押し込みが遊び範囲を超えていたら移動フラグをtrueにする
			if (Length(move) > threshold) {
				isMoving = true;
			}
			// 移動フラグがtrueの時に移動する
			if (isMoving) {
				// 速さ
				const float speed = 0.1f;
				// 移動量に速さを反映
				move = Multiply(Normalize(move), speed);
				// 移動ベクトルをカメラの角度だけ回転する
				Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
				move = TransformNormal(move, rotateMatrix);
				// 移動
				worldTransform_.translation_ = Add(worldTransform_.translation_, move);
			}

			//行列の更新
			BaseCharacter::Update();
			worldTransformBody_.UpdateMatrix();
			worldTransformL_arm_.UpdateMatrix();
			worldTransformR_arm_.UpdateMatrix();
			worldTransformWeapon_.UpdateMatrix();

			//射撃処理
			Player::Fire();
			return;
		}
	}

	//RTを押していないときは通常行動に戻す
	behaviorRequest_ = Behavior::kRoot;
}

void Player::Fire() {
	// 発射タイマーが０以下の時に弾を発射する
	if (--bulletTimer_ <= 0) {
		// 発射タイマーを戻す
		bulletTimer_ = bulletTime;

		//射撃音を再生
		sAudio_->PlayWave(soundDataHandleShot_, false, 0.5f);

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		Vector3 worldPos{};
		worldPos = Player::GetWorldPosition();
		newBullet->Initialize(models_[3], worldPos, velocity);

		// 弾を登録する
		gameScene_->AddPlayerBullet(newBullet);
	}
}

void Player::InitializeFloatingGimmick() { 
	// 浮遊ギミックの媒介変数の初期化
	floatingParameter_ = 0.0f;
	// 浮遊移動のサイクルの初期化
    cycle_ = 60;
	// 浮遊の振動の初期化
	amplitude_ = 0.1f;
}

void Player::UpdateFloatingGimmick() {
	//1フレームでのパラメータ加算値
	const float step = 2.0f * 3.14f / cycle_;
	//パラメータを１ステップ分加算
	floatingParameter_ += step;
	//2πを超えたら０に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * 3.14f);
	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude_;
	worldTransformL_arm_.rotation_.y = std::sin(floatingParameter_) * amplitude_;
	worldTransformR_arm_.rotation_.y = -std::sin(floatingParameter_) * amplitude_;
}

void Player::ShakeLifeTexture() {
	if (isShake_) {
		shakeTimer_--;
		if (shakeTimer_ > 20) {
			float randX = float(rand() % 11 - 5);
			float randY = float(rand() % 11 - 5);
			for (int32_t i = 0; i < sLife_; i++) {
				spriteLifes_[i]->SetPosition({(i * 50.0f + 30.0f) + randX, 30.0f + randY});
			}
		} 
		else if (shakeTimer_ <= 20 && shakeTimer_ > 10) {
			float randX = float(rand() % 5 - 2);
			float randY = float(rand() % 5 - 2);
			for (int32_t i = 0; i < sLife_; i++) {
				spriteLifes_[i]->SetPosition({(i * 50.0f + 30.0f) + randX, 30.0f + randY});
			}
		}
		else if (shakeTimer_ <= 10 && shakeTimer_ > 0) {
			float randX = float(rand() % 3 - 1);
			float randY = float(rand() % 3 - 1);
			for (int32_t i = 0; i < sLife_; i++) {
				spriteLifes_[i]->SetPosition({(i * 50.0f + 30.0f) + randX, 30.0f + randY});
			}
		}
		else if (shakeTimer_ <= 0) {
			isShake_ = false;
		}
	}
}

void Player::SetStrongState() { 
	//強化状態にする
	isStrong_ = true;
	//強化タイマーの初期化
	strongTimer_ = kStrongTime;
}

void Player::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformL_arm_.translation_ =
	    globalVariables->GetVector3Value(groupName, "ArmL Translation");
	worldTransformR_arm_.translation_ =
	    globalVariables->GetVector3Value(groupName, "ArmR Translation");
	cycle_ = globalVariables->GetIntValue(groupName, "floatingCycle");
	amplitude_ = globalVariables->GetFloatValue(groupName, "floatingAmplitude");
}

void Player::Reset() {
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	//死亡フラグを初期化
	isDead_ = false;
	//発射速度を初期化
	bulletTime = 20;
	//無敵時間を初期化
	Player::ResetInvincibleTimer();
}