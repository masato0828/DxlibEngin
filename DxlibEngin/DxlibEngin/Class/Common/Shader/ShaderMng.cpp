#include <Dxlib.h>
#include "ShaderMng.h"
#include "ConstantBuffer.h"

ShaderMng::~ShaderMng()
{
    Clear();
}

bool ShaderMng::LoadShader(const std::wstring& name, const std::string& vsPath, const std::string& psPath, int bufferSize)
{
    // ���_�V�F�[�_�̓ǂݍ���
    int vsHandle = LoadVertexShader(vsPath.c_str());
    if (vsHandle < 0)
        //return false;
        vsHandle = -1;

    // �s�N�Z���V�F�[�_�̓ǂݍ���
    int psHandle = LoadPixelShader(psPath.c_str());
    if (psHandle < 0)
    {
        // �s�N�Z���V�F�[�_�[�̓ǂݍ��݂Ɏ��s�����ꍇ�A���_�V�F�[�_�[���������
        DeleteShader(vsHandle);
        return false;
    }

    // �V�F�[�_�[�n���h�����}�b�v�ɒǉ�
    shaders_[name] = std::make_pair(vsHandle, psHandle);
    
    constansBuffers_[name] = std::make_unique<ConstantBuffer>();
    constansBuffers_[name]->Create(bufferSize);
    /*if (constansBuffers_[name]->Create(bufferSize) < 0)
    {
        return false;
    }*/

    return true;
}

void ShaderMng::UnloadShader(const std::wstring& name)
{
    // �w�肳�ꂽ���O�̃V�F�[�_�[�n���h�������
    auto it = shaders_.find(name);
    if (it != shaders_.end())
    {
        DeleteShader(it->second.first);
        DeleteShader(it->second.second);
        shaders_.erase(it);
    }

    auto bufferName = constansBuffers_.find(name);
    if (bufferName != constansBuffers_.end())
    {
        constansBuffers_.erase(bufferName);
    }
}

void ShaderMng::Clear()
{
    // �S�ẴV�F�[�_�[�n���h�������
    for (auto& shader : shaders_)
    {
        DeleteShader(shader.second.first);
        DeleteShader(shader.second.second);
    }
    shaders_.clear();

    for (auto& buffer : constansBuffers_)
    {
        buffer.second->Release();
    }
}

int ShaderMng::GetShaderVSHandle(const std::wstring& name) const
{
    // �w�肳�ꂽ���O�̒��_�V�F�[�_�[�n���h�����擾
    auto it = shaders_.find(name);
    if (it != shaders_.end())
    {
        return it->second.first;
    }
    return -1;
}

int ShaderMng::GetShaderPSHandle(const std::wstring& name) const
{
    // �w�肳�ꂽ���O�̃s�N�Z���V�F�[�_�[�n���h�����擾
    auto it = shaders_.find(name);
    if (it != shaders_.end())
    {
        return it->second.second;
    }
    return -1;
}

void ShaderMng::DrawBegin(const std::wstring& name)
{
    MV1SetUseOrigShader(true);

    SetUseVertexShader(shaders_[name].first);
    SetUsePixelShader(shaders_[name].second);
}

void ShaderMng::DrawEnd()
{
    SetUseVertexShader(-1);
    SetUsePixelShader(-1);

    MV1SetUseOrigShader(false);
}

void ShaderMng::SetTexture(SLOT_TYPE slot, int imageHnadle)
{
    SetUseTextureToShader(static_cast<int>(slot),imageHnadle);
}

void ShaderMng::SetSkiningVertex(const std::wstring& name, const int& modelHandle)
{
    // ���f���Ɋ܂܂��g���C�A���O�����X�g�̐����擾����
    int modelListNum = MV1GetTriangleListNum(modelHandle);
    int modelShaderType = DX_MV1_VERTEX_TYPE_1FRAME;
    int vsHandle = -1;

    // ���_�^�C�v�̎擾
    for (int i = 0; i < modelListNum; i++)
    {
        // �g���C�A���O�����X�g�̒��_�^�C�v
        modelShaderType = MV1GetTriangleListVertexType(modelHandle, i);
        break;
    }


    // ���_�V�F�[�_�̓ǂݍ���
    switch (modelShaderType)
    {
    case DX_MV1_VERTEX_TYPE_1FRAME:
        // �P�t���[���̉e�����󂯂钸�_
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/Model1FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_4FRAME:
        // �P�`�S�t���[���̉e�����󂯂钸�_
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/Model4FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_8FRAME:
        // �T�`�W�t���[���̉e�����󂯂钸�_
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/Model8FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_FREE_FRAME:
        // �X�t���[���ȏ�̉e�����󂯂钸�_
        vsHandle = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
        // �@���}�b�v�p�̏�񂪊܂܂��P�t���[���̉e�����󂯂钸�_
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/ModelNormal1FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:
        // �@���}�b�v�p�̏�񂪊܂܂��P�`�S�t���[���̉e�����󂯂钸�_
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/ModelNormal4FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_8FRAME:
        // �@���}�b�v�p�̏�񂪊܂܂��T�`�W�t���[���̉e�����󂯂钸�_
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/ModelNormal8FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_FREE_FRAME:
        // �@���}�b�v�p�̏�񂪊܂܂��X�t���[���ȏ�̉e�����󂯂钸�_
        vsHandle = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NUM:
        // ���_�^�C�v�̐�
        vsHandle = -1;
        break;
    default:
        vsHandle = shaders_[name].first;
        break;
    }

    shaders_[name].first = vsHandle;
}

void ShaderMng::Draw(const std::wstring& name, const int& modelHandle)
{
    //���f�����̃g���C�A���O�����X�g�̐�������
    const auto triangleListNum = MV1GetTriangleListNum(modelHandle);
    for (int i = 0; i < triangleListNum; i++)
    {
       MV1DrawTriangleList(modelHandle, i);		//�g���C�A���O�����X�g�P�ʂł̕`��
    }
}