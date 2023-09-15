#include <DxLib.h>
#include "ConstantBuffer.h"

ConstantBuffer::~ConstantBuffer()
{
    Release();
}

bool ConstantBuffer::Create(int bufferSize)
{
    Release();

   

    // 定数バッファの作成
    bufferHandle_ = CreateShaderConstantBuffer(bufferSize);
    return (bufferHandle_ != -1);
}

void ConstantBuffer::Release()
{
    // 定数バッファの解放
    if (bufferHandle_ != -1)
    {
        DeleteShaderConstantBuffer(bufferHandle_);
        bufferHandle_ = -1;
    }
}
