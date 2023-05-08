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

    // くっつけるスペースの作成のための変数
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // ウィンドウの効果
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoResize;

    // バックグラウンドの表示を行うか
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Imgui用ウィンドウクラスの作成
    ImGuiWindowClass window_class;
    // ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);
    //ImGui::SetNextWindowSize(ImVec2{500,500}, ImGuiCond_Once);// ImGuiCond_FirstUseEver

    // ウィンドウの作成
    if (ImGui::Begin("##d", NULL, window_flags))
    {
        // ドッキングスペースの送信
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // ウィンドウの終了
        ImGui::End();
    }
}
