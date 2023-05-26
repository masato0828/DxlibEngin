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
		ImGuiCustom::IM_COLOR color;
		Vector2Flt screenSize;
		Vector2Flt dir;
		int div;
		float val;
		float width;
		float revers;
	};

	Transition_Circle pram_;

	float timeSpeed_;
};

