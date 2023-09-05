#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "ConstantBuffer.h"

#define lpShaderMng ShaderMng::GetInstance()

class ShaderMng
{
public:

    static ShaderMng& GetInstance()
    {
        static ShaderMng Instance_;
        return Instance_;
    }

    /// <summary>
    /// �V�F�[�_�[�̓ǂݍ���
    /// </summary>
    /// <param name="name">map�pkey�l</param>
    /// <param name="vsPath">���_�V�F�[�_�̃p�X</param>
    /// <param name="psPath">�s�N�Z���V�F�[�_�̃p�X</param>
    /// <param name="bufferSize">�o�b�t�@�̃T�C�Y</param>
    /// <returns>�ǂݍ��ݐ����Ftrue�@���s�Ffalse</returns>
    bool LoadShader(const std::string& name, const std::string& vsPath, const std::string& psPath, int bufferSize);

    /// <summary>
    /// �w�肳�ꂽ�V�F�[�_�[�n���h���A�o�b�t�@�̉��
    /// </summary>
    /// <param name="name">key�l</param>
    void UnloadShader(const std::string& name);

    /// <summary>
    /// �S�ẴV�F�[�_�[�n���h���A�o�b�t�@�̉��
    /// </summary>
    void Clear();

    /// <summary>
    /// ���_�V�F�[�_�n���h���̎擾
    /// </summary>
    /// <param name="name">key�l</param>
    /// <returns>�����F�n���h��  ���s�F-1</returns>
    int GetShaderVSHandle(const std::string& name) const;

    /// <summary>
    /// �s�N�Z���V�F�[�_�n���h���̎擾
    /// </summary>
    /// <param name="name">key�l</param>
    /// <returns>�����F�n���h��  ���s�F-1</returns>
    int GetShaderPSHandle(const std::string& name) const;

    /// <summary>
    /// �V�F�[�_�[���g�p�����`��̊J�n
    /// </summary>
    /// <param name="name">key�l</param>
    void DrawBegin(const std::string& name);

    /// <summary>
    /// �V�F�[�_�[���g�p�����`��̏I��
    /// </summary>
    void DrawEnd();

    /// <summary>
    /// �o�b�t�@�n���h���̎擾
    /// </summary>
    /// <param name="name">key�l</param>
    /// <returns>�o�b�t�@�n���h��</returns>
    int GetConstansBufferHnadle(const std::string& name) { return constansBuffers_[name]->GetHandle(); };

    /// <summary>
    /// �e�N�X�`���̃Z�b�g(DrawBegin�̌�)
    /// </summary>
    /// <param name="slot">�V�F�[�_�[�̂ǂ̃X���b�g�ɃZ�b�g���邩</param>
    /// <param name="imageHnadle">�摜�̃n���h��</param>
    void SetTexture(int slot,int imageHnadle);

private:

    // �V�F�[�_�[�̊Ǘ�(key�l,���_�V�F�[�_�n���h��,�s�N�Z���V�F�[�_�n���h��)
    std::unordered_map<std::string, std::pair<int, int>> shaders_;

    // �o�b�t�@�̊Ǘ�(key�l,�R���X�^���g�o�b�t�@)
    std::unordered_map<std::string, std::unique_ptr<ConstantBuffer>> constansBuffers_;

    ShaderMng() = default;
    ~ShaderMng();
};