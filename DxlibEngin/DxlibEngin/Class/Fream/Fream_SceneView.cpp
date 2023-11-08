#include "Fream_SceneView.h"
#include <vector>
#include <Dxlib.h>
#include <filesystem>
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_impl_dx11.h"
#include "../../imGui/imgui_impl_win32.h"
#include "../System/ImguiImageStb.h"
#include "../../imGui/imgui_internal.h"
#include "../Common/ImGuiMyCustom.h"
#include "../Common/Utility.h"
#include "../../imGui/guizmo/ImGuizmo.h"

Fream_SceneView::Fream_SceneView()
{
    Init();
}

Fream_SceneView::~Fream_SceneView()
{
}

void Fream_SceneView::Init()
{
    
}

void Fream_SceneView::Update()
{
}


void Fream_SceneView::Create()
{
    // Imgui用ウィンドウクラスの作成
    ImGuiWindowClass window_classview;
    // ウィンドウの効果の編集（今回はウィンドウの非表示を無くす設定とウィンドウタブを無くす処理）
    window_classview.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
    ImGui::SetNextWindowClass(&window_classview);

    ImGuiIO& io = ImGui::GetIO();
    static ImGuiWindowFlags gizmoWindowFlags = 0;
    // ウィンドウの表示
    ImGui::Begin("scene", 0, gizmoWindowFlags);
    // シェーダ情報の作成
    static ID3D11ShaderResourceView* my_shaderData = NULL;
    // 画像の読み込み
    bool ret = LoadBackGroundTextureFromFile(&my_shaderData, &defaultImageSize_.x_, &defaultImageSize_.y_);

    auto sizeX = ImGui::GetWindowSize().x;
    auto sizeY = ImGui::GetWindowSize().y;

    // 横割合
    auto rectX = ImGui::GetWindowDrawList()->GetClipRectMax().x - ImGui::GetWindowDrawList()->GetClipRectMin().x;
    auto rectY = ImGui::GetWindowDrawList()->GetClipRectMax().y - ImGui::GetWindowDrawList()->GetClipRectMin().y - 20;
    //auto x = ImGui::GetWindowSize().x / defaultImageSize_.x_;
    auto x = rectX / defaultImageSize_.x_;
    // 縦割合
    //auto y = ImGui::GetWindowSize().y / defaultImageSize_.y_;
    auto y = rectY / defaultImageSize_.y_;


    // 係数
    auto factor = (std::min)(x, y);

    if (factor == 0)
    {
        ImGui::End();
        return;
    }
    factor_ = factor;
    ImGui::Image(
        // 画像情報
        (void*)my_shaderData,
        // 画像サイズを割合で変える
        ImVec2{ (float)defaultImageSize_.x_ * factor_, (float)defaultImageSize_.y_ * factor_ },
        // UV1
        ImVec2{ 0,0 },
        // UV2
        ImVec2{ 1,1 },
        // 描画カラー
        ImVec4{ 1,1,1,1 },
        // 枠のカラー
        ImVec4{ 0,0,0,0 });


    // このウィンドウの画像の左上
    imageLeftUpCornor_ = {
        ImGui::GetWindowDrawList()->VtxBuffer[0].pos.x,
        ImGui::GetWindowDrawList()->VtxBuffer[0].pos.y
    };
    // このウィンドウの画像の右下
    imageRightDownCornor_ = {
        ImGui::GetWindowDrawList()->VtxBuffer[2].pos.x,
        ImGui::GetWindowDrawList()->VtxBuffer[2].pos.y
    };

    CreateDragAndDropHandle();

    
    ImGuizmo::BeginFrame();
    ImGuizmo::SetDrawlist();
    /*float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
    viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
    viewManipulateTop = ImGui::GetWindowPos().y;*/
    
    float windowWidth = imageRightDownCornor_.x_;
    float windowHeight = imageRightDownCornor_.y_;
    ImGuizmo::SetRect(imageLeftUpCornor_.x_, imageLeftUpCornor_.y_, windowWidth, windowHeight);
    float viewManipulateRight = imageLeftUpCornor_.x_ + windowWidth;
    float viewManipulateTop = imageLeftUpCornor_.y_;

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;

    auto cameraview = GetCameraViewMatrix();
    auto cameraprojection = GetCameraProjectionMatrix();

    static const float identityMatrix[16] =
    { 1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f };
    ImGuizmo::DrawGrid((float*)&cameraview, (float*)&cameraprojection, identityMatrix, 100.f);

    // ウィンドウの終了
    ImGui::End();
}

float Fream_SceneView::GetFactor()
{
    return factor_;
}

Vector2 Fream_SceneView::GetDefaultImageSize()
{
    return defaultImageSize_;
}

Vector2Flt Fream_SceneView::GetImageLeftUpCornor()
{
    return imageLeftUpCornor_;
}

Vector2Flt Fream_SceneView::GetImageRightDownCornor()
{
    return imageRightDownCornor_;
}

Vector2 Fream_SceneView::GetWindowCenterPoint()
{
    RECT cR;
    GetWindowClientRect(&cR);

    Vector2Flt imageRightDown = {
        imageRightDownCornor_.x_ - cR.left,
        imageRightDownCornor_.y_ - cR.top
    };
    Vector2Flt imageLeftUp = {
        imageLeftUpCornor_.x_ - cR.left,
        imageLeftUpCornor_.y_ - cR.top
    };

    screenSize_ = {
       imageRightDown.x_ - imageLeftUp.x_,
       imageRightDown.y_ - imageLeftUp.y_
    };

    Vector2Flt windowHlfeSize_ = screenSize_ / 2.f;
    Vector2 correctWindowLeftTop_ = imageLeftUp.int_cast();
    Vector2 centerPoint = correctWindowLeftTop_ + windowHlfeSize_.int_cast();

    return centerPoint;
}

Vector2Flt Fream_SceneView::GetScreenSize()
{
    return screenSize_;
}

void Fream_SceneView::CreateDragAndDropHandle()
{
    // ドラッグアンドドロップしてきたデータを取得
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEST"))
        {
            // ドラッグアンドドロップのデータを取得
            auto payload_n = static_cast<const wchar_t*>(payload->Data);

            // ファイルパスに変更
            dropModelPath_ = payload_n;

            is_FileLoad_ = true;
        }
        ImGui::EndDragDropTarget();
    }
}

std::filesystem::path Fream_SceneView::GetDropModelPath()
{
    if (is_FileLoad_)
    {
        is_FileLoad_ = false;
        return dropModelPath_;
    }
    else
    {
        return "";
    }
    
}
