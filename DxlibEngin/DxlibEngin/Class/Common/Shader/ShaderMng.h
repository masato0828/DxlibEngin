#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "ConstantBuffer.h"
#include <wrl/client.h>
#include <assert.h>

#include <d3dcompiler.h>
#include <dxgi.h>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")

#define lpShaderMng ShaderMng::GetInstance()

enum class SLOT_TYPE
{
    DEFFUSE,
    NORMAL_MAP,
    SPECULAR_MAP,
    TOON_DEFFUSE_GRAD,
    TOON_SPECULAR_GRAD,
    TOON_SPHERE_MAP,
    TOON_RGB_TO_MAXRGB_VOLUME,
    SUB,
    SHADOW_MAP_0,
    SHADOW_MAP_1,
    SHADOW_MAP_2,
    CUBE_MAP,
};

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
    bool LoadShader(const std::wstring& name, const std::string& vsPath, const std::string& psPath, int bufferSize);

    /// <summary>
    /// �w�肳�ꂽ�V�F�[�_�[�n���h���A�o�b�t�@�̉��
    /// </summary>
    /// <param name="name">key�l</param>
    void UnloadShader(const std::wstring& name);

    /// <summary>
    /// �S�ẴV�F�[�_�[�n���h���A�o�b�t�@�̉��
    /// </summary>
    void Clear();

    /// <summary>
    /// ���_�V�F�[�_�n���h���̎擾
    /// </summary>
    /// <param name="name">key�l</param>
    /// <returns>�����F�n���h��  ���s�F-1</returns>
    int GetShaderVSHandle(const std::wstring& name) const;

    /// <summary>
    /// �s�N�Z���V�F�[�_�n���h���̎擾
    /// </summary>
    /// <param name="name">key�l</param>
    /// <returns>�����F�n���h��  ���s�F-1</returns>
    int GetShaderPSHandle(const std::wstring& name) const;

    /// <summary>
    /// �V�F�[�_�[���g�p�����`��̊J�n
    /// </summary>
    /// <param name="name">key�l</param>
    void DrawBegin(const std::wstring& name);

    /// <summary>
    /// �V�F�[�_�[���g�p�����`��̏I��
    /// </summary>
    void DrawEnd();

    /// <summary>
    /// �o�b�t�@�n���h���̎擾
    /// </summary>
    /// <param name="name">key�l</param>
    /// <returns>�o�b�t�@�n���h��</returns>
    int GetConstansBufferHnadle(const std::wstring& name) { return constansBuffers_[name]->GetHandle(); };

    /// <summary>
    /// �e�N�X�`���̃Z�b�g(DrawBegin�̌�)
    /// </summary>
    /// <param name="slot">�V�F�[�_�[�̂ǂ̃X���b�g�ɃZ�b�g���邩</param>
    /// <param name="imageHnadle">�摜�̃n���h��</param>
    void SetTexture(SLOT_TYPE slot,int imageHnadle);

    void EndTextere(SLOT_TYPE slot);

    void SetSkiningVertex(const std::wstring& name,const int& modelHandle);

    void Draw(const std::wstring& name,const int& modelHandle);

    bool LoadShaderFile(const std::wstring& name,const std::wstring& filePath);

    void Updater(const std::wstring& name);


private:

    // �V�F�[�_�[�̊Ǘ�(key�l,���_�V�F�[�_�n���h��,�s�N�Z���V�F�[�_�n���h��)
    std::unordered_map<std::wstring, std::pair<int, int>> shaders_;

    std::unordered_map <std::wstring, std::string> pixelFilePath_;

    // �o�b�t�@�̊Ǘ�(key�l,�R���X�^���g�o�b�t�@)
    std::unordered_map<std::wstring, std::unique_ptr<ConstantBuffer>> constansBuffers_;

    ID3DBlob* pPSBlob = nullptr;// �s�N�Z���V�F�[�_�[

    ShaderMng() = default;
    ~ShaderMng();
};