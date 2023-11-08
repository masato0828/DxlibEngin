#pragma once
#include "FreamBase.h"
class Fream_DokingArea :
    public FreamBase
{
public:
    Fream_DokingArea();
    ~Fream_DokingArea();

    /// <summary>
    /// ‰Šú‰»
    /// </summary>
    void Init()override;

    /// <summary>
    /// XV
    /// </summary>
    void Update()override;
    
    /// <summary>
    /// ì¬
    /// </summary>
    void Create();

private:

};

