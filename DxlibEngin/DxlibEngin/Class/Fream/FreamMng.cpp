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
#include "../Common/PostMyGraph.h"
#include "../Common/Utility.h"

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

    system_FileCreate_ = std::make_unique<System_FileCreate>();

    sceneView_ = std::make_unique<Fream_SceneView>();
    dokingArea_ = std::make_unique<Fream_DokingArea>();
    stage_ = std::make_unique<Fream_Stage>();
    camera_ = std::make_unique<Fream_Camera>();
    mouse_ = std::make_unique<Device_Mouse>();
    items_ = std::make_unique<Fream_Item>();
    fileDialog_ = std::make_unique<Fream_FileDialog>();
    postEffect_ = std::make_unique<PostEffectMng>();
    models_ = std::make_unique<Fream_Model>();
    gizumo_ = std::make_unique<Gizumo>();
    console_ = std::make_unique<Fream_Console>();

    optionWindowFlg_ = false;
    demoWindowActivFlg_ = false;
    firstWindowFlg_ = false;
    windowMaxFlag_ = false;
    windowMinFlag_ = false;
    outputWindowFlg_ = false;
    inputWindowFlg_ = false;

    m_show = true;

    int ww, wh;
    GetWindowSize(&ww,&wh);
    screen_ = MakeScreen(ww,wh,true);
    systemUIScreen_ = MakeScreen(ww,wh,true);

    SetCubeMapTextureCreateFlag(true);
    cubeTexture_ = MakeScreen(1024,1024,true);
    SetCubeMapTextureCreateFlag(false);

    viewFlg_ = { 
        {L"console",false},
        {L"codeEditer",false},
    };
}

void FreamMng::Update(bool window_open_flg)
{

    //models_->SetModelHandle(sceneView_->GetModelMap());
    models_->SetModelPath(sceneView_->GetDropModelPath());
    postEffect_->Update();
    

    system_FileCreate_->IsMainFile();

    if (!system_FileCreate_->GetAppOpenFlg())
    {
        return;
    }


    if (firstWindowFlg_)
    {
        if (windowMaxFlag_)
        {
            Vector2 w;
            GetWindowSize(&w.x_, &w.y_);
            ImGui::SetWindowSize("##model view", ImVec2(static_cast<float>(w.x_), static_cast<float>(w.y_)));
            ImGui::SetWindowPos("##model view", ImVec2(0.0f, 0.0f));
        }
        else
        {
            if (windowMinFlag_)
            {
                ImGui::SetWindowPos("##model view", ImVec2(copyWindowPos_.x_, copyWindowPos_.y_));
                ImGui::SetWindowSize("##model view", ImVec2(copyWindowSize_.x_, copyWindowSize_.y_));
                windowMinFlag_ = false;
            }
        }
    }
	
    // ウィンドウ効果
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    window_flags |=
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoTitleBar;

    // Imgui用ウィンドウクラスの作成
    ImGuiWindowClass window_class;
    // ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定）
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
    ImGui::SetNextWindowClass(&window_class);

    // ウィンドウが開いている
    if (m_show)
    {
        if (!firstWindowFlg_)
        {
            // デスクトップのサイズを取得
            int DesktopW, DesktopH;
            GetDefaultState(&DesktopW, &DesktopH, NULL);
            // ウィンドウの位置
            ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiCond_Once);
            ///ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
            // ウィンドウの大きさ
            ImGui::SetNextWindowSize(ImVec2(static_cast<float>(DesktopW / 2 + DesktopW / 3), static_cast<float>(DesktopH / 2 + DesktopH / 3)), ImGuiCond_Once);
            ///ImGui::SetNextWindowSize(ImVec2(DesktopW, DesktopH), ImGuiCond_FirstUseEver);
            firstWindowFlg_ = true;
        }
        
        // ウィンドウの作成（ウィンドウの名前、開いているか、ウィンドウの効果）
        if (ImGui::Begin("##model view", &m_show,ImGui::WindowZOrder::WindowZOrder_Menu, window_flags))
        {
            hwnd_ = (HWND)ImGui::GetMainViewport()->PlatformHandleRaw;

            // メニューバーの作成
            CreateMenuBer();

            // ドッキングエリアの作成
            dokingArea_->Create();

            Inspector();

            // ステージの更新
            stage_->Update();

            // マウスの更新
            mouse_->Update(
                sceneView_->GetImageLeftUpCornor(),
                sceneView_->GetImageRightDownCornor(),
                sceneView_->GetDefaultImageSize());

            models_->Update();

            if (models_->IsModelSelect())
            {
                gizumo_->Update(mouse_->GetScreenMousePoint(), models_->GetModelPos());
            }

            // カメラの更新
            //camera_->Update(mouse_->GetSceneMousePoint().int_cast(), sceneView_->GetScreenSize() / 2, sceneView_->GetWindowCenterPoint());

            

            // オプション項目を開いているかどうか
            if (optionWindowFlg_) { OptionWindow(); };
            ViewWindow();

            // シーンビューの作成
            sceneView_->Create();

            items_->Update();
            fileDialog_->Update(viewFlg_[L"codeEditer"]);

            // ウィンドウの終了
            ImGui::End();
        };
    }
    else
    {
        ImGuiWindowFlags windowFlags = 
            ImGuiWindowFlags_NoTitleBar | 
            ImGuiWindowFlags_NoBackground | 
            ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        ImGui::Begin("Invisible Window",nullptr, windowFlags);
        ImGui::End();
    }

    if (outputWindowFlg_)
    {
        OutputSystemWindow();
    }

    if (!m_show&&CheckHitKey(KEY_INPUT_0))
    {
        m_show = true;
    }

    if (CheckHitKey(KEY_INPUT_ESCAPE))
    {
        std::exit(0);
    }

}

