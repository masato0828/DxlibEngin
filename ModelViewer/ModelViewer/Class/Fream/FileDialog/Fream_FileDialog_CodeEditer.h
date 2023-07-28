#pragma once
#include "../FreamBase.h"
#include <string>
class Fream_FileDialog_CodeEditer :
    public FreamBase
{
public:
    Fream_FileDialog_CodeEditer();
    ~Fream_FileDialog_CodeEditer();
    void Init() override;
    void Update() override;
    void Update(std::string filePath);

private:
    //TextEditor editor;
};

