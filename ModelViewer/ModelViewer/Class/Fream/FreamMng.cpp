#include <Dxlib.h>
#include <memory>
#include <string>
#include "FreamMng.h"
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_impl_dx11.h"
#include "../../imGui/imgui_impl_win32.h"
#include "../System/ImguiImageStb.h"
#include "../../imGui/imgui_internal.h"
#include "../Common/ImGuiMyCustom.h"

FreamMng::FreamMng()
{
	Init();
}

FreamMng::~FreamMng()
{
}

void FreamMng::Init()
{
	Style();

	ImGui_ImplWin32_Init(GetMainWindowHandle());
	ImGui_ImplDX11_Init(
		(ID3D11Device*)GetUseDirect3D11Device(),
		(ID3D11DeviceContext*)GetUseDirect3D11DeviceContext());

    sceneView_ = std::make_unique<Fream_SceneView>();
    dokingArea_ = std::make_unique<Fream_DokingArea>();
    stage_ = std::make_unique<Fream_Stage>();
    camera_ = std::make_unique<Fream_Camera>();
    mouse_ = std::make_unique<Device_Mouse>();
    items_ = std::make_unique<Fream_Item>();
    fileDialog_ = std::make_unique<Fream_FileDialog>();
    optionWindowFlg_ = false;
    demoWindowActivFlg_ = false;
}

void FreamMng::Update()
{
	SysNewFream();

	// SceneViewPort生成
     // 閉じるボタン用
    static bool m_show = true;

    // ウィンドウ効果
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    window_flags |=
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    // Imgui用ウィンドウクラスの作成
    ImGuiWindowClass window_class;
    // ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定）
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
    ImGui::SetNextWindowClass(&window_class);

    // ウィンドウが開いている
    if (m_show)
    {
        // デスクトップのサイズを取得
        int DesktopW, DesktopH;
        GetDefaultState(&DesktopW, &DesktopH, NULL);
        // ウィンドウの位置
        ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiCond_Once);
        ///ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
        // ウィンドウの大きさ
        ImGui::SetNextWindowSize(ImVec2(DesktopW / 2 + DesktopW / 3, DesktopH / 2 + DesktopH / 3), ImGuiCond_Once);
        ///ImGui::SetNextWindowSize(ImVec2(DesktopW, DesktopH), ImGuiCond_FirstUseEver);
        
        
        // ウィンドウの作成（ウィンドウの名前、開いているか、ウィンドウの効果）
        if (ImGui::Begin("model view", &m_show, window_flags))
        {
            // メニューバーの作成
            CreateMenuBer();

            // ドッキングエリアの作成
            dokingArea_->Create();

            // ウィンドウの終了
            ImGui::End();
            
            //Test();
        }
        Inspector();
    }
    else
    {
        std::exit(0);
    }

    // ステージの更新
    stage_->Update();

    // マウスの更新
    mouse_->Update(
        sceneView_->GetImageLeftUpCornor(),
        sceneView_->GetImageRightDownCornor(),
        sceneView_->GetDefaultImageSize());
    
    // カメラの更新
    camera_->Update(mouse_->GetSceneMousePoint().int_cast(), sceneView_->GetScreenSize() / 2, sceneView_->GetWindowCenterPoint());
    
    // オプション項目を開いているかどうか
    if (optionWindowFlg_) { OptionWindow(); };
  
    // シーンビューの作成
    sceneView_->Create();

    items_->Update();
    fileDialog_->Update();

   
}

void FreamMng::Draw()
{
    stage_->PreviewMake();

    SetDrawScreen(DX_SCREEN_BACK);
    RefreshDxLibDirect3DSetting();
    ClearDrawScreen();

    camera_->Set();
    stage_->Draw();
    mouse_->Draw();
}

void FreamMng::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	//// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void FreamMng::ShutDown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void FreamMng::SysNewFream()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void FreamMng::Style()
{
	// ImGui初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\WINDOWS\\FONTS\\MEIRYO.TTC", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// 全てのウィンドウでのビューポートを表示
	io.ConfigViewportsNoAutoMerge = true;

	// iniファイルの吐き出しを止める
	io.IniFilename = NULL;
	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//　ウィンドウの丸み
        style.WindowRounding = 1.0f;
        style.ChildRounding = 1.0f;
        style.FrameRounding = 1.0f;
        style.PopupRounding = 1.0f;
        style.ScrollbarRounding = 1.0f;
        style.GrabRounding = 1.0f;
        style.LogSliderDeadzone = 1.0f;
        style.TabRounding = 1.0f;

		// 指定ウィンドウ（ここでは背景）の変更
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
	}

    ImGui::StyleColorsClassic();
}

