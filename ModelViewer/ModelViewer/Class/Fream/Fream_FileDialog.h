#pragma once
#include "FreamBase.h"
#include <filesystem>
#include <iostream>
#include <unordered_map>

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

    // �I�����Ă��鐔
    int selected_;
    // �I��p�}�X�N
    //int selection_mask_;


    struct FileData
    {
        std::unordered_map<std::string,FileData> fileMap_;
    };

    FileData fileData_;
    FileData* nowSelect;

    std::vector<int> selection_mask_vector_;

    struct Color
    {
        float r;
        float g;
        float b;
        float a;
    };
    bool CharacterSearch(std::string target, std::string searchFileName,Color color );
};

