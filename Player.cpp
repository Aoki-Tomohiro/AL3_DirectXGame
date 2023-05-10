#include <Player.h>
#include <cassert>
#include "ImGuiManager.h"
#include "MathFunction.h"

//void Move(Vector3 &transform,Vector3 &move){ 
//	transform.x += move.x;
//	transform.y += move.y;
//	transform.z += move.z;
//}
//
//Matrix4x4 MakeRotateXMatrix(float radian) {
//	Matrix4x4 result;
//	result.m[0][0] = 1;
//	result.m[0][1] = 0;
//	result.m[0][2] = 0;
//	result.m[0][3] = 0;
//
//	result.m[1][0] = 0;
//	result.m[1][1] = std::cos(radian);
//	result.m[1][2] = std::sin(radian);
//	result.m[1][3] = 0;
//
//	result.m[2][0] = 0;
//	result.m[2][1] = -std::sin(radian);
//	result.m[2][2] = std::cos(radian);
//	result.m[2][3] = 0;
//
//	result.m[3][0] = 0;
//	result.m[3][1] = 0;
//	result.m[3][2] = 0;
//	result.m[3][3] = 1;
//
//	return result;
//}
//
//Matrix4x4 MakeRotateYMatrix(float radian) {
//	Matrix4x4 result;
//	result.m[0][0] = std::cos(radian);
//	result.m[0][1] = 0;
//	result.m[0][2] = -std::sin(radian);
//	result.m[0][3] = 0;
//
//	result.m[1][0] = 0;
//	result.m[1][1] = 1;
//	result.m[1][2] = 0;
//	result.m[1][3] = 0;
//
//	result.m[2][0] = std::sin(radian);
//	result.m[2][1] = 0;
//	result.m[2][2] = std::cos(radian);
//	result.m[2][3] = 0;
//
//	result.m[3][0] = 0;
//	result.m[3][1] = 0;
//	result.m[3][2] = 0;
//	result.m[3][3] = 1;
//
//	return result;
//}
//
//Matrix4x4 MakeRotateZMatrix(float radian) {
//	Matrix4x4 result;
//	result.m[0][0] = std::cos(radian);
//	result.m[0][1] = std::sin(radian);
//	result.m[0][2] = 0;
//	result.m[0][3] = 0;
//
//	result.m[1][0] = -std::sin(radian);
//	result.m[1][1] = std::cos(radian);
//	result.m[1][2] = 0;
//	result.m[1][3] = 0;
//
//	result.m[2][0] = 0;
//	result.m[2][1] = 0;
//	result.m[2][2] = 1;
//	result.m[2][3] = 0;
//
//	result.m[3][0] = 0;
//	result.m[3][1] = 0;
//	result.m[3][2] = 0;
//	result.m[3][3] = 1;
//
//	return result;
//}
//
//Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
//	Matrix4x4 result;
//	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
//	                 m1.m[0][3] * m2.m[3][0];
//	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
//	                 m1.m[0][3] * m2.m[3][1];
//	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
//	                 m1.m[0][3] * m2.m[3][2];
//	result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
//	                 m1.m[0][3] * m2.m[3][3];
//
//	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
//	                 m1.m[1][3] * m2.m[3][0];
//	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
//	                 m1.m[1][3] * m2.m[3][1];
//	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
//	                 m1.m[1][3] * m2.m[3][2];
//	result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
//	                 m1.m[1][3] * m2.m[3][3];
//
//	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
//	                 m1.m[2][3] * m2.m[3][0];
//	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
//	                 m1.m[2][3] * m2.m[3][1];
//	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
//	                 m1.m[2][3] * m2.m[3][2];
//	result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
//	                 m1.m[2][3] * m2.m[3][3];
//
//	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
//	                 m1.m[3][3] * m2.m[3][0];
//	result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
//	                 m1.m[3][3] * m2.m[3][1];
//	result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
//	                 m1.m[3][3] * m2.m[3][2];
//	result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
//	                 m1.m[3][3] * m2.m[3][3];
//
//	return result;
//}
//
//Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
//	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
//	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
//	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
//	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
//	Matrix4x4 result;
//	result.m[0][0] = scale.x * rotateXYZMatrix.m[0][0];
//	result.m[0][1] = scale.x * rotateXYZMatrix.m[0][1];
//	result.m[0][2] = scale.x * rotateXYZMatrix.m[0][2];
//	result.m[0][3] = 0.0f;
//
//	result.m[1][0] = scale.y * rotateXYZMatrix.m[1][0];
//	result.m[1][1] = scale.y * rotateXYZMatrix.m[1][1];
//	result.m[1][2] = scale.y * rotateXYZMatrix.m[1][2];
//	result.m[1][3] = 0.0f;
//
//	result.m[2][0] = scale.z * rotateXYZMatrix.m[2][0];
//	result.m[2][1] = scale.z * rotateXYZMatrix.m[2][1];
//	result.m[2][2] = scale.z * rotateXYZMatrix.m[2][2];
//	result.m[2][3] = 0.0f;
//
//	result.m[3][0] = translate.x;
//	result.m[3][1] = translate.y;
//	result.m[3][2] = translate.z;
//	result.m[3][3] = 1.0f;
//
//	return result;
//}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN))
	{
		move.y -= kCharacterSpeed;
	}

	//移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//座標移動(ベクトルの加算)
	MathFunction::Move(worldTransform_.translation_, move);

	//行列更新
	worldTransform_.matWorld_ = MathFunction::MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix(); 

	//キャラクターの座標を画面表示する処理
	ImGui::Begin("PlayerPosition");
	ImGui::Text(
	    "%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}