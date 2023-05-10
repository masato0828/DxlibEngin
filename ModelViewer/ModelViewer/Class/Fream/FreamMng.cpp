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

    sceneView_ = std::make_unique<Fream_SceneView>();
    dokingArea_ = std::make_unique<Fream_DokingArea>();
    stage_ = std::make_unique<Fream_Stage>();
    camera_ = std::make_unique<Fream_Camera>();
    mouse_ = std::make_unique<Device_Mouse>();
    optionWindowFlg_ = true;
}

void FreamMng::Update()
{
	SysNewFream();

	// SceneViewPort����
     // ����{�^���p
    static bool m_show = true;

    // �E�B���h�E����
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    window_flags |=
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    // Imgui�p�E�B���h�E�N���X�̍쐬
    ImGuiWindowClass window_class;
    // �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�j
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
    ImGui::SetNextWindowClass(&window_class);

    // �E�B���h�E���J���Ă���
    if (m_show)
    {
        // �f�X�N�g�b�v�̃T�C�Y���擾
        int DesktopW, DesktopH;
        GetDefaultState(&DesktopW, &DesktopH, NULL);
        // �E�B���h�E�̈ʒu
        //ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ///ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
        // �E�B���h�E�̑傫��
        //ImGui::SetNextWindowSize(ImVec2(DesktopW/2, DesktopH/2), ImGuiCond_Once);
        ///ImGui::SetNextWindowSize(ImVec2(DesktopW, DesktopH), ImGuiCond_FirstUseEver);

        // �E�B���h�E�̍쐬�i�E�B���h�E�̖��O�A�J���Ă��邩�A�E�B���h�E�̌��ʁj
        if (ImGui::Begin("model view", &m_show, window_flags))
        {
            // ���j���[�o�[�̍쐬
            CreateMenuBer();

            // �E�B���h�E�̏I��
            ImGui::End();
        }
        Inspector();

        dokingArea_->Create();
    }
    else
    {
        std::exit(0);
    }

    stage_->Update();
    auto mousePoint = mousePoint_.Create({
        sceneView_->GetDefaultImageSize(),
        sceneView_->GetImageLeftUpCornor(),
        sceneView_->GetImageRightDownCornor() });

    RECT cR;
    GetWindowClientRect(&cR);

    Vector2Flt imageRightDown = {
        sceneView_->GetImageRightDownCornor().x_-cR.left,
        sceneView_->GetImageRightDownCornor().y_ - cR.top
    };
    Vector2Flt imageLeftUp = {
        sceneView_->GetImageLeftUpCornor().x_-cR.left,
        sceneView_->GetImageLeftUpCornor().y_ - cR.top
    };

    Vector2Flt windowSize = {
       imageRightDown.x_ - imageLeftUp.x_,
       imageRightDown.y_ - imageLeftUp.y_
    };

    camera_->Update(mousePoint, windowSize / 2.f, imageLeftUp);
    
    if (optionWindowFlg_) { OptionWindow(); };
   
    mouse_->Update(
        sceneView_->GetImageLeftUpCornor(),
        sceneView_->GetImageRightDownCornor(), 
        sceneView_->GetDefaultImageSize());

    // �V�[���r���[�̍쐬
    sceneView_->Create();

	// �f���E�B���h�E
	ImGui::ShowDemoWindow();

    
}

void FreamMng::Draw()
{
    camera_->Set();
    stage_->Draw();
    DrawMousePoint();
    //mouse_->Draw();
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
	// ImGui������
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// �S�ẴE�B���h�E�ł̃r���[�|�[�g��\��
	io.ConfigViewportsNoAutoMerge = true;

	// ini�t�@�C���̓f���o�����~�߂�
	//io.IniFilename = NULL;
	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//�@�E�B���h�E�̊ۂ�
		style.WindowRounding = 1.0f;

        style.WindowRounding = 12.0f;
        style.ChildRounding = 12.0f;
        style.FrameRounding = 12.0f;
        style.PopupRounding = 12.0f;
        style.ScrollbarRounding = 12.0f;
        style.GrabRounding = 12.0f;
        style.LogSliderDeadzone = 12.0f;
        style.TabRounding = 12.0f;

		// �w��E�B���h�E�i�����ł͔w�i�j�̕ύX
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
	}

    ImGui::StyleColorsClassic();
}

void FreamMng::Inspector()
{
    ImGui::Begin("Inspector");
    ImGui::End();
}


void FreamMng::DrawMousePoint()
{
    //DrawCircleAA(sceneViewMousePoint_.x_, sceneViewMousePoint_.y_,10,100,0xff0000);

    auto mousePoint = mousePoint_.Create({
        sceneView_->GetDefaultImageSize(),
        sceneView_->GetImageLeftUpCornor(),
        sceneView_->GetImageRightDownCornor() });

    Vector2Flt reductionScreenSize_ = sceneView_->GetImageRightDownCornor() - sceneView_->GetImageLeftUpCornor() ;
    Vector2Flt defaultScreenSize = { sceneView_->GetDefaultImageSize().x_, sceneView_->GetDefaultImageSize().y_ };
    mousePoint /= reductionScreenSize_;
    mousePoint *= defaultScreenSize;

    DrawCircleAA(
        mousePoint.x_,
        mousePoint.y_,
        10, 100, 0xff0000);
}

void FreamMng::CreateMenuBer()
{
    static bool open = false;
    // ���j���[�o�[�̍쐬
    if (ImGui::BeginMenuBar())
    {
        // ���j���[1
        if (ImGui::BeginMenu("File"))
        {
            // ���j���[�̒��g
            if (ImGui::MenuItem("new data"))
            {
                // 
                {
                    auto id = MessageBoxA(NULL, "Do you want to save the current data?", "SAVE", MB_OKCANCEL);
                    if (id == IDOK)
                    {
                        // �S������
                    }
                }
            }
            // ���j���[�̒��g
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
        // ���j���[2
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
        // ���j���[3
        if (ImGui::BeginMenu("View")) { ImGui::EndMenu(); };
        // ���j���[4
        if (ImGui::BeginMenu("Project")) { ImGui::EndMenu(); };
        // ���j���[5
        if (ImGui::BeginMenu("Desktop")) { ImGui::EndMenu(); };
        // ���j���[6
        if (ImGui::BeginMenu("Help")) { ImGui::EndMenu(); };

        // ���j���[�o�[�̏I��
        ImGui::EndMenuBar();
    }

    if (open)
    {
        ImGui::Begin("single export");/*
        if (ImGui::Button("export"))
        {
            auto fileName = FilePathErase()(
                FileSeve(nullptr, "json", "json�t�@�C��(*.json)\0 * .json\0")
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
    ImGui::Begin("Option");
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
