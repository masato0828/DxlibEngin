#pragma once
#include <string>
class PostEffect_Base
{
public:
	PostEffect_Base();
	~PostEffect_Base();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="name">シェーダーの名前
	/// (ポストエフェクトマネージャーのkey値と一致する必要あり)</param>
	/// <param name="imageHnadle">画像ハンドル</param>
	virtual void Draw(std::wstring name,const int imageHnadle) = 0;

	/// <summary>
	/// カスタム
	/// </summary>
	virtual void Custom() = 0;
private:
};