void FreamMng::Inspector()
{
    // Imgui用ウィンドウクラスの作成
    ImGuiWindowClass window_classview;
    // ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
    window_classview.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
    ImGui::SetNextWindowClass(&window_classview);
    ImGui::Begin("Inspector");
    ImGui::End();
}

void FreamMng::CreateMenuBer()
{
    static bool open = false;
    // メニューバーの作成
    if (ImGui::BeginMenuBar())
    {
        // メニュー1
        if (ImGui::BeginMenu("File"))
        {
            // メニューの中身
            if (ImGui::MenuItem("new data"))
            {
                // 
                {
                    auto id = MessageBoxA(NULL, "Do you want to save the current data?", "SAVE", MB_OKCANCEL);
                    if (id == IDOK)
                    {
                        // 全初期化
                    }
                }
            }
            // メニューの中身
            if (ImGui::BeginMenu("add model"))
            {
                ImGui::EndMenu();
            };
            if (ImGui::BeginMenu("model data"))
            {
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        // メニュー2
        if (ImGui::BeginMenu("System")) {

#ifndef IMGUI_DISABLE_DEBUG_TOOLS
            const bool has_debug_tools = true;
#else
            const bool has_debug_tools = false;
#endif
            ImGui::MenuItem("Option", NULL, &optionWindowFlg_, has_debug_tools);
            //ImGui::MenuItem("Debug Log", NULL, &show_app_debug_log, has_debug_tools);
            //ImGui::MenuItem("Stack Tool", NULL, &show_app_stack_tool, has_debug_tools);
            //ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
            //ImGui::MenuItem("About Dear ImGui", NULL, &show_app_about);
            //ImGui::EndMenu();

            

            ImGui::EndMenu();
        };
        // メニュー3
        if (ImGui::BeginMenu("View")) { ImGui::EndMenu(); };
        // メニュー4
        if (ImGui::BeginMenu("Project")) { ImGui::EndMenu(); };
        // メニュー5
        if (ImGui::BeginMenu("Desktop")) { ImGui::EndMenu(); };
        // メニュー6
        if (ImGui::BeginMenu("Help")) { ImGui::EndMenu(); };

        // メニューバーの終了
        ImGui::EndMenuBar();
    }

    if (open)
    {
        ImGui::Begin("single export");/*
        if (ImGui::Button("export"))
        {
            auto fileName = FilePathErase()(
                FileSeve(nullptr, "json", "jsonファイル(*.json)\0 * .json\0")
                );
            std::ofstream writing_file;
            writing_file.open(fileName, std::ios::out);
            fModel_->SingleE(writing_file);
            writing_file.close();


            open = false;
        }*/


        ImGui::End();


    }
}

void FreamMng::OptionWindow()
{
    // Imgui用ウィンドウクラスの作成
    ImGuiWindowClass window_classview;
    // ウィンドウの効果の編集(ウィンドウのドッキングは切っておく)
    window_classview.DockNodeFlagsOverrideSet = 
        ImGuiDockNodeFlags_NoDocking| 
        ImGuiDockNodeFlags_NoDockingSplitMe| 
        ImGuiDockNodeFlags_NoDockingSplitOther| 
        ImGuiDockNodeFlags_NoDockingOverMe| 
        ImGuiDockNodeFlags_NoDockingOverOther|
        ImGuiDockNodeFlags_NoDockingOverEmpty;
    ImGui::SetNextWindowClass(&window_classview);

    if (ImGui::Begin("Option"))
    {
        ImGui::Checkbox("demoWindowActivFlg_", &demoWindowActivFlg_);

        if (demoWindowActivFlg_)
        {
            ImGui::ShowDemoWindow();
        }

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Stage"))
            {
                stage_->Custom();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Camera"))
            {
                camera_->Custom();
                ImGui::EndTabItem();
            }


            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}