void FreamMng::Draw()
{
    
    // 上から見たカメラに変更
    camera_->Update(mouse_->GetSceneMousePoint().int_cast(), sceneView_->GetScreenSize() / 2, sceneView_->GetWindowCenterPoint());
    // 2Dの上から見たステージの作成
    stage_->PreviewMake();


    // ギズモ用のスクリーン作成
    SetUseLighting(true);
    SetDrawScreen(systemUIScreen_);
    ClearDrawScreen();
    camera_->Set();
    gizumo_->Draw();

    // スクリーン作成
    {
        SetDrawScreen(screen_);
        ClearDrawScreen();
        camera_->Set();

        int ww, wh;
        GetWindowSize(&ww, &wh);
        // 背景
        DrawBox(0, 0, ww, wh, GetColor(60, 60, 60), true);
        stage_->Draw();

        ObjectDrawField();
        if (models_->IsModelSelect())
        {
            DrawGraph(0, 0, systemUIScreen_, true);
        }

        mouse_->Draw();
    }

    for (int i = 0; i < CUBE_TEXTURE_NUM; i++)
    {
        SetRenderTargetToShader(0, cubeTexture_, i);
        ClearDrawScreen();
        camera_->Set();
        models_->DrawSkyDome();
    }

    SetDrawScreen(DX_SCREEN_BACK);
    RefreshDxLibDirect3DSetting();
    ClearDrawScreen();

    postEffect_->Draw(screen_);

    camera_->Set();
    
}

