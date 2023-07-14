#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"
#include "MathFunction.h"
#include "collider/CollisionConfig.h"

Player::~Player() { 
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet.release();
	}
	delete sprite2DReticle_;
}

void Player::Initialize(
    Model* model, uint32_t textureHandle, Vector3& playerPosition, uint32_t reticleTextureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = playerPosition;
	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
	SetRadius(1.0f);
	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	//レティクル用テクスチャ取得
	uint32_t textureReticle = reticleTextureHandle;
	//スプライト生成
	sprite2DReticle_ =
	    Sprite::Create(textureReticle, {1280.0f/2, 720.0f/2}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void Player::OnCollision(){};

void Player::Update(const ViewProjection& viewProjection) {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->isDead()) {
			bullet.release();
			return true;
		}
		return false;
	});

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	//回転の速さ
	const float kRotSpeed = 0.02f;

	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

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

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//座標移動(ベクトルの加算)
	/*Move(worldTransform_.translation_, move);*/
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	//行列の更新
	worldTransform_.UpdateMatrix();

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	//Set3DReticlePosition(viewProjection);
	
	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	Set3DReticleMousePosition(viewProjection);

	//キャラクターの座標を画面表示する処理
	ImGui::Begin("PlayerPosition");
	ImGui::SliderFloat3("Player", *inputFloat3, -34, 34);
	ImGui::Text("1     DebugCamera");
	ImGui::Text("Space Shot");
	ImGui::End();
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = Subtract(Player::Get3DReticleWorldPosition(), Player::GetWorldPosition());
		Vector3 nVelocity = Normalize(velocity);
		velocity.x = nVelocity.x * kBulletSpeed;
		velocity.y = nVelocity.y * kBulletSpeed;
		velocity.z = nVelocity.z * kBulletSpeed;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		Vector3 worldPos;
		worldPos = Player::GetWorldPosition();
		newBullet->Initialize(model_, worldPos, velocity);

		// 弾を登録する
		bullets_.push_back(std::unique_ptr<PlayerBullet>(newBullet));
	}

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	//Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = Subtract(Player::Get3DReticleWorldPosition(), Player::GetWorldPosition());
		Vector3 nVelocity = Normalize(velocity);
		velocity.x = nVelocity.x * kBulletSpeed;
		velocity.y = nVelocity.y * kBulletSpeed;
		velocity.z = nVelocity.z * kBulletSpeed;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		Vector3 worldPos;
		worldPos = Player::GetWorldPosition();
		newBullet->Initialize(model_, worldPos, velocity);

		// 弾を登録する
		bullets_.push_back(std::unique_ptr<PlayerBullet>(newBullet));
	}
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
};

Vector3 Player::Get3DReticleWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::DrawUI() {
	sprite2DReticle_->Draw(); 
}

void Player::Set3DReticlePosition(const ViewProjection& viewProjection) {
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを整える
	Vector3 nOffset = Normalize(offset);
	offset.x = nOffset.x * kDistancePlayerTo3DReticle;
	offset.y = nOffset.y * kDistancePlayerTo3DReticle;
	offset.z = nOffset.z * kDistancePlayerTo3DReticle;
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = Player::GetWorldPosition().x + offset.x;
	worldTransform3DReticle_.translation_.y = Player::GetWorldPosition().y + offset.y;
	worldTransform3DReticle_.translation_.z = Player::GetWorldPosition().z + offset.z;
	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = Player::Get3DReticleWorldPosition();
	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

void Player::Set3DReticleMousePosition(const ViewProjection& viewProjection) {
	//スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		//スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
	}
	
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		POINT mousePosition;
		// マウス座標(スクリーン座標)を取得する
		GetCursorPos(&mousePosition);
		// クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);
		// マウス座標を2Dレティクルのスプライトに代入する
		sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));
	}


	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	// 合成行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(
	    float(sprite2DReticle_->GetPosition().x), float(sprite2DReticle_->GetPosition().y), 0);
	Vector3 posFar = Vector3(
	    float(sprite2DReticle_->GetPosition().x), float(sprite2DReticle_->GetPosition().y), 1);

	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 50;
	worldTransform3DReticle_.translation_.x = posNear.x + (mouseDirection.x * kDistanceTestObject);
	worldTransform3DReticle_.translation_.y = posNear.y + (mouseDirection.y * kDistanceTestObject);
	worldTransform3DReticle_.translation_.z = posNear.z + (mouseDirection.z * kDistanceTestObject);
	worldTransform3DReticle_.UpdateMatrix();

	ImGui::Begin("Reticle");
	ImGui::Text(
	    "2DReticle:(%f,&f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
}