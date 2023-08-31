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
    void Init();
    void Update();

    std::filesystem::path GetNowFile();
private:

    void EditMainCppCode();

    bool fileDialogFlg_;

    FileData fileData_;
    FileData* nowSelect;
    std::filesystem::path nowSelectPath_;

    std::filesystem::path fileFullPaht_;
    std::wstring nowSelectFileName_;

    std::wstring nowSelectFile_;

    std::unique_ptr<Fream_FileDialog_Tree>tree_;
    std::unique_ptr<Fream_FileDialog_Project>project_;
    std::unique_ptr<Fream_FileDialog_Item>item_;
    std::unique_ptr<Fream_FileDialog_CodeEditer>codeEditer_;

    std::vector<std::string> lines;  // 文字列を行ごとに分割するためのベクター


    bool is_update_;

    TextEditor editor;


    int fontNum_;
};

