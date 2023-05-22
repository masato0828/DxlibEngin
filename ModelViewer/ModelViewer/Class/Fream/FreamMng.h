#pragma once
#include <memory>
#include "../Common/Vector2.h"
#include "Fream_DokingArea.h"
#include "Fream_Stage.h"
#include "Fream_SceneView.h"
#include "Fream_Camera.h"
#include "../Device/Device_Mouse.h"
#include "Fream_Item.h"
#include "Fream_FileDialog.h"
#include "../Common/Shader/ShaderMng.h"

class FreamMng
{
public:
	FreamMng();
	~FreamMng();

	// 更新処理
	void Update(bool window_open_flg);
	// 描画処理
	void Draw();
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

	void Inspector(); 

	void OptionWindow();
    
	void CreateMenuBer();

	std::unique_ptr<Fream_SceneView> sceneView_;
	std::unique_ptr<Fream_DokingArea> dokingArea_;
	std::unique_ptr<Fream_Stage> stage_;
	std::unique_ptr<Fream_Camera> camera_;
	std::unique_ptr<Device_Mouse> mouse_;
	std::unique_ptr<Fream_Item> items_;
	std::unique_ptr<Fream_FileDialog> fileDialog_;

	bool optionWindowFlg_;
	bool demoWindowActivFlg_;

	Vector2 copyWindowSize_;
	Vector2 copyWindowPos_;
	bool windowMaxFlag_;
	bool windowMinFlag_;
	bool firstWindowFlg_;

	HWND hwnd_;

	bool m_show;

	int screen_;
};

