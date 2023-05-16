#include "Fream_SceneView.h"
#include <vector>
#include <Dxlib.h>
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_impl_dx11.h"
#include "../../imGui/imgui_impl_win32.h"
#include "../System/ImguiImageStb.h"
#include "../../imGui/imgui_internal.h"
#include "../Common/ImGuiMyCustom.h"

Fream_SceneView::Fream_SceneView()
{
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
    
    // ウィンドウの表示
    if (ImGui::Begin("scene"))
    {
        //ImGuiCustom::NoCenterDoking();

        // シェーダ情報の作成
        static ID3D11ShaderResourceView* my_shaderData = NULL;
        // 画像の読み込み
        bool ret = LoadBackGroundTextureFromFile(&my_shaderData, &defaultImageSize_.x_, &defaultImageSize_.y_);

        auto sizeX = ImGui::GetWindowSize().x;
        auto sizeY = ImGui::GetWindowSize().y;

        // 横割合
        auto x = ImGui::GetWindowSize().x / defaultImageSize_.x_;
        // 縦割合
        auto y = ImGui::GetWindowSize().y / defaultImageSize_.y_;


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
            ImVec2{ defaultImageSize_.x_ * factor, defaultImageSize_.y_ * factor },
            // UV1
            ImVec2{ 0,0 },
            // UV2
            ImVec2{ 1,1 },
            // 描画カラー
            ImVec4{ 1,1,1,1 },
            // 枠のカラー
            ImVec4{ 0,0,0,0 });

        factor_ = factor;
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

        // ウィンドウの終了
        ImGui::End();
    }
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
