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

class Fream_FileDialog :
    public FreamBase
{
public:

    Fream_FileDialog();
    ~Fream_FileDialog();
    void Init();
    void Update();

    void Tree(std::filesystem::path directory, FileData& fileData);
private:

    bool fileDialogFlg_;

    // 選択している数
    int selected_;
    // 選択用マスク
    //int selection_mask_;

    FileData fileData_;
    FileData* nowSelect;
    std::filesystem::path nowSelectPath_;

    std::vector<int> selection_mask_vector_;

    std::filesystem::path fileFullPaht_;
    std::wstring nowSelectFileName_;

    std::wstring nowSelectFile_;

    // シェーダ情報の作成
    ID3D11ShaderResourceView* my_shaderData = NULL;
    std::vector<ID3D11ShaderResourceView*> my_shaderData_vector_;

    std::unique_ptr<Fream_FileDialog_Tree>tree_;
    std::unique_ptr<Fream_FileDialog_Project>project_;
    std::unique_ptr<Fream_FileDialog_Item>item_;

    struct Color
    {
        float r;
        float g;
        float b;
        float a;
    };
    bool CharacterSearch(std::string showName, std::string searchFileName,Color color, std::string defaultTarget);

    void ItemWindow();
    
    void Recovery(FileData* selectData);

    void MakeFileImage(std::wstring name);

    void CreateImage(std::string filePath);

    bool IsHeaderFile(const std::string& filename,const std::string& ext);

    bool IsMatch(const std::string& filepath, const std::wstring& target);

    std::wstring StringToWideString(const std::string& str);

    std::string WideStringToString(const std::wstring& wideString);

    // std::stringをwchar_t*に変換する関数
    std::wstring ConvertToWideString(const std::string& str);

    std::wstring GetAssociatedApplicationPath(const std::wstring& filePath);

    std::wstring GetDefaultApplication(const std::wstring& extension);

    bool LaunchApplication(const std::wstring& applicationPath, const std::wstring& filePath);

    bool OpenWithDefaultApplication(const std::wstring& filePath);

    std::wstring ShowApplicationSelectionDialog();

};

