#pragma once
#include <memory>
#include "../Common/Vector2.h"
#include "Fream_DokingArea.h"
#include "Fream_Stage.h"
#include "Fream_SceneView.h"
#include "Fream_Camera.h"
#include "MousePointCreate.h"
#include "../NewFolder1/Device_Mouse.h"

class FreamMng
{
public:
	FreamMng();
	~FreamMng();

	// 更新処理
	void Update();
	// 描画処理
	void Draw();
	// ImGui描画終了処理
	void Render();
	// ImGui終了処理
	void ShutDown();
	// マウスの位置の表示
	void DrawMousePoint();
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

	bool optionWindowFlg_;

	MousePoint mousePoint_;
};

