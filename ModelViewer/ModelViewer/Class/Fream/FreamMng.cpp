#include <Dxlib.h>
#include <memory>
#include "FreamMng.h"
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_impl_dx11.h"
#include "../../imGui/imgui_impl_win32.h"
#include "../System/ImguiImageStb.h"
#include "../../imGui/imgui_internal.h"

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
        //ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ///ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
        // ウィンドウの大きさ
        //ImGui::SetNextWindowSize(ImVec2(DesktopW/2, DesktopH/2), ImGuiCond_Once);
        ///ImGui::SetNextWindowSize(ImVec2(DesktopW, DesktopH), ImGuiCond_FirstUseEver);

        // ウィンドウの作成（ウィンドウの名前、開いているか、ウィンドウの効果）
        if (ImGui::Begin("model view", &m_show, window_flags))
        {
            // メニューバーの作成

            // ウィンドウの終了
            ImGui::End();
        }
    }
    else
    {
        std::exit(0);
    }

    // 画像サイズ
    static int my_image_width = 0;
    static int my_image_height = 0;
    // シェーダ情報の作成
    static ID3D11ShaderResourceView* my_shaderData = NULL;
    // 画像の読み込み
    bool ret = LoadBackGroundTextureFromFile(&my_shaderData, &my_image_width, &my_image_height);

    //GetMousePoint(&mouseX, &mouseY);

    // ウィンドウの表示
    if (ImGui::Begin("scene"))
    {
        auto sizeX = ImGui::GetWindowSize().x;
        auto sizeY = ImGui::GetWindowSize().y;

        // 横割合
        auto x = ImGui::GetWindowSize().x / my_image_width;
        // 縦割合
        auto y = ImGui::GetWindowSize().y / my_image_height;


        // 係数
        auto factor = (std::min)(x, y);

        if (factor == 0)
        {
            ImGui::End();
            return;
        }

        ImGui::Image(
            // 画像情報
            (void*)my_shaderData,
            // 画像サイズを割合で変える
            ImVec2{ my_image_width * factor, my_image_height * factor },
            // UV1
            ImVec2{ 0,0 },
            // UV2
            ImVec2{ 1,1 },
            // 描画カラー
            ImVec4{ 1,1,1,1 },
            // 枠のカラー
            ImVec4{ 0,0,0,0 });

        // シーンビューのマウスの位置を計算
        //sceneViewMousePoint_ = SceneMouseCreate(factor);

       // ImGui::Begin("mouseDebug");
       // ImGui::Text("sceneViewMousePoint = {%f:%f}", sceneViewMousePoint_.x_, sceneViewMousePoint_.y_);
       // ImGui::End();

       // sceneViewMousePoint_.x_ *= my_image_width;
       // sceneViewMousePoint_.y_ *= my_image_height;
        //imageSize_ = { static_cast<float>(my_image_width * factor),static_cast<float>(my_image_height * factor) };

        // シーンビューのサイズ
        /*sceneViewSize_ = {
            ImGui::GetWindowDrawList()->VtxBuffer[0].pos.x + my_image_width * factor,
             ImGui::GetWindowDrawList()->VtxBuffer[0].pos.y + my_image_height * factor };*/

        // ウィンドウの終了
        ImGui::End();
    }


	// デモウィンドウ
	ImGui::ShowDemoWindow();
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
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// 全てのウィンドウでのビューポートを表示
	io.ConfigViewportsNoAutoMerge = true;

	// iniファイルの吐き出しを止める
	//io.IniFilename = NULL;
	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//　ウィンドウの丸み
		style.WindowRounding = 1.0f;
		// 指定ウィンドウ（ここでは背景）の変更
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
	}
}
