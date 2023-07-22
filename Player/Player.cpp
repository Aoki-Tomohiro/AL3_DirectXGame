#include "Player.h"
#include "math/MathFunction.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
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
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);

		// 移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}

	//行列の更新
	worldTransform_.UpdateMatrix(); 

	ImGui::Begin(" ");
	ImGui::Text("Lstick : move");
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}