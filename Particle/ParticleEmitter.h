#pragma once
#include <list>
#include <memory>
#include "HitParticle.h"
#include "DestructionParticle.h"

//パーティクルの種類
enum class ParticleType {
	Hit,
	Destruction,
};

/// <summary>
/// パーティクルエミッター
/// </summary>
class ParticleEmitter {
public:
	//パーティクルの最大数
	static const size_t kHitParticleMax = 50;
	static const size_t kDestructionParticleMax = 100;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
	    Model* model, uint32_t textureHandle, const Vector3& position, ParticleType particleType);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// デスフラグを取得
	/// </summary>
	/// <returns></returns>
	bool isDead() const { return isDead_; };

private:
	//ヒットパーティクルのリスト
	std::list<std::unique_ptr<HitParticle>> hitParticles_{};
	//破壊パーティクルのリスト
	std::list<std::unique_ptr<DestructionParticle>> destructionParticles_{};
	//デスフラグ
	bool isDead_ = false;
	//パーティクルの種類
	ParticleType particleType_;
};