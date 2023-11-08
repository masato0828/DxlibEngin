#pragma once
#include <string>
#include "FreamBase.h"

/// <summary>
/// �R���\�[���Ƀe�L�X�g��ǉ�����֐�
/// </summary>
/// <param name="text">�o�̓e�L�X�g���e</param>
void AddConsoleText(const std::string& text);

class Fream_Console :
    public FreamBase
{
public:
    Fream_Console();
    ~Fream_Console();

    /// <summary>
    /// �X�V
    /// </summary>
    void Update() override;
private:
    /// <summary>
    /// ������
    /// </summary>
    void Init() override;
    
    /// <summary>
    /// �R���\�[���E�B���h�E�̍쐬
    /// </summary>
    void ConsoleWindow();
    
    
};

