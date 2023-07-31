#include "Player.h"
#include "math/MathFunction.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(Model* modelBody, Model* modelL_arm, Model* modelR_arm) {
	//NULLポインタチェック
	assert(modelBody);
	assert(modelL_arm);
	assert(modelR_arm);
	modelBody_ = modelBody;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;
	//ワールドトランスフォームの初期化
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_.x = 0.8f;
	worldTransformL_arm_.translation_.y = 1.0f;
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_.x = -0.8f;
	worldTransformR_arm_.translation_.y = 1.0f;
	//親子関係
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	//浮遊ギミックの初期化
	InitializeFloatingGimmick();
}

void Player::Update() { 
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

		//移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		move = TransformNormal(move, rotateMatrix);

		// 移動方向に見た目を合わせる
		worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);

		// 移動
		worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, move);
	}

	//浮遊ギミックの更新
	UpdateFloatingGimmick();

	//行列の更新
	worldTransformBase_.UpdateMatrix(); 
	worldTransformBody_.UpdateMatrix(); 
	worldTransformL_arm_.UpdateMatrix(); 
	worldTransformR_arm_.UpdateMatrix(); 

	ImGui::Begin(" ");
	ImGui::DragFloat3("Base Translation", &worldTransformBase_.translation_.x, 0.01f);
	ImGui::DragFloat3("Base Rotation", &worldTransformBase_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Body Translation", &worldTransformBody_.translation_.x, 0.01f);
	ImGui::DragFloat3("Body Rotation", &worldTransformBody_.rotation_.x, 0.01f);
	ImGui::DragFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("ArmL Rotation", &worldTransformL_arm_.rotation_.x, 0.01f);
	ImGui::DragFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("ArmR Rotation", &worldTransformR_arm_.rotation_.x, 0.01f);
	ImGui::DragInt("cycle", reinterpret_cast<int*>(&cycle_), 1);
	ImGui::DragFloat("magnitude", &amplitude_, 0.01f);
	ImGui::Text("Lstick : move");
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
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