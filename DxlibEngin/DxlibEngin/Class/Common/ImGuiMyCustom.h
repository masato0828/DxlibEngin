#pragma once
#include "../Common/Geometry.h"
#include <string>
#include <Dxlib.h>
#include "../../imGui/imgui.h"

namespace ImGui
{
    /// <summary>
    /// 3D���W�J�X�^��
    /// </summary>
    /// <param name="label">�^�C�g��</param>
    /// <param name="vec3">3D���W</param>
    /// <param name="v_speed">�J�X�^���X�s�[�h</param>
    /// <param name="v_min">�h���b�O�̍ŏ�</param>
    /// <param name="v_max">�h���b�O�̍ő�</param>
    /// <param name="format">�\��(%.3f)</param>
    /// <param name="flags">ImGuiSliderFlags</param>
    /// <returns>�G��Ă���Ftrue �@�G��Ă��Ȃ��Ffalse </returns>
    IMGUI_API bool  DragFloat3(const char* label, Vector3* vec3, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    
    // 2D���W�J�X�^��
    IMGUI_API bool  DragInt2(const char* label, Vector2* vec2, int v_speed = 1, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool  DragFloat2(const char* label, Vector2Flt* vec2, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool  DragDouble2(const char* label, Vector2Dbl* vec2, double v_speed = 1.0, double v_min = 0.0, double v_max = 0.0, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    enum WindowZOrder {
        WindowZOrder_Chat,
        WindowZOrder_Scoreboard,
        WindowZOrder_Menu,
        WindowZOrder_Console,
    };
    bool Begin(const char* name, bool* p_open,WindowZOrder z_order, ImGuiWindowFlags flags);
  
}

// �J�X�^���@����
namespace ImGuiCustom
{
    struct IM_COLOR
    {
        float red;
        float green;
        float blue;
        float alpha;
    };

    // �F�̕ύX�J�X�^��
    IMGUI_API bool  ColorEdit3(const char* label, IM_COLOR* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit3(const char* label, COLOR_F* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit3(const char* label, FLOAT4* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit4(const char* label, IM_COLOR* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit4(const char* label, COLOR_F* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool  ColorEdit4(const char* label, FLOAT4* col, ImGuiColorEditFlags flags = 0);

    /// <summary>
   /// �E�B���h�E��^�񒆂ւ̃h�b�L���O�𖳌��ɂ���
   /// </summary>
    void NoCenterDoking();

    /// <summary>
    /// �w���p�[�e�L�X�g(?)�̍쐬
    /// </summary>
    /// <param name="desc">�e�L�X�g�Ƃ��ē��͂���������</param>
    void HelpMarker(const char* desc);

    /// <summary>
    /// ImGui�̊e�F�̕ύX
    /// </summary>
    /// <param name="colType">ImGuiCol</param>
    /// <param name="colorStyle">�F</param>
    void SetCustomStyle(ImGuiCol colType, ImVec4 colorStyle);
}
