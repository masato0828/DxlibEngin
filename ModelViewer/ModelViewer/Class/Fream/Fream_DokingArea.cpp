#include "Fream_DokingArea.h"
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_internal.h"
#include "../Common/ImGuiMyCustom.h"

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
    ImGuiID dockspace_id = ImGui::GetID("my_dockspace");
    if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
    {
        ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node

        ImGuiID dock_main_id = dockspace_id;
        // è„Ç…èoÇµÇΩÇ¢ÇÃÇ≈àÍî‘è„
        ImGuiID dock_id_Inspector = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 1.f, NULL, &dock_main_id);
        ImGuiID dock_id_Project = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 1.f, NULL, &dock_main_id);
        ImGuiID dock_id_Items = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 1.f, NULL, &dock_main_id);

        ImGui::DockBuilderSetNodeSize(dock_main_id, ImVec2(300, 200));
        ImGui::DockBuilderSetNodeSize(dock_id_Project, ImVec2(500, 350));
        ImGui::DockBuilderSetNodeSize(dock_id_Items, ImVec2(300, 200));
        ImGui::DockBuilderSetNodeSize(dock_id_Inspector, ImVec2(300, 500));

        ImGui::DockBuilderDockWindow("Inspector", dock_id_Inspector);
        ImGui::DockBuilderDockWindow("scene", dock_main_id);
        ImGui::DockBuilderDockWindow("Items", dock_id_Items);
        ImGui::DockBuilderDockWindow("Project", dock_id_Project);

        ImGui::DockBuilderFinish(dockspace_id);
    }
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
};
