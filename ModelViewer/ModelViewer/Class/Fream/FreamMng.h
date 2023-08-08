#pragma once
#include <memory>
#include <sstream>

#include "../Common/Vector2.h"
#include "Fream_DokingArea.h"
#include "Fream_Stage.h"
#include "Fream_SceneView.h"
#include "Fream_Camera.h"
#include "../Device/Device_Mouse.h"
#include "Fream_Item.h"
#include "Fream_FileDialog.h"
#include "../Common/Shader/ShaderMng.h"
#include "../Common/Shader/PostEffectMng.h"
#include "../../Class/System/System_FileCreate.h"
#include "Fream_Model.h"

class FreamMng
{
public:
	FreamMng();
	~FreamMng();

	// ImGuiのダイレクトXとウィンドウの更新
	void SysNewFream();
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
	
	// ウィンドウのスタイル
	void Style();

	void Inspector(); 

	void OptionWindow();
    
	void CreateMenuBer();

	void ObjectDrawField();

	void ConsoleWindow();
	// コンソールにテキストを追加する関数
	void AddConsoleText(const std::string& text);

	std::unique_ptr<Fream_SceneView> sceneView_;
	std::unique_ptr<Fream_DokingArea> dokingArea_;
	std::unique_ptr<Fream_Stage> stage_;
	std::unique_ptr<Fream_Camera> camera_;
	std::unique_ptr<Device_Mouse> mouse_;
	std::unique_ptr<Fream_Item> items_;
	std::unique_ptr<Fream_FileDialog> fileDialog_;
	std::unique_ptr<PostEffectMng> postEffect_;
	std::unique_ptr<System_FileCreate> system_FileCreate_;
	std::unique_ptr<Fream_Model> models_;

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


	int testModel_;
	std::string consoleTextBuffer_;
	int consoleTextBufferCnt_;
	Vector3 scl = { 1.f,1.f,1.f };

	// n桁のゼロ埋めを行う際の数値
	const int ZERO_PADDING_DIGITS = 4;


};

