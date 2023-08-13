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

    void Update() override;
private:
    void Init() override;
    

    void ConsoleWindow();
    
    
};

