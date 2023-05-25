#pragma once
#include "PostEffect_Base.h"
#include "../../Common/Vector2.h"
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

	struct COLORS
	{
		float r;
		float g;
		float b;
		float a;
	};

	struct Transition_OuterCircumference
	{
		Vector2Flt size;
		COLORS color;
		float time;
		float _rotation;
		float _aspect;
		Vector2Flt _screenSize;
	};

	Transition_OuterCircumference pram_;
};

