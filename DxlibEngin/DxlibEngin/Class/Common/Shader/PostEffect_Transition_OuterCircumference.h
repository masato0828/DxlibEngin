#pragma once
#include "PostEffect_Base.h"
#include "../../Common/Vector2.h"
#include "../../Common/ImGuiMyCustom.h"
class PostEffect_Transition_OuterCircumference :
    public PostEffect_Base
{
public:
	PostEffect_Transition_OuterCircumference();
	~PostEffect_Transition_OuterCircumference();
	void Init() override final;
	void Update() override final;
	void Draw(std::string name, const int imageHnadle) override final;
	void Custom()override final;
private:

	struct Transition_OuterCircumference
	{
		
		ImGuiCustom::IM_COLOR color;// 色
		Vector2Flt screenSize;// スクリーンのサイズ
		Vector2Flt aspect;// アスペクト比
		float time;// 時間
		float div;//
		float direction;//
		float rotation;//回転
		float size;//サイズ
	};

	// パラメータ管理
	Transition_OuterCircumference pram_;

	// 時間のスピード
	float timespeed_;

	// 回転方向
	bool rotationDir_;
};

