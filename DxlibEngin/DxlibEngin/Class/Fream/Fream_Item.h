#pragma once
#include "FreamBase.h"
class Fream_Item :
    public FreamBase
{
public:
    Fream_Item();
    ~Fream_Item();

    /// <summary>
    /// 初期化
    /// </summary>
    void Init()override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update()override;
private:

};

