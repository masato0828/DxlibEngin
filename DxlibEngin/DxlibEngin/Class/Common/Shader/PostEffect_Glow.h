#pragma once
#include "PostEffect_Base.h"
class PostEffect_Glow :
    public PostEffect_Base
{
public:
	PostEffect_Glow();
	~PostEffect_Glow();
	void Init() override final;
	void Update() override final;
	void Draw(std::wstring name, const int imageHnadle) override final;
	void Custom()override final;
private:
	struct LevelParam
	{
		float rblack;
		float rwhite;
		float gblack;
		float gwhite;
		float bblack;
		float bwhite;
	};

	struct Blur
	{
		float sizeX;
		float sizeY;
	};

	LevelParam levelPram_;
	Blur blurPram_;

	int screen_gaussH_;
	int screen_gaussV_;
	int screen_levels_;

	int mainScreen_;
};

