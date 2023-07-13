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
	    {0,  0,  0},
	    {0,  0,  0},
        {10, 10, 0},
        {10, 15, 0},
        {20, 15, 0},
        {20, 0,  0},
        {30, 0,  0},
        {30, 0,  0},
	};
};

void RailCamera::Update() { 
	////視点
	//Vector3 eye;
	//if (eyeSection < 4) {
	//	if (eyet_ < 1.0f) {
	//		eyet_ += 0.01f;
	//	} else {
	//		eyet_ = 0.0f;
	//		eyeSection++;
	//	}
	//}
	//switch (eyeSection) { 
	//case 0:
	//	eye = Catmull_Rom(
	//	    controlPoints_[0], controlPoints_[0], controlPoints_[1], controlPoints_[2], eyet_);
	//	break;
	//case 1:
	//	eye = Catmull_Rom(
	//	    controlPoints_[1], controlPoints_[2], controlPoints_[3], controlPoints_[4], eyet_);
	//	break;
	//case 2:
	//	eye = Catmull_Rom(
	//	    controlPoints_[2], controlPoints_[3], controlPoints_[4], controlPoints_[5], eyet_);
	//	break;
	//case 3:
	//	eye = Catmull_Rom(
	//	    controlPoints_[3], controlPoints_[4], controlPoints_[5], controlPoints_[5], eyet_);
	//	break;
	//}

	////注視点
	//Vector3 target;
	//if (targetSection < 4) {
	//	if (targett_ < 1.0f) {
	//		targett_ += 0.01f;
	//	} else {
	//		targett_ = 0.0f;
	//		targetSection++;
	//	}
	//}
	//switch (targetSection) {
	//case 0:
	//	target = Catmull_Rom(
	//	    controlPoints_[0], controlPoints_[0], controlPoints_[1], controlPoints_[2], targett_);
	//	break;
	//case 1:
	//	target = Catmull_Rom(
	//	    controlPoints_[1], controlPoints_[2], controlPoints_[3], controlPoints_[4], targett_);
	//	break;
	//case 2:
	//	target = Catmull_Rom(
	//	    controlPoints_[2], controlPoints_[3], controlPoints_[4], controlPoints_[5], targett_);
	//	break;
	//case 3:
	//	target = Catmull_Rom(
	//	    controlPoints_[3], controlPoints_[4], controlPoints_[5], controlPoints_[5], targett_);
	//	break;
	//}
	//Vector3 forward = Normalize(Subtract(target, eye));

	if (eyet_ < 4.99f) {
		eyet_ += 0.01f;
	}

	if (targett_ < 4.99f) {
		targett_ += 0.01f;
	}

	Vector3 eye = Catmull_Rom(controlPoints_, eyet_);
	Vector3 target = Catmull_Rom(controlPoints_, targett_);
	Vector3 forward = Normalize(Subtract(target, eye));

	worldTransform_.translation_ = eye;
	worldTransform_.rotation_.y = std::atan2(forward.x, forward.z);
	float length = Length(Vector3{forward.x, 0, forward.z});
	worldTransform_.rotation_.x = std::atan2(-forward.y, length);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	
	ImGui::Begin("Camera");
	ImGui::DragFloat3("CameraTranslate", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("CameraRotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
};