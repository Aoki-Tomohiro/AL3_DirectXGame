#include "RailCamera.h"
#include "math/MathFunction.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 translation, Vector3 rotation){
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = translation;
	worldTransform_.rotation_ = rotation;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
};

void RailCamera::Update() { 
	//Vector3 move{0, 0, 0}; 
	//const float kCameraSpeed = 0.02f;

	//// 行列の更新
	//move.z -= kCameraSpeed;
	//Move(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	
	ImGui::Begin("Camera");
	ImGui::DragFloat3("CameraTranslate", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("CameraRotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
};