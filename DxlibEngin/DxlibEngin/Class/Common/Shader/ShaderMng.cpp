#include "ShaderMng.h"
#include "ConstantBuffer.h"
#include "../../Common/Utility.h"
#include "../FileDialog.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

ShaderMng::~ShaderMng()
{
    Clear();
}

std::map<std::string, ShaderMng::BufferData>& ShaderMng::DataAcsess(const std::wstring& key, const std::string& registerMapKey)
{
    return constantBufferMap_[key][registerMapKey].bufferData;
}

bool ShaderMng::LoadShader(const std::wstring& name, const std::string& vsPath, const std::string& psPath, int registerNumber , int registerNumberLoss)
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
    constansBuffers_[name]->Create(registerNumber);

    CreateRegisterData(name,psPath, registerNumber,registerNumberLoss);

    return true;
}

bool ShaderMng::LoadShader(const std::wstring& name, const std::string& vsPath, const std::string& psPath, int bufferSize)
{
    LoadShader(name,vsPath,psPath,bufferSize,0);
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

void ShaderMng::SetModelTexture(SLOT_TYPE slot, int imageHnadle)
{
    if (imageHnadle == -1)
    {
        return;
    }
    SetUseTextureToShader(static_cast<int>(slot),imageHnadle);
}

void ShaderMng::SetTexture(int slotNum, int imageHnadle)
{
    if (imageHnadle == -1)
    {
        return;
    }
    SetUseTextureToShader(slotNum, imageHnadle);
}

void ShaderMng::EndTextere(SLOT_TYPE slot)
{
    SetUseTextureToShader(static_cast<int>(slot), -1);			//設定されたテクスチャを解除する
}

void ShaderMng::EndTexture(int slotNum)
{
    SetUseTextureToShader(slotNum, -1);			//設定されたテクスチャを解除する
}

void ShaderMng::SetSkiningVertex(const std::wstring& name, const int& modelHandle)
{
    // モデルに含まれるトライアングルリストの数を取得する
    int modelListNum = MV1GetTriangleListNum(modelHandle);
    int modelShaderType = DX_MV1_VERTEX_TYPE_1FRAME;
    int vsHandle = -1;
    int psHandle = -1;

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
        psHandle = LoadPixelShader("data/ShaderBinary/Pixel/ModelNormalPixelShader.ps");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:
        // 法線マップ用の情報が含まれる１～４フレームの影響を受ける頂点
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/ModelNormal4FrameVertexShader.vs");
        psHandle = LoadPixelShader("data/ShaderBinary/Pixel/ModelNormalPixelShader.ps");
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_8FRAME:
        // 法線マップ用の情報が含まれる５～８フレームの影響を受ける頂点
        vsHandle = LoadVertexShader("data/ShaderBinary/Vertex/ModelNormal8FrameVertexShader.vs");
        psHandle = LoadPixelShader("data/ShaderBinary/Pixel/ModelNormalPixelShader.ps");
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

    if (psHandle != -1)
    {
        shaders_[name].second = psHandle;
    }

}

bool ShaderMng::LoadShaderFile(const std::wstring& name, const std::wstring& filePath)
{
    ID3DInclude* include = D3D_COMPILE_STANDARD_FILE_INCLUDE;
    UINT flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    ID3DBlob* pErrorBlob = nullptr;

    // ピクセルシェーダーのコンパイル
    {
        std::wstring fullFilepath = filePath;
        pixelFilePath_[name] = Utility::WideStringToString(filePath);
        auto hr = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
            "ps_5_0", flag, 0, &pPSBlob, &pErrorBlob);

        // コンパイル失敗
        if (FAILED(hr))
        {
            return false;
        }
    }

    return true;
}

void ShaderMng::Updater(const std::wstring& name)
{
    if (ImGui::Button("shaderReload"))
    {
        std::wstring format = L".hlsl";
        std::wstring currentPath = L"Shader/Pixel/";
        std::filesystem::path path = pixelFilePath_[name];
        std::wstring noExtFilePath = path.filename().replace_extension();

        auto hlslPath = currentPath + noExtFilePath + format;

        bool result = LoadShaderFile(name,hlslPath.c_str());

        if (result)
        {
            int handle = LoadPixelShaderFromMem(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize());

            shaders_[name].second = handle;
        }
    }

    if (ImGui::Button("shaderLoad"))
    {
        bool result = LoadShaderFile(name,OpenFileDialog()());

        if (result)
        {
            int handle = LoadPixelShaderFromMem(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize());

            shaders_[name].second = handle;
        }
    }

    RegisterUpdate();
};

