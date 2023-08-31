#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "PostEffect_Base.h"
class PostEffectMng
{
public:
	PostEffectMng();
	~PostEffectMng();
	void Init();
	void Update();
	void Draw(const int screen);
	void Custom();
private:
	std::unordered_map<std::string,std::unique_ptr<PostEffect_Base>> postEffect_;

	std::string selectName_;

	
};

