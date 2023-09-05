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
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="screen">指定スクリーン</param>
	void Draw(const int screen);
	
	/// <summary>
	/// カスタム
	/// </summary>
	void Custom();
private:

	// ポストエフェクト管理(key値,ポストエフェクト)
	std::unordered_map<std::string,std::unique_ptr<PostEffect_Base>> postEffect_;

	// 選択中のポストエフェクトのkey値
	std::string selectName_;

	
};