void ShaderMng::RegisterCustom(const std::wstring& key)
{
    ImGui::Begin("register");
    std::filesystem::path bufferKey = key;

    for (auto& b : constantBufferMap_[key])
    {
        //if (ImGui::CollapsingHeader(b.first.c_str()))
        {
            auto* cbBuf = (float*)GetBufferShaderConstantBuffer(b.second.bufferHandle);
            for (auto& var : b.second.bufferData)
            {
                if (var.second.typeName == "float4")
                {
                    ImGui::Text((bufferKey.string() + var.second.varName).c_str());
                    ImGuiCustom::ColorEdit4((bufferKey.string() + var.second.varName).c_str(), &var.second.data);
                    cbBuf[0] = var.second.data.x;
                    cbBuf[1] = var.second.data.y;
                    cbBuf[2] = var.second.data.z;
                    cbBuf[3] = var.second.data.w;
                    cbBuf += 4;
                }
                if (var.second.typeName == "float3")
                {
                    ImGui::Text((bufferKey.string() + var.second.varName).c_str());
                    ImGuiCustom::ColorEdit3((bufferKey.string() + var.second.varName).c_str(), &var.second.data);
                    cbBuf[0] = var.second.data.x;
                    cbBuf[1] = var.second.data.y;
                    cbBuf[2] = var.second.data.z;
                    cbBuf[3] = var.second.data.w;
                    cbBuf += 4;
                }
                if (var.second.typeName == "float2")
                {
                    ImGui::Text((bufferKey.string() + var.second.varName).c_str());
                    ImGui::DragFloat2((bufferKey.string() + var.second.varName).c_str(), (float*)&var.second.data, 0.1f, 0.0f, 1.0f);
                    cbBuf[0] = var.second.data.x;
                    cbBuf[1] = var.second.data.y;
                    cbBuf += 2;
                }
                if (var.second.typeName == "float")
                {
                    ImGui::Text((bufferKey.string() + var.second.varName).c_str());
                    ImGui::DragFloat((bufferKey.string() + var.second.varName).c_str(), (float*)&var.second.data, 0.1f, 0.0f, 1.0f);
                    cbBuf[0] = var.second.data.x;
                    cbBuf += 1;
                }

            }
            UpdateShaderConstantBuffer(b.second.bufferHandle);
            SetShaderConstantBuffer(b.second.bufferHandle, DX_SHADERTYPE_PIXEL, b.second.registerNumber);
        }

    }
    ImGui::End();
}

void ShaderMng::RegisterUpdate(const std::wstring& key)
{
    std::filesystem::path bufferKey = key;

    for (auto& b : constantBufferMap_[key])
    {
        //if (ImGui::CollapsingHeader(b.first.c_str()))
        {
            auto* cbBuf = (float*)GetBufferShaderConstantBuffer(b.second.bufferHandle);
            for (auto& var : b.second.bufferData)
            {
                if (var.second.typeName == "float4")
                {
                    cbBuf[0] = var.second.data.x;
                    cbBuf[1] = var.second.data.y;
                    cbBuf[2] = var.second.data.z;
                    cbBuf[3] = var.second.data.w;
                    cbBuf += 4;
                }
                if (var.second.typeName == "float3")
                {
                    cbBuf[0] = var.second.data.x;
                    cbBuf[1] = var.second.data.y;
                    cbBuf[2] = var.second.data.z;
                    cbBuf[3] = var.second.data.w;
                    cbBuf += 4;
                }
                if (var.second.typeName == "float2")
                {
                    cbBuf[0] = var.second.data.x;
                    cbBuf[1] = var.second.data.y;
                    cbBuf += 2;
                }
                if (var.second.typeName == "float")
                {
                    cbBuf[0] = var.second.data.x;
                    cbBuf += 1;
                }
                UpdateShaderConstantBuffer(b.second.bufferHandle);
                SetShaderConstantBuffer(b.second.bufferHandle, DX_SHADERTYPE_PIXEL, b.second.registerNumber);
            }
        }

    }
}

void ShaderMng::RegisterUpdate()
{
    for (auto& cbuffer : constantBufferMap_)
    {
        RegisterUpdate(cbuffer.first);
    }
}

void ShaderMng::CreateRegisterData(const std::wstring& key, const std::string& psPath, const int registerNumber, const int registerNumberLoss)
{
    std::ifstream infile(psPath, std::ios::binary); // シェーダー（バイナリ）を読み込む
    std::vector<char> data;
    data.resize(infile.seekg(0, std::ios::end).tellg());
    infile.seekg(0, std::ios::beg).read(data.data(), data.size());

    Microsoft::WRL::ComPtr<ID3D12ShaderReflection> shaderRef;
    D3DReflect(data.data(), data.size(), IID_PPV_ARGS(&shaderRef));

    std::stringstream ss;
    D3D12_SHADER_DESC desc{};
    shaderRef->GetDesc(&desc);

    std::map<std::string, RegisterData> registerMap;
    const auto cbCount = desc.ConstantBuffers;
    for (auto i = registerNumberLoss; i < cbCount; ++i)
    {
        D3D12_SHADER_BUFFER_DESC shaderBuffer{};
        auto cbuffer = shaderRef->GetConstantBufferByIndex(i);
        cbuffer->GetDesc(&shaderBuffer);

        std::map<std::string, BufferData> bufferData;
        RegisterData registerData;
        auto constantHandle = CreateShaderConstantBuffer(shaderBuffer.Size);
        // コンスタントバッファ内
        for (auto j = 0; j < shaderBuffer.Variables; ++j)
        {
            D3D12_SHADER_VARIABLE_DESC varDesc{};
            D3D12_SHADER_TYPE_DESC typeDesc;
            auto shaderReflection = cbuffer->GetVariableByIndex(j);
            auto varTypeRefl = shaderReflection->GetType();

            shaderReflection->GetDesc(&varDesc);
            varTypeRefl->GetDesc(&typeDesc);

            std::string varName = varDesc.Name;
            std::string var = typeDesc.Name;
            size_t varSize = varDesc.Size;
            std::string Name = varDesc.Name;

            FLOAT4 forfloat = {1,1,1,1};
            bufferData.emplace(varName.c_str(),BufferData(var,varName,varSize, forfloat));

            registerData = RegisterData(constantHandle, i + registerNumber, bufferData);
        }

        registerMap.emplace((std::string)shaderBuffer.Name, registerData);
    }

    constantBufferMap_.emplace(key, registerMap);
}
