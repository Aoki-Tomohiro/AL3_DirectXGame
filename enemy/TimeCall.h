#pragma once
#include <functional>

//時限発動
class TimedCall {
public:
	//コンストラクタ
	TimedCall(std::function<void(void)> f,uint32_t time);
	//更新
	void Update();
	//完了ならtrueを返す
	bool IsFinished() { return flag; };
	int32_t GetTime() { return time_; };

private:
	//コールバック
	std::function<void(void)> f;
	//残り時間
	uint32_t time_;
	//完了フラグ
	bool flag = false;
};