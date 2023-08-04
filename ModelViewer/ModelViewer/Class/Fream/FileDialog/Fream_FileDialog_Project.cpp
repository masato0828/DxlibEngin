#include "Fream_FileDialog_Project.h"
#include "../../../imGui/imgui.h"
#include "../../../imGui/ImGuiFileDialog.h"
#include "../../../imGui/CustomFont.h"
#include "../../Common/FileDialog.h"
#include "../../../imGui/imgui_internal.h"
#include "../../Common/ImGuiMyCustom.h"

Fream_FileDialog_Project::Fream_FileDialog_Project()
{
}

Fream_FileDialog_Project::~Fream_FileDialog_Project()
{
}

void Fream_FileDialog_Project::Init()
{
}

void Fream_FileDialog_Project::Update()
{

	if (ImGui::Begin("Project",0, ImGuiDockNodeFlags_NoDockingSplitMe))
	{
		// 強制ドッキング設定
		ImGuiID dockspace_id = ImGui::GetID("projectDock");
		if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
		{
			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace| ImGuiDockNodeFlags_NoDockingSplitMe); // Add empty node

			ImGuiID dock_main_id = dockspace_id;
			ImGuiID dock_id_FileDialog = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 1.f, NULL, &dock_main_id);

			ImGui::DockBuilderSetNodeSize(dock_main_id, ImVec2(300, 200));
			ImGui::DockBuilderSetNodeSize(dock_id_FileDialog, ImVec2(293, 200));

			ImGui::DockBuilderDockWindow("FileTree", dock_id_FileDialog);
			ImGui::DockBuilderDockWindow("##ItemWindow", dock_main_id);

			ImGui::DockBuilderFinish(dockspace_id);
		}
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		ImGui::End();
	}
}
