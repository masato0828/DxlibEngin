#pragma once
#include "FreamBase.h"
class Fream_DokingArea :
    public FreamBase
{
public:
    Fream_DokingArea();
    ~Fream_DokingArea();

    /// <summary>
    /// ������
    /// </summary>
    void Init()override;

    /// <summary>
    /// �X�V
    /// </summary>
    void Update()override;
    
    /// <summary>
    /// �쐬
    /// </summary>
    void Create();

private:

};

