#pragma once
#include "PostEffect_Base.h"
class PostEffect_ScanLine :
    public PostEffect_Base
{
public:
	PostEffect_ScanLine();
	~PostEffect_ScanLine();
	void Init() override final;
	void Update() override final;
	void Draw(std::string name, const int imageHnadle) override final;
	void Custom()override final;
private:
	struct ScanLine
	{
		float time;
		float pram1;
		float pram2;
		float pram3;
		float pram4;
		float pram5;
		float pram6;
		float pram7;
	};

	ScanLine pram_;

	float speed_;
};

