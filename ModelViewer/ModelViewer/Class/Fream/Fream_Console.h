#pragma once
#include <string>
#include "FreamBase.h"





/// <summary>
/// コンソールにテキストを追加する関数
/// </summary>
/// <param name="text">出力テキスト内容</param>
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

