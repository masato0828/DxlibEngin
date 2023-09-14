#pragma once
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include "FreamBase.h"
#include "../Common/Vector2.h"
#include "../Common/Geometry.h"

enum class COLOR_TYPE
{
    DIF,
    SPC,
    AMB,
    EMI,
};


class Fream_Model :
    public FreamBase
{
public:
    Fream_Model();
    ~Fream_Model();
    void Init()override;
    void Update()override;

    void SetModelPath(const std::filesystem::path& path);

    void Draw();

    void CustomStatus();

   

    bool IsModelSelect();
  
    Vector3& GetModelPos();

private:
    
    struct ModelFream
    {
        Vector3 ferampos;
        Vector3 feramrot;
        Vector3 feramscl;
    };

    struct Material
    {
        std::wstring materialName;
        std::map<COLOR_TYPE,COLOR_F> color;
        int spcPower;
        int difMapTex;
        int spcMapTex;
        int normalMapTex;
    };

    struct Model
    {
        std::wstring name;
        int handle;
        Vector3 pos;
        Vector3 rot;
        Vector3 scl;
        std::vector<ModelFream>fream;
        bool isRotation;
        std::vector<Material> material;
        std::map<COLOR_TYPE, bool> allChangeColor;
    };

    std::map<std::wstring, Model> model_;
    std::map<std::wstring, Model> defoModelData_;

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

    struct PsColorPram
    {
        FLOAT3 diffuse;
        FLOAT3 specular;
    };

    FreamData freamData_;
    FreamData* nowSelect_;
    std::wstring nowSelectFreamName_;

    int freamNumber_;

    void Tree(std::pair<const std::wstring, Model>& handleData,FreamData& freamData,FreamData*& nowSelect);

    bool contextMenuFlg_;

    std::wstring deleteModelName_;

    void Fream();

    int FileCnt(const std::wstring& fileName);

    void ContextMenu();

    void ColorEdit(const std::string& tagName, COLOR_TYPE type);

    void AllColorEdit(const std::string& tagName);

    void LoadShaderProc(const std::wstring& name);

    std::vector<Material> CreateMaterialData(const int& handle);


    int handleCnt_;

    Vector3 nullPos_;
    
};

