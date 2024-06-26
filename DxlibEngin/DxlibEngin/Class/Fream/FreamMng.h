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
#include "../Common/Gizumo.h"
#include "Fream_Console.h"

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

	// ウィンドウ状態の取得
	bool GetShowWindow();
private:

	// 初期化
	void Init();
	
	// ウィンドウのスタイル
	void Style();

	void Inspector(); 

	void OptionWindow();

	void ViewWindow();
    
	void CreateMenuBer();

	void ObjectDrawField();

	void OutputSystemWindow();
	void InputSystemWindow();

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
	std::unique_ptr<Gizumo> gizumo_;
	std::unique_ptr<Fream_Console> console_;

	bool optionWindowFlg_;

	std::map<std::wstring, bool> viewFlg_;
	std::vector<std::wstring> viewName_;

	bool demoWindowActivFlg_;

	Vector2Flt copyWindowSize_;
	Vector2Flt copyWindowPos_;
	bool windowMaxFlag_;
	bool windowMinFlag_;
	bool firstWindowFlg_;
	bool outputWindowFlg_;
	bool inputWindowFlg_;

	HWND hwnd_;

	bool m_show;

	int screen_;
	int systemUIScreen_;
	int cubeTexture_;

	const int CUBE_TEXTURE_NUM = 6;

	Vector3 scl = { 1.f,1.f,1.f };

	bool is_close_;

	struct Light
	{
		int customLightHandle_;
		Vector3 lightPos_;
		float Range_;
		float Atten0_;
		float Atten1_;
		float Atten2_;
		COLOR_F dcolor_;
		COLOR_F scolor_;
		COLOR_F acolor_;
		bool alive_;
	};

	Light light;
	Light defLight;
	
};

