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

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;
private:
    /// <summary>
    /// 初期化
    /// </summary>
    void Init() override;
    
    /// <summary>
    /// コンソールウィンドウの作成
    /// </summary>
    void ConsoleWindow();
    
    
};

