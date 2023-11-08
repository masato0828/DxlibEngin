#pragma once
#include "FreamBase.h"
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include "../System/ImguiImageStb.h"
#include "../Common/Vector2.h"
#include "FileDialog\FileCommon.h"
#include "FileDialog\Fream_FileDialog_Tree.h"
#include "FileDialog\Fream_FileDialog_Project.h"
#include "FileDialog\Fream_FileDialog_Item.h"
#include "../../imGui/ImGuiColorTextEdit/TextEditor.h"
#include "FileDialog\Fream_FileDialog_CodeEditer.h"

class Fream_FileDialog :
    public FreamBase
{
public:

    Fream_FileDialog();
    ~Fream_FileDialog();
    
    /// <summary>
    /// ������
    /// </summary>
    void Init();

    /// <summary>
    /// �X�V
    /// </summary>
    void Update();

    /// <summary>
    /// �X�V
    /// </summary>
    /// <param name="codeEditerWindowOpenFlg">�E�B���h�E�̕\���t���O</param>
    void Update(bool codeEditerWindowOpenFlg);

    /// <summary>
    /// ���݂̃t�@�C���p�X�̎擾
    /// </summary>
    /// <returns>���݂̃t�@�C���p�X</returns>
    std::filesystem::path GetNowFile();
private:
    // �_�C�A���O�̕\���t���O
    bool fileDialogFlg_;

    // �t�@�C���f�[�^
    FileData fileData_;
    // �I�𒆃t�@�C���f�[�^
    FileData* nowSelect;
    // �I�𒆂̃t�@�C���p�X
    std::filesystem::path nowSelectPath_;
    // �t�@�C���̃t���p�X
    std::filesystem::path fileFullPaht_;
    // �I�𒆂̃t�@�C���̃t���p�X
    std::wstring nowSelectFileName_;
    // ���݂̃t�@�C����
    std::wstring nowSelectFile_;

    std::unique_ptr<Fream_FileDialog_Tree>tree_;
    std::unique_ptr<Fream_FileDialog_Project>project_;
    std::unique_ptr<Fream_FileDialog_Item>item_;
    std::unique_ptr<Fream_FileDialog_CodeEditer>codeEditer_;
};

