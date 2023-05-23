#pragma once
#include "PostEffect_Base.h"
class PostEffect_Noise :
    public PostEffect_Base
{
public:
	PostEffect_Noise();
	~PostEffect_Noise();
	void Init() override final;
	void Update() override final;
	void Draw(std::string name, const int imageHnadle) override final;
	void Custom()override final;
private:
	struct Noise_Effect
	{
		float time;
		float noise1;
		float noise2;
		float noise3;
	};

	Noise_Effect pram_;
};

