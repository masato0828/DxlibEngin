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

    // �h�b�L���O�X�y�[�X�̑��M
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
};
