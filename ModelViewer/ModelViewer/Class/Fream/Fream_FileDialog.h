#pragma once
#include "FreamBase.h"
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include "../System/ImguiImageStb.h"

class Fream_FileDialog :
    public FreamBase
{
public:
    struct FileData;

    Fream_FileDialog();
    ~Fream_FileDialog();
    void Init();
    void Update();

    void Tree(std::filesystem::path directory,int levelNo);

    void Tree2(std::filesystem::path directory, FileData& fileData);
private:

    bool fileDialogFlg_;

    // 選択している数
    int selected_;
    // 選択用マスク
    //int selection_mask_;


    struct FileData
    {
        FileData(FileData*, std::string);
        std::unordered_map<std::string,FileData> fileMap_;
        FileData* parentFile_;
        std::string myName;
    };

    FileData fileData_;
    FileData* nowSelect;

    std::vector<int> selection_mask_vector_;

    // シェーダ情報の作成
    ID3D11ShaderResourceView* my_shaderData = NULL;

    struct Color
    {
        float r;
        float g;
        float b;
        float a;
    };
    bool CharacterSearch(std::string target, std::string searchFileName,Color color );

    void ItemWindow();
    
    void Recovery(FileData* selectData);

    void MakeFileImage(std::string_view name);

    void CreateImage();
};

