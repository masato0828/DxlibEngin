#pragma once
#include "FreamBase.h"
#include "../Common/Vector2.h"
#include <string>
#include <filesystem>
#include <map>

class Fream_SceneView :
    public FreamBase
{
public:
    Fream_SceneView();
    ~Fream_SceneView();
    void Init() override;
    void Update() override;
    /// <summary>
    /// �E�B���h�E�̍쐬
    /// </summary>
    void Create();

    /// <summary>
    /// ��ʊ����̌W���擾
    /// </summary>
    /// <returns>�W��</returns>
    float GetFactor();
    
    /// <summary>
    /// �摜�T�C�Y�擾
    /// </summary>
    /// <returns>�摜�T�C�Y</returns>
    Vector2 GetDefaultImageSize();
    
    /// <summary>
    /// �摜�̍���̎擾
    /// </summary>
    /// <returns>��ʂ̍���</returns>
    Vector2Flt GetImageLeftUpCornor();

    /// <summary>
    /// �摜�̉E���̎擾
    /// </summary>
    /// <returns>��ʂ̉E��</returns>
    Vector2Flt GetImageRightDownCornor();

    /// <summary>
    /// �E�B���h�E�̐^�񒆈ʒu�̎擾
    /// </summary>
    /// <returns>�E�B���h�E�̒����ʒu</returns>
    Vector2 GetWindowCenterPoint();

    /// <summary>
    /// �E�B���h�E�̃T�C�Y�̎擾
    /// </summary>
    /// <returns>�E�B���h�E�̃T�C�Y</returns>
    Vector2Flt GetScreenSize();

    /// <summary>
    /// �h���b�O�A���h�h���b�v�̃p�X�擾
    /// </summary>
    /// <returns>�p�X</returns>
    std::filesystem::path GetDropModelPath();

private:

    // ��ʊ����W��
    float factor_;

    // �摜�T�C�Y
    Vector2 defaultImageSize_;

    // �E�B���h�E���̉摜�̍���
    Vector2Flt imageLeftUpCornor_;
    // �E�B���h�E���̉摜�̉E��
    Vector2Flt imageRightDownCornor_;

    // �X�N���[���̃T�C�Y
    Vector2Flt screenSize_;

    // �h���b�O�A���h�h���b�v�̃p�X�擾�p
    std::filesystem::path dropModelPath_;

    // �t�@�C�����[�h���s������
    bool is_FileLoad_;

    /// <summary>
   /// �h���b�O�A���h�h���b�v���Ă����f�[�^���擾
   /// </summary>
    void CreateDragAndDropHandle();
};

