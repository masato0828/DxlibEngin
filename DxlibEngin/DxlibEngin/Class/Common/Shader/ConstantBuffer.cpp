#include <DxLib.h>
#include "ConstantBuffer.h"

ConstantBuffer::~ConstantBuffer()
{
    Release();
}

bool ConstantBuffer::Create(int bufferSize)
{
    Release();

   

    // �萔�o�b�t�@�̍쐬
    bufferHandle_ = CreateShaderConstantBuffer(bufferSize);
    return (bufferHandle_ != -1);
}

void ConstantBuffer::Release()
{
    // �萔�o�b�t�@�̉��
    if (bufferHandle_ != -1)
    {
        DeleteShaderConstantBuffer(bufferHandle_);
        bufferHandle_ = -1;
    }
}
