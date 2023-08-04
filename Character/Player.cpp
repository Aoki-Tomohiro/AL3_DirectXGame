#include "Player.h"
#include "math/MathFunction.h"
#include "ImGuiManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models) {
	//基底クラスの初期化
	BaseCharacter::Initialize(models);
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
	InitializeFloatingGimmick();
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->SetValue(groupName, "TestInt", 90);
	globalVariables->SetValue(groupName, "TestFloat", 90.0f);
	globalVariables->SetValue(groupName, "TestVector3", Vector3{1.0f, 1.0f, 1.0f});
}

void Player::Update() { 
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 攻撃行動を予約
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}

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
		//攻撃行動
	case Behavior::kAttack:
		Player::BehaviorAttackUpdate();
		break;
	}

	//行列の更新
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix(); 
	worldTransformL_arm_.UpdateMatrix(); 
	worldTransformR_arm_.UpdateMatrix(); 
	worldTransformWeapon_.UpdateMatrix();

	ImGui::Begin(" ");
	ImGui::DragFloat3("Base Translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Base Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Body Translation", &worldTransformBody_.translation_.x, 0.01f);
	ImGui::DragFloat3("Body Rotation", &worldTransformBody_.rotation_.x, 0.01f);
	ImGui::DragFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("ArmL Rotation", &worldTransformL_arm_.rotation_.x, 0.01f);
	ImGui::DragFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("ArmR Rotation", &worldTransformR_arm_.rotation_.x, 0.01f);
	ImGui::DragInt("cycle", reinterpret_cast<int*>(&cycle_), 1);
	ImGui::DragFloat("amplitude", &amplitude_, 0.01f);
	ImGui::Text("Lstick : move");
	ImGui::Text("R1 : Attack");
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection) {
	//3Dモデルを描画
	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformL_arm_, viewProjection);
	models_[2]->Draw(worldTransformR_arm_, viewProjection);
	if (behavior_ == Behavior::kAttack) {
		models_[3]->Draw(worldTransformWeapon_, viewProjection);
	}
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
		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};

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

	// 浮遊ギミックの更新
	UpdateFloatingGimmick();
}

void Player::BehaviorAttackInitialize() { 
	animationTimer_ = 0;
	animationCount_ = 0;
	worldTransformL_arm_.rotation_.x = 0.0f;
	worldTransformR_arm_.rotation_.x = 0.0f;
	worldTransformWeapon_.rotation_.x = 1.5f;
}

void Player::BehaviorAttackUpdate() { 
	animationTimer_++;

	//振りかぶりアニメーション
	if (animationCount_ == 0) {
		if (animationTimer_ == 30) {
			animationCount_++;
			animationTimer_ = 0;
		}
		worldTransformL_arm_.rotation_.x -= 0.1f;
		worldTransformR_arm_.rotation_.x -= 0.1f;
	}
	
	//攻撃アニメーション
	if (animationCount_ == 1) {
		if (animationTimer_ == 15) {
			animationCount_++;
			animationTimer_ = 0;
		}
		worldTransformL_arm_.rotation_.x += 0.2f;
		worldTransformR_arm_.rotation_.x += 0.2f;
	}

	//硬直アニメーション
	if (animationCount_ == 2) {
		if (animationTimer_ == 30) {
			animationCount_++;
			animationTimer_ = 0;
		}
	}

	//通常行動に切り替え
	if (animationCount_ == 3) {
		behaviorRequest_ = Behavior::kRoot;
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