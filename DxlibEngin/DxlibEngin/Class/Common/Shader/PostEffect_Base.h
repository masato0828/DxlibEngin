#pragma once
#include <string>
class PostEffect_Base
{
public:
	PostEffect_Base();
	~PostEffect_Base();
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw(std::string name,const int imageHnadle) = 0;
	virtual void Custom() = 0;
private:
};

