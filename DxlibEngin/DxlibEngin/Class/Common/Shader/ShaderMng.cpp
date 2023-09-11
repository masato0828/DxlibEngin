#include <Dxlib.h>
#include "ShaderMng.h"
#include "ConstantBuffer.h"

ShaderMng::~ShaderMng()
{
    Clear();
}

bool ShaderMng::LoadShader(const std::string& name, const std::string& vsPath, const std::string& psPath, int bufferSize)
{
    // 頂点シェーダの読み込み
    int vsHandle = LoadVertexShader(vsPath.c_str());
    if (vsHandle < 0)
        //return false;
        vsHandle = -1;

    // ピクセルシェーダの読み込み
    int psHandle = LoadPixelShader(psPath.c_str());
    if (psHandle < 0)
    {
        // ピクセルシェーダーの読み込みに失敗した場合、頂点シェーダーを解放する
        DeleteShader(vsHandle);
        return false;
    }

    // シェーダーハンドルをマップに追加
    shaders_[name] = std::make_pair(vsHandle, psHandle);
    
    constansBuffers_[name] = std::make_unique<ConstantBuffer>();
    constansBuffers_[name]->Create(bufferSize);
    /*if (constansBuffers_[name]->Create(bufferSize) < 0)
    {
        return false;
    }*/

    return true;
}

void ShaderMng::UnloadShader(const std::string& name)
{
    // 指定された名前のシェーダーハンドルを解放
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
    // 全てのシェーダーハンドルを解放
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

int ShaderMng::GetShaderVSHandle(const std::string& name) const
{
    // 指定された名前の頂点シェーダーハンドルを取得
    auto it = shaders_.find(name);
    if (it != shaders_.end())
    {
        return it->second.first;
    }
    return -1;
}

int ShaderMng::GetShaderPSHandle(const std::string& name) const
{
    // 指定された名前のピクセルシェーダーハンドルを取得
    auto it = shaders_.find(name);
    if (it != shaders_.end())
    {
        return it->second.second;
    }
    return -1;
}

void ShaderMng::DrawBegin(const std::string& name)
{
    SetUseVertexShader(shaders_[name].first);
    SetUsePixelShader(shaders_[name].second);
}

void ShaderMng::DrawEnd()
{
    SetUseVertexShader(-1);
    SetUsePixelShader(-1);
}

void ShaderMng::SetTexture(int slot, int imageHnadle)
{
    SetUseTextureToShader(slot,imageHnadle);
}

void ShaderMng::SetSkiningVertex(const std::string& name, const int& modelHandle, const std::string& vsPath)
{
    // モデルに含まれるトライアングルリストの数を取得する
    int modelListNum = MV1GetTriangleListNum(modelHandle);
    int modelShaderType = DX_MV1_VERTEX_TYPE_1FRAME;
    int vsHandle = -1;

    // 頂点タイプの取得
    for (int i = 0; i < modelListNum; i++)
    {
        // トライアングルリストの頂点タイプ
        modelShaderType = MV1GetTriangleListVertexType(modelHandle, i);
        break;
    }


    // 頂点シェーダの読み込み
    switch (modelShaderType)
    {
    case DX_MV1_VERTEX_TYPE_1FRAME:
        // １フレームの影響を受ける頂点
        vsHandle = -1;
            break;
    case DX_MV1_VERTEX_TYPE_4FRAME:
        // １～４フレームの影響を受ける頂点
        vsHandle = -1;
            break;
    case DX_MV1_VERTEX_TYPE_8FRAME:
        // ５～８フレームの影響を受ける頂点
        vsHandle = -1;
            break;
    case DX_MV1_VERTEX_TYPE_FREE_FRAME:
        // ９フレーム以上の影響を受ける頂点
        vsHandle = -1;
            break;
    case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
        // 法線マップ用の情報が含まれる１フレームの影響を受ける頂点
        vsHandle = -1;
            break;
    case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:
        // 法線マップ用の情報が含まれる１～４フレームの影響を受ける頂点
        vsHandle = -1;
            break;
    case DX_MV1_VERTEX_TYPE_NMAP_8FRAME:
        // 法線マップ用の情報が含まれる５～８フレームの影響を受ける頂点
        vsHandle = -1;
            break;
    case DX_MV1_VERTEX_TYPE_NMAP_FREE_FRAME:
        // 法線マップ用の情報が含まれる９フレーム以上の影響を受ける頂点
        vsHandle = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NUM:
        // 頂点タイプの数
        vsHandle = -1;
        break;
    default:
        vsHandle = LoadVertexShader(vsPath.c_str());
        break;
    }

    
    if (vsHandle < 0)
        // 元の頂点シェーダーを入れる
        vsHandle = shaders_[name].second;

    shaders_[name].second = vsHandle;
}
