#pragma once
#include <map>
#include <string>
#include <vector>
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

    void SetModelHandle(std::map<std::wstring, int> handle);

    void Draw();

    void CustomStatus();

    void Fream();

    void Tree(std::pair<const std::wstring,int>& handleData);
private:
    std::map<std::wstring, int> handleMap_;

    std::map<std::wstring, Vector3> sclMap_;
    std::map<std::wstring, Vector3> posMap_;
    std::map<std::wstring, Vector3> rotMap_;

    std::vector<Vector3> freamSclVec_;
    std::vector<Vector3> freamPosVec_;
    std::vector<Vector3> freamRotVec_;

    //int selected = 0;

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
};

