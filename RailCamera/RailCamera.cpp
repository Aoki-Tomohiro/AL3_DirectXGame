#include "RailCamera.h"
#include "math/MathFunction.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 translation, Vector3 rotation){
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = translation;
	worldTransform_.rotation_ = rotation;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	controlPoints_ = {
        {0,  0,  0  },
        {10, 10, 10 },
        {10, 15, 50 },
	    {20, 15, 80 },
        {20, 0,  120},
        {30, 0,  160},
    };
};

void RailCamera::Update() {
	//視点の媒介変数の処理
	if (targetSection_ != 5) {
		eyet_ += 1.0f / 340.0f;
		if (eyet_ >= 1.0f) {
			eyet_ = 0.0f;
			eyeSection_++;
		}
	}
	
	//注視点の媒介変数の処理
	if (targetSection_ != 5) {
		targett_ += 1.0f / 340.0f;
		if (targett_ >= 1.0f) {
			targett_ = 0.0f;
			targetSection_++;
		}
	}

	//視点の通過点の切り替え
	if (eyeSection_ - 1 == -1) {
		eye_ = Catmull_Rom(
		    controlPoints_[eyeSection_], controlPoints_[eyeSection_],
		    controlPoints_[eyeSection_ + 1], controlPoints_[eyeSection_ + 2], eyet_);
	} else if (eyeSection_ + 1 == 5) {
		eye_ = Catmull_Rom(
		    controlPoints_[eyeSection_ - 1], controlPoints_[eyeSection_],
		    controlPoints_[eyeSection_ + 1], controlPoints_[eyeSection_ + 1], eyet_);
	} else if (eyeSection_ != 5){
		eye_ = Catmull_Rom(
		    controlPoints_[eyeSection_ - 1], controlPoints_[eyeSection_],
		    controlPoints_[eyeSection_ + 1], controlPoints_[eyeSection_ + 2], eyet_);
	}
	
	//注視点の通過点の切り替え
	if (targetSection_ - 1 == -1) {
		target_ = Catmull_Rom(
		    controlPoints_[targetSection_], controlPoints_[targetSection_],
		    controlPoints_[targetSection_ + 1], controlPoints_[targetSection_ + 2], targett_);
	} else if (targetSection_ + 1 == 5) {
		target_ = Catmull_Rom(
		    controlPoints_[targetSection_ - 1], controlPoints_[targetSection_],
		    controlPoints_[targetSection_ + 1], controlPoints_[targetSection_ + 1], targett_);
	} else if (targetSection_ != 5){
		target_ = Catmull_Rom(
		    controlPoints_[targetSection_ - 1], controlPoints_[targetSection_],
		    controlPoints_[targetSection_ + 1], controlPoints_[targetSection_ + 2], targett_);
	}

	//注視点と視点の差分ベクトル
	Vector3 forward = Normalize(Subtract(target_, eye_));

	//z座標軸
	Vector3 z = Subtract(target_, eye_);
	z = Normalize(z);
	//x座標軸
	Vector3 x = Cross({0.0f, 1.0f, 0.0f}, z);
	x = Normalize(x);
	//y座標軸
	Vector3 y = Cross(z, x);
	//view行列の作成
	Matrix4x4 matrix{};
	matrix.m[0][0] = x.x;
	matrix.m[0][1] = y.x;
	matrix.m[0][2] = z.x;
	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = x.y;
	matrix.m[1][1] = y.y;
	matrix.m[1][2] = z.y;
	matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = x.z;
	matrix.m[2][1] = y.z;
	matrix.m[2][2] = z.z;
	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -Dot(eye_, x);
	matrix.m[3][1] = -Dot(eye_, y);
	matrix.m[3][2] = -Dot(eye_, z);
	matrix.m[3][3] = 1.0f;
	viewProjection_.matView = matrix;

	//view行列の位置や方向をワールド行列に反映
	worldTransform_.translation_ = eye_;
	worldTransform_.rotation_.y = std::atan2(forward.x, forward.z);
	float length = Length({forward.x, 0, forward.z});
	worldTransform_.rotation_.x = std::atan2(-forward.y, length);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	ImGui::Begin("Camera");
	ImGui::DragFloat3("CameraTranslation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("CameraRotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat4("WorldTransform m[0]", &worldTransform_.matWorld_.m[0][0], 0.01f);
	ImGui::DragFloat4("WorldTransform m[1]", &worldTransform_.matWorld_.m[1][0], 0.01f);
	ImGui::DragFloat4("WorldTransform m[2]", &worldTransform_.matWorld_.m[2][0], 0.01f);
	ImGui::DragFloat4("WorldTransform m[3]", &worldTransform_.matWorld_.m[3][0], 0.01f);
	ImGui::DragFloat4("ViewProjection m[0]", &viewProjection_.matView.m[0][0], 0.01f);
	ImGui::DragFloat4("ViewProjection m[1]", &viewProjection_.matView.m[1][0], 0.01f);
	ImGui::DragFloat4("ViewProjection m[2]", &viewProjection_.matView.m[2][0], 0.01f);
	ImGui::DragFloat4("ViewProjection m[3]", &viewProjection_.matView.m[3][0], 0.01f);
	ImGui::End();
}