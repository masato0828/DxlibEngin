#pragma once
#include "FreamBase.h"
class Fream_DokingArea :
    public FreamBase
{
public:
    Fream_DokingArea();
    ~Fream_DokingArea();

    /// <summary>
    /// 初期化
    /// </summary>
    void Init()override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update()override;
    
    /// <summary>
    /// 作成
    /// </summary>
    void Create();

private:

};

