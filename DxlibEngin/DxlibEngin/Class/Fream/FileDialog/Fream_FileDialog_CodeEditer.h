#pragma once
#include "../FreamBase.h"
#include <string>
#include <filesystem>
#include "TextEditerSetUp.h"
#include "../../../imGui/ImGuiColorTextEdit/TextEditor.h"
class Fream_FileDialog_CodeEditer :
    public FreamBase
{
public:
    Fream_FileDialog_CodeEditer();
    ~Fream_FileDialog_CodeEditer();
    void Init() override;
    void Update() override;
    void Update(std::filesystem::path filePath);

private:
    TextEditor editor;

    bool isText_;

    std::string beforCode_;
};

