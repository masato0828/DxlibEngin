#pragma once
#include "PostEffect_Base.h"
#include "../../Common/Vector2.h"
#include "../../Common/ImGuiMyCustom.h"
class PostEffect_Transition_Circle :
    public PostEffect_Base
{
public:
	PostEffect_Transition_Circle();
	~PostEffect_Transition_Circle();
	void Init() override final;
	void Update() override final;
	void Draw(std::string name, const int imageHnadle) override final;
	void Custom()override final;
private:

	struct Transition_Circle
	{
		ImGuiCustom::IM_COLOR color;// 色
		Vector2Flt screenSize;// スクリーンサイズ
		Vector2Flt dir;// 大きくなる方向
		int div;// 円の数
		float val;// 経過時間
		float width;// 円が大きくなるタイミング調整用

		// 1.0：画面のポストエフェクトが入る
		// 0.0：画面のポストエフェクトが無くなる
		float revers;
	};

	// パラメータ管理
	Transition_Circle pram_;

	// 時間
	float timeSpeed_;
};

