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
    /// 初期化
    /// </summary>
    void Init();

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="codeEditerWindowOpenFlg">ウィンドウの表示フラグ</param>
    void Update(bool codeEditerWindowOpenFlg);

    /// <summary>
    /// 現在のファイルパスの取得
    /// </summary>
    /// <returns>現在のファイルパス</returns>
    std::filesystem::path GetNowFile();
private:
    // ダイアログの表示フラグ
    bool fileDialogFlg_;

    // ファイルデータ
    FileData fileData_;
    // 選択中ファイルデータ
    FileData* nowSelect;
    // 選択中のファイルパス
    std::filesystem::path nowSelectPath_;
    // ファイルのフルパス
    std::filesystem::path fileFullPaht_;
    // 選択中のファイルのフルパス
    std::wstring nowSelectFileName_;
    // 現在のファイル名
    std::wstring nowSelectFile_;

    std::unique_ptr<Fream_FileDialog_Tree>tree_;
    std::unique_ptr<Fream_FileDialog_Project>project_;
    std::unique_ptr<Fream_FileDialog_Item>item_;
    std::unique_ptr<Fream_FileDialog_CodeEditer>codeEditer_;
};

