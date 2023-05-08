#include "Fream_DokingArea.h"
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_internal.h"

Fream_DokingArea::Fream_DokingArea()
{
}

Fream_DokingArea::~Fream_DokingArea()
{
}

void Fream_DokingArea::Init()
{
}

void Fream_DokingArea::Update()
{
}

void Fream_DokingArea::Create()
{

    // ��������X�y�[�X�̍쐬�̂��߂̕ϐ�
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // �E�B���h�E�̌���
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoResize;

    // �o�b�N�O���E���h�̕\�����s����
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Imgui�p�E�B���h�E�N���X�̍쐬
    ImGuiWindowClass window_class;
    // �E�B���h�E�̌��ʂ̕ҏW�i����̓E�B���h�E�̔�\���𖳂����ݒ�ƃE�B���h�E�^�u�𖳂��������j
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);
    //ImGui::SetNextWindowSize(ImVec2{500,500}, ImGuiCond_Once);// ImGuiCond_FirstUseEver

    // �E�B���h�E�̍쐬
    if (ImGui::Begin("##d", NULL, window_flags))
    {
        // �h�b�L���O�X�y�[�X�̑��M
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // �E�B���h�E�̏I��
        ImGui::End();
    }
}
