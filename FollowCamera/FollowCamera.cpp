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
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);
		
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

	//ビュー行列の更新
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	ImGui::Begin(" ");
	ImGui::Text("Rstick : camera");
	ImGui::End();
};