#include <Dxlib.h>
#include "ShaderMng.h"
#include "ConstantBuffer.h"
#include "../../Common/Utility.h"
#include "../FileDialog.h"

#include <algorithm>

ShaderMng::~ShaderMng()
{
    Clear();
}

bool ShaderMng::LoadShader(const std::wstring& name, const std::string& vsPath, const std::string& psPath, int bufferSize)
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
    pixelFilePath_[name] = psPath;
    
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

int ShaderMng::GetShaderVSHandle(const std::wstring& name) const
{
    // 指定された名前の頂点シェーダーハンドルを取得
    auto it = shaders_.find(name);
    if (it != shaders_.end())
    {
        return it->second.first;
    }
    return -1;
}

int ShaderMng::GetShaderPSHandle(const std::wstring& name) const
{
    // 指定された名前のピクセルシェーダーハンドルを取得
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
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/Model1FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_4FRAME:
        // １～４フレームの影響を受ける頂点
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/Model4FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_8FRAME:
        // ５～８フレームの影響を受ける頂点
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/Model8FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_FREE_FRAME:
        // ９フレーム以上の影響を受ける頂点
        vsHandle = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
        // 法線マップ用の情報が含まれる１フレームの影響を受ける頂点
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/ModelNormal1FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:
        // 法線マップ用の情報が含まれる１～４フレームの影響を受ける頂点
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/ModelNormal4FrameVertexShader.vs");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_8FRAME:
        // 法線マップ用の情報が含まれる５～８フレームの影響を受ける頂点
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/ModelNormal8FrameVertexShader.vs");
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
        vsHandle = shaders_[name].first;
        break;
    }

    shaders_[name].first = vsHandle;
}

void ShaderMng::Draw(const std::wstring& name, const int& modelHandle)
{
    //モデル中のトライアングルリストの数だけ回す
    const auto triangleListNum = MV1GetTriangleListNum(modelHandle);
    for (int i = 0; i < triangleListNum; i++)
    {
       MV1DrawTriangleList(modelHandle, i);		//トライアングルリスト単位での描画
    }
}

void ShaderMng::LoadShaderFile(const std::wstring& filePath)
{
    //ID3DInclude* include = D3D_COMPILE_STANDARD_FILE_INCLUDE;
    ID3DInclude* include = ((ID3DInclude*)(UINT_PTR)1);
    UINT flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    ID3DBlob* pErrorBlob = nullptr;

    std::wstring format = L".ps";
    std::wstring currentPath = L"data/ShaderBinary/";

    // ピクセルシェーダーのコンパイル
    {
        std::wstring fullFilepath = filePath;

        auto hr = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
            "ps_5_0", flag, 0, &pPSBlob, &pErrorBlob);


        if (FAILED(hr))
        {    
            std::string errstr;

        errstr.resize(pErrorBlob->GetBufferSize());
        std::copy_n((char*)pErrorBlob->GetBufferPointer(), pErrorBlob->GetBufferSize(), errstr.begin());
            assert(0 && "ピクセルシェーダーのコンパイルに失敗しました");
            return;
        }
    }
}

void ShaderMng::Updater(const std::wstring& name)
{
    if (ImGui::Button("shaderReload"))
    {
        LoadShaderFile(Utility::StringToWideString(OpenFileDialog()()));

        int handle = LoadPixelShaderFromMem(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize());

        shaders_[name].second = handle;
    }
};