void FreamMng::Render()
{
    ImGuiContext* ctx = ImGui::GetCurrentContext();
    std::stable_sort(ctx->Windows.begin(), ctx->Windows.end(),
        [](const ImGuiWindow* a, const ImGuiWindow* b) {
            return a->BeginOrderWithinContext < b->BeginOrderWithinContext;
        }
    );


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

bool FreamMng::GetShowWindow()
{
    return m_show;
}

void FreamMng::SysNewFream()
{
    ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
   
    
	ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.NavInputs[ImGuiNavInput_DpadUp] = 0.0f;
    io.NavInputs[ImGuiNavInput_DpadDown] = 0.0f;
    io.NavInputs[ImGuiNavInput_DpadLeft] = 0.0f;
    io.NavInputs[ImGuiNavInput_DpadRight] = 0.0f;


   
}

void FreamMng::Style()
{
	// ImGui初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\WINDOWS\\FONTS\\MEIRYO.TTC", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// 全てのウィンドウでのビューポートを表示
	io.ConfigViewportsNoAutoMerge = true;

	// iniファイルの吐き出しを止める
	io.IniFilename = NULL;
	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
        float rounding = 0.5f;

		//　ウィンドウの丸み
        style.WindowRounding = rounding;
        style.ChildRounding = rounding;
        style.FrameRounding = rounding;
        style.PopupRounding = rounding;
        style.ScrollbarRounding = rounding;
        style.GrabRounding = rounding;
        style.LogSliderDeadzone = rounding;
        style.TabRounding = rounding;

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

    models_->CustomStatus();

    ImGui::End();
}

void FreamMng::CreateMenuBer()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding.y = 8.0f;  // メニューバーの上下の余白の高さを調整
    style.FrameRounding = 1.0f;  // メニューバーの角の丸みを無効化

    // メニューバーの作成
    if (ImGui::BeginMenuBar())
    {
        // メニュー1
        if (ImGui::BeginMenu("File"))
        {
            // メニューの中身
            if (ImGui::MenuItem("new data"))
            {
                auto id = MessageBoxA(NULL, "Do you want to save the current data?", "SAVE", MB_OKCANCEL);
                if (id == IDOK)
                {
                    // 全初期化
                    models_.release();
                    camera_.release();
                    stage_.release();
                    postEffect_.release();
                    lpShaderMng.Clear();
                    stage_ = std::make_unique<Fream_Stage>();
                    camera_ = std::make_unique<Fream_Camera>();
                    models_ = std::make_unique<Fream_Model>();
                    postEffect_ = std::make_unique<PostEffectMng>();
                }
            }
            // メニューの中身
            if (ImGui::BeginMenu("input data"))
            {
                inputWindowFlg_ = true;
                ImGui::EndMenu();
            };
            if (ImGui::BeginMenu("output data"))
            {
                if (ImGui::BeginMenu("model"))
                {
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("singleData")) {};
                if (ImGui::MenuItem("allData")) {};
                ImGui::Separator();
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
            ImGui::EndMenu();
        };
        // メニュー3
        if (ImGui::BeginMenu("View")) {
            for (auto& view : viewFlg_)
            {
                ImGui::MenuItem(Utility::WideStringToString(view.first).c_str(), NULL, &view.second);
            }
            ImGui::EndMenu();
        };
        // メニュー4
        if (ImGui::BeginMenu("Project")) { ImGui::EndMenu(); };
        // メニュー5
        if (ImGui::BeginMenu("Desktop")) { ImGui::EndMenu(); };
        // メニュー6
        if (ImGui::BeginMenu("Help")) { 
            std::string app = "start chrome.exe ";
            std::filesystem::path result = std::filesystem::absolute("data/html/help.html");
            std::string path = result.string();
            if(ImGui::Button("ImGui Wiki")) { system("start chrome.exe https://github.com/ocornut/imgui/wiki"); }
            //if(ImGui::Button("help page")) { system((app+path).c_str()); }
            ImGui::EndMenu(); };

        // カスタムのボタンを追加
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 20.0f - 25.0f);
        if (ImGui::Button("x", ImVec2(25.0f, ImGui::GetFrameHeight())))
        {
            std::exit(0);
        }
        if (windowMaxFlag_)
        {
            ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 20.0f - 55.0f);
            if (ImGui::Button(u8"---", ImVec2(25.0f, ImGui::GetFrameHeight())))
            {
                windowMaxFlag_ = false;
                windowMinFlag_ = true;
            }
        }
        else
        {
            ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 20.0f - 55);
            if (ImGui::Button(u8"口", ImVec2(25.0f, ImGui::GetFrameHeight())))
            {
                if (!windowMaxFlag_)
                {
                    copyWindowSize_ = { ImGui::GetWindowSize().x,ImGui::GetWindowSize().y };
                    copyWindowPos_ = { ImGui::GetWindowPos().x,ImGui::GetWindowPos().y };
                }
                windowMaxFlag_ = true;
                windowMinFlag_ = false;
            }
        }
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 20.0f - 85.0f);
        if (ImGui::Button(u8"ー", ImVec2(25.0f, ImGui::GetFrameHeight())))
        {
            m_show = false;
        };
        // メニューバーの終了
        ImGui::EndMenuBar();
    }
}

void FreamMng::ObjectDrawField()
{
    SetUseLighting(false);


    models_->DrawSkyDome();


    models_->Draw(cubeTexture_);
    //gizumo_->Draw();

}

void FreamMng::OutputSystemWindow()
{
}

void FreamMng::InputSystemWindow()
{
    ImGui::Begin("input");
    ImGui::End();
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
            if (ImGui::BeginTabItem("PostEffect"))
            {
                postEffect_->Custom();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("skyDome"))
            {
                models_->CustomSkyDome();

                ImGui::EndTabItem();
            }


            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}

void FreamMng::ViewWindow()
{
    for (auto& view : viewFlg_)
    {
        if (view.first == L"console")
        {
            if (view.second)
            {
                console_->Update();
            }
        }
    }
}
