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
		
		ImGuiCustom::IM_COLOR color;
		Vector2Flt screenSize;
		Vector2Flt aspect;
		float time;
		float div;
		float direction;
		float rotation;
		float size;
		/*float _rotation;
		float _aspect;
		Vector2Flt _screenSize;
		float _drection;*/
	};

	Transition_OuterCircumference pram_;

	float timespeed_;

	bool rotationDir_;
};

