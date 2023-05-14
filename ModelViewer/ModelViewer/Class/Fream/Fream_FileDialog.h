#pragma once
#include "FreamBase.h"
#include <filesystem>
#include <iostream>

class Fream_FileDialog :
    public FreamBase
{
public:
    Fream_FileDialog();
    ~Fream_FileDialog();
    void Init();
    void Update();

    void Tree(std::filesystem::path directory,int levelNo);

    void Tree2(std::filesystem::path directory,int levelNo);

private:

    bool fileDialogFlg_;

    // �I�����Ă��鐔
    int selected_;
    // �I��p�}�X�N
    //int selection_mask_;

    std::vector<int> selection_mask_vector_;
};

