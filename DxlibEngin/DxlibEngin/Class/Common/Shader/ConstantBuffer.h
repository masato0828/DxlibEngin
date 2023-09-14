#pragma once

class ConstantBuffer
{
public:
    ConstantBuffer() : bufferHandle_(-1) {}
    ~ConstantBuffer();

    /// <summary>
    /// �o�b�t�@�̍쐬
    /// </summary>
    /// <param name="bufferSize">�o�b�t�@�̃T�C�Y</param>
    /// <returns>�����Ftrue�@���s�Ffalse</returns>
    bool Create(int bufferSize);

    /// <summary>
    /// �o�b�t�@�̉��
    /// </summary>
    void Release();

    /// <summary>
    /// �o�b�t�@�̎擾
    /// </summary>
    /// <returns>�o�b�t�@�̃n���h��</returns>
    int GetHandle() const { return bufferHandle_; }

private:
    // �o�b�t�@�̃n���h��
    int bufferHandle_;
};
