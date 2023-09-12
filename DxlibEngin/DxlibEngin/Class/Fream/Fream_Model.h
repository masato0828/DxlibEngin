#pragma once
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include "FreamBase.h"
#include "../Common/Vector2.h"
#include "../Common/Geometry.h"



class Fream_Model :
    public FreamBase
{
public:
    Fream_Model();
    ~Fream_Model();
    void Init()override;
    void Update()override;

    //void SetModelHandle(std::map<std::wstring, int> handle);

    void SetModelPath(const std::filesystem::path& path);

    void Draw();

    void CustomStatus();

    void Fream();

    int FileCnt(const std::wstring& fileName);

    std::map<std::wstring, int> NowModelHnadle();

    Vector3& GetModelPos();

    bool IsModelSelect();

    void LoadShaderProc(const std::wstring& name);

private:
    struct ModelFream
    {
        Vector3 ferampos;
        Vector3 feramrot;
        Vector3 feramscl;
    };

    struct Model
    {
        int handle;
        Vector3 pos;
        Vector3 rot;
        Vector3 scl;
        std::vector<ModelFream>modelFream;
    };

    std::map<std::wstring, Model> model_;

    struct FreamData
    {
        //FileData(FileData*, std::string); 
        FreamData(FreamData* parent, std::wstring name)
        {
            parentFream_ = parent;
            myName = name;
        }
        std::map<std::wstring, FreamData> freamMap_;
        FreamData* parentFream_;
        std::wstring myName;
    };

    FreamData freamData_;
    FreamData* nowSelect;
    std::wstring nowSelectFreamName_;

    int freamNumber_;

    void Tree(std::pair<const std::wstring, Model>& handleData,FreamData& freamData,FreamData*& nowSelect);

    bool contextMenuFlg_;

    std::wstring deleteModelName_;

    void ContextMenu();

    int handleCnt_;

    Vector3 nullPos_;

    std::map<std::wstring, bool> is_rotation_;
    
};

