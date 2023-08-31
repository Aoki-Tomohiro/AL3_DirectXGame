#include <cassert>
#include "ParticleEmitter.h"

void ParticleEmitter::Initialize(
    Model* model, uint32_t textureHandle, const Vector3& position, ParticleType particleType){
	assert(model);
	//パーティクルの種類を決める
	particleType_ = particleType;
	//破壊パーティクルの生成
	if (particleType_ == ParticleType::Hit) {
		for (int i = 0; i < kHitParticleMax; i++) {
			HitParticle* particle = new HitParticle();
			Vector3 particlePosition = position;
			particle->Initialize(model, particlePosition);
			hitParticles_.push_back(std::unique_ptr<HitParticle>(particle));
		}
	}
	else if (particleType_ == ParticleType::Destruction) {
		for (int i = 0; i < kDestructionParticleMax; i++) {
			DestructionParticle* particle = new DestructionParticle();
			Vector3 particlePosition = position;
			particle->Initialize(model, textureHandle, particlePosition);
			destructionParticles_.push_back(std::unique_ptr<DestructionParticle>(particle));
		}
	}
}

void ParticleEmitter::Update() {
	//ヒットパーティクルの更新処理
	if (particleType_ == ParticleType::Hit) {
		//デスフラグの立ったパーティクルを削除
		hitParticles_.remove_if([](std::unique_ptr<HitParticle>& particle) {
			if (particle->isDead()) {
				particle.reset();
				return true;
			}
			return false;
		});

		//パーティクル更新
		for (std::unique_ptr<HitParticle>& particle : hitParticles_) {
			particle->Update();
		}

		//パーティクルがなくなったらデスフラグを立てる
		if (hitParticles_.size() == 0) {
			isDead_ = true;
		}
	}

	//破壊パーティクルの更新処理
	else if (particleType_ == ParticleType::Destruction) {
		//デスフラグの立ったパーティクルを削除
		destructionParticles_.remove_if([](std::unique_ptr<DestructionParticle>& particle) {
			if (particle->isDead()) {
				particle.reset();
				return true;
			}
			return false;
		});

		//パーティクル更新
		for (std::unique_ptr<DestructionParticle>& particle : destructionParticles_) {
			particle->Update();
		}

		//パーティクルがなくなったらデスフラグを立てる
		if (destructionParticles_.size() == 0) {
			isDead_ = true;
		}
	}
}

void ParticleEmitter::Draw(const ViewProjection& viewProjection) {
	//ヒットパーティクルの描画
	if (particleType_ == ParticleType::Hit) {
		for (std::unique_ptr<HitParticle>& particle : hitParticles_) {
			particle->Draw(viewProjection);
		}
	}
	//破壊パーティクル描画
	else if (particleType_ == ParticleType::Destruction) {
		for (std::unique_ptr<DestructionParticle>& particle : destructionParticles_) {
			particle->Draw(viewProjection);
		}
	}
}