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

            // �E�B���h�E�̏I��
            ImGui::End();
        }
    }
    else
    {
        std::exit(0);
    }

    // �摜�T�C�Y
    static int my_image_width = 0;
    static int my_image_height = 0;
    // �V�F�[�_���̍쐬
    static ID3D11ShaderResourceView* my_shaderData = NULL;
    // �摜�̓ǂݍ���
    bool ret = LoadBackGroundTextureFromFile(&my_shaderData, &my_image_width, &my_image_height);

    //GetMousePoint(&mouseX, &mouseY);

    // �E�B���h�E�̕\��
    if (ImGui::Begin("scene"))
    {
        auto sizeX = ImGui::GetWindowSize().x;
        auto sizeY = ImGui::GetWindowSize().y;

        // ������
        auto x = ImGui::GetWindowSize().x / my_image_width;
        // �c����
        auto y = ImGui::GetWindowSize().y / my_image_height;


        // �W��
        auto factor = (std::min)(x, y);

        if (factor == 0)
        {
            ImGui::End();
            return;
        }

        ImGui::Image(
            // �摜���
            (void*)my_shaderData,
            // �摜�T�C�Y�������ŕς���
            ImVec2{ my_image_width * factor, my_image_height * factor },
            // UV1
            ImVec2{ 0,0 },
            // UV2
            ImVec2{ 1,1 },
            // �`��J���[
            ImVec4{ 1,1,1,1 },
            // �g�̃J���[
            ImVec4{ 0,0,0,0 });

        // �V�[���r���[�̃}�E�X�̈ʒu���v�Z
        //sceneViewMousePoint_ = SceneMouseCreate(factor);

       // ImGui::Begin("mouseDebug");
       // ImGui::Text("sceneViewMousePoint = {%f:%f}", sceneViewMousePoint_.x_, sceneViewMousePoint_.y_);
       // ImGui::End();

       // sceneViewMousePoint_.x_ *= my_image_width;
       // sceneViewMousePoint_.y_ *= my_image_height;
        //imageSize_ = { static_cast<float>(my_image_width * factor),static_cast<float>(my_image_height * factor) };

        // �V�[���r���[�̃T�C�Y
        /*sceneViewSize_ = {
            ImGui::GetWindowDrawList()->VtxBuffer[0].pos.x + my_image_width * factor,
             ImGui::GetWindowDrawList()->VtxBuffer[0].pos.y + my_image_height * factor };*/

        // �E�B���h�E�̏I��
        ImGui::End();
    }


	// �f���E�B���h�E
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
		// �w��E�B���h�E�i�����ł͔w�i�j�̕ύX
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
	}
}
