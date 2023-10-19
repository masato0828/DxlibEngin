#pragma once
#include "PostEffect_Base.h"
class PostEffect_Led :
    public PostEffect_Base
{
public:
	PostEffect_Led();
	~PostEffect_Led();
	void Init() override final;
	void Update() override final;
	void Draw(std::wstring name, const int imageHnadle) override final;
	void Custom()override final;
private:
	struct Led
	{
		float wdith;
		float height;
	};

	Led pram_;

	
};

