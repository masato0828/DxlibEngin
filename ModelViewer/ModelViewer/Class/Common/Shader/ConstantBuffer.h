#pragma once
class ConstantBuffer
{
public:
    ConstantBuffer() : bufferHandle_(-1) {}
    ~ConstantBuffer();

    bool Create(int bufferSize);

    void Release();

    int GetHandle() const { return bufferHandle_; }

private:
    int bufferHandle_;
};
