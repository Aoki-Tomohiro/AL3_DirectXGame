#include "FollowCamera.h"
#include "MathFunction.h"
#include "ImGuiManager.h"
#include "GlobalVariables/GlobalVariables.h"

void FollowCamera::Initialize(){
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.rotation_.x = 0.2f;

	// グローバル変数の追加
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	// グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "offset", offset_);
};

void FollowCamera::Update(){
	//追従対象がいれば
	if (target_) {
		//追従対象からカメラまでのオフセット(0度の時の値)
		Vector3 offset = offset_;

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

	//Y軸回転
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//回転の速さ
		const float kRotSpeed = 0.06f;
		//回転
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotSpeed;
	}

	//X軸回転
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 回転の速さ
		const float kRotSpeed = 0.06f;
		// 回転
		viewProjection_.rotation_.x -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kRotSpeed;
	}

	//カメラの回転上限
	if (viewProjection_.rotation_.x < 0.0f) {
		viewProjection_.rotation_.x = 0.0f;
	}
	if (viewProjection_.rotation_.x > 0.8f) {
		viewProjection_.rotation_.x = 0.8f;
	}

	//ビュー行列の更新
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	FollowCamera::ApplyGlobalVariables();
};

void FollowCamera::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	offset_ = globalVariables->GetVector3Value(groupName, "offset");
}

void FollowCamera::Reset() {
	viewProjection_.translation_ = {0.0f, 0.0f, -50.0f};
	viewProjection_.rotation_ = {0.2f, 0.0f, 0.0f};
}