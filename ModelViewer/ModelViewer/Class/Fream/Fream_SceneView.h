#pragma once
#include "FreamBase.h"
#include "../Common/Vector2.h"
#include <string>
#include <filesystem>
#include <map>

class Fream_SceneView :
    public FreamBase
{
public:
    Fream_SceneView();
    ~Fream_SceneView();
    void Init() override;
    void Update() override;
    void Create();

    float GetFactor();
    Vector2 GetDefaultImageSize();
    Vector2Flt GetImageLeftUpCornor();
    Vector2Flt GetImageRightDownCornor();
    Vector2 GetWindowCenterPoint();
    Vector2Flt GetScreenSize();

    std::filesystem::path GetDropModelPath();
    //std::map<std::wstring, int> GetModelMap();

private:

    float factor_;

    Vector2 defaultImageSize_;

    Vector2Flt imageLeftUpCornor_;
    Vector2Flt imageRightDownCornor_;

    Vector2Flt screenSize_;

    //std::map<std::wstring, int> handleMap_;

    //int fileCount;

    //int FileCnt(const std::wstring& fileName);
    void CreateDragAndDropHandle();

   
    std::filesystem::path dropModelPath_;

    bool is_FileLoad_;


};

