#pragma once
#include "../FreamBase.h"
#include <string>
#include <filesystem>
#include "../../../imGui/ImGuiColorTextEdit/TextEditor.h"
class Fream_FileDialog_CodeEditer :
    public FreamBase
{
public:
    Fream_FileDialog_CodeEditer();
    ~Fream_FileDialog_CodeEditer();
    
    /// <summary>
    /// ������
    /// </summary>
    void Init() override;

    /// <summary>
    /// �X�V
    /// </summary>
    void Update() override;

    /// <summary>
    /// �X�V
    /// </summary>
    /// <param name="filePath">�t�@�C���p�X</param>
    void Update(std::filesystem::path filePath);

private:
    // �G�f�B�^�[����
    TextEditor editor_;

    // �e�L�X�g�̓ǂݍ��ݏ�ԃt���O
    bool isText_;
    
    // �O��̃R�[�h�ێ�
    std::string beforCode_;
};

