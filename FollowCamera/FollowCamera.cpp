#include "FollowCamera.h"
#include "MathFunction.h"
#include "ImGuiManager.h"

void FollowCamera::Initialize(){
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
};

void FollowCamera::Update(){
	//追従対象がいれば
	if (target_) {
		//追従対象からカメラまでのオフセット(0度の時の値)
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		//カメラの角度から回転行列を計算する
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(viewProjection_.rotation_.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(viewProjection_.rotation_.z);
		Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
		//オフセットをカメラの回転に合わせて回転させる
		offset = TransformNormal(offset, rotateMatrix);

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//回転の速さ
		const float kRotSpeed = 0.06f;
		//回転
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotSpeed;
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 回転の速さ
		const float kRotSpeed = 0.06f;
		// 回転
		viewProjection_.rotation_.x -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kRotSpeed;
	}

	//ビュー行列の更新
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	ImGui::Begin(" ");
	ImGui::Text("Rstick : camera");
	ImGui::Text(
	    "%f,%f,%f,%f", viewProjection_.matView.m[0][0], viewProjection_.matView.m[0][1],
	    viewProjection_.matView.m[0][2], viewProjection_.matView.m[0][3]);
	ImGui::Text(
	    "%f,%f,%f,%f", viewProjection_.matView.m[1][0], viewProjection_.matView.m[1][1],
	    viewProjection_.matView.m[1][2], viewProjection_.matView.m[1][3]);
	ImGui::Text(
	    "%f,%f,%f,%f", viewProjection_.matView.m[2][0], viewProjection_.matView.m[2][1],
	    viewProjection_.matView.m[2][2], viewProjection_.matView.m[2][3]);
	ImGui::Text(
	    "%f,%f,%f,%f", viewProjection_.matView.m[3][0], viewProjection_.matView.m[3][1],
	    viewProjection_.matView.m[3][2], viewProjection_.matView.m[3][3]);
	ImGui::End();
};