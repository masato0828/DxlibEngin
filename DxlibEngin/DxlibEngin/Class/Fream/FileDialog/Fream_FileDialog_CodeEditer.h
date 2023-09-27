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
    /// 初期化
    /// </summary>
    void Init() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="filePath">ファイルパス</param>
    void Update(std::filesystem::path filePath);

private:
    // エディター操作
    TextEditor editor_;

    // テキストの読み込み状態フラグ
    bool isText_;
    
    // 前回のコード保持
    std::string beforCode_;
};

