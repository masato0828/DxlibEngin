#pragma once
#include "FreamBase.h"
class Fream_FileDialog :
    public FreamBase
{
public:
    Fream_FileDialog();
    ~Fream_FileDialog();
    void Init();
    void Update();
private:

    bool fileDialogFlg_;
};

