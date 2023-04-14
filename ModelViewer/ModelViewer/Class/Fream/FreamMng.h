#pragma once

class FreamMng
{
public:
	FreamMng();
	~FreamMng();

	// 更新処理
	void Update();
	// ImGui描画終了処理
	void Render();
	// ImGui終了処理
	void ShutDown();

private:
	// 初期化
	void Init();
	// ImGuiのダイレクトXとウィンドウの更新
	void SysNewFream();
	// ウィンドウのスタイル
	void Style();


};

