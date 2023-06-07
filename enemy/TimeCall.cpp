#include "TimeCall.h"
#include "ImGuiManager.h"

TimedCall::TimedCall(std::function<void(void)> f,uint32_t time) { 
	this->f = f;
	this->time_ = time;
}

void TimedCall::Update() {
	if (flag == true) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		flag = true;
		//コールバック関数の呼び出し
		f();
	}
}