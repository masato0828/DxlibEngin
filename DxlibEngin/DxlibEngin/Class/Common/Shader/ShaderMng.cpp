#include "ShaderMng.h"
#include "ConstantBuffer.h"
#include "../../Common/Utility.h"
#include "../FileDialog.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>


#include <direct.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <errno.h>

ShaderMng::~ShaderMng()
{
    Clear();
}

void ShaderMng::CmdPronpt(std::string fileName, std::string outPutFile)
{
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hChildStdinRd, hChildStdinWr;
    CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdInput = hChildStdinRd;
    si.dwFlags |= STARTF_USESTDHANDLES;

    // コマンドプロンプトを起動
    CreateProcess(NULL, (LPSTR)"cmd", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

    
   // const char* text = "fxc.exe screen_ps.hlsl /T ps_5_0 /Fo Test.ps\n";
    const char* text = fileName.c_str();
    DWORD bytesWritten;
    WriteFile(hChildStdinWr, text, strlen(text), &bytesWritten, NULL);

   /* text = "exit\n";
    bytesWritten;
    WriteFile(hChildStdinWr, text, strlen(text), &bytesWritten, NULL);*/
    // 標準入力への書き込み
   /* const char* inputText = "echo Hello from C++!\n";
    DWORD bytesWritten;
    WriteFile(hChildStdinWr, inputText, strlen(inputText), &bytesWritten, NULL);*/

    // パイプを閉じる
    CloseHandle(hChildStdinWr);

    // プロセスの終了を待つ
    //WaitForSingleObject(pi.hProcess, INFINITE);
    Sleep(1);  // 5000:5秒待機（適切な時間に調整してください）

    // ハンドルのクローズ
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hChildStdinRd);
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
        std::filesystem::path fileName = OpenFileDialog()();
        bool result = LoadShaderFile(name,fileName);

        if (result)
        {
            int handle = LoadPixelShaderFromMem(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize());

            shaders_[name].second = handle;

            auto currentPath = std::filesystem::current_path();
            std::string targetPath = currentPath.string() + "\\Shader\\Pixel";
            _chdir(targetPath.c_str());

            //system("cmd");
        //C:\Program Files(x86)\Microsoft DirectX SDK(June 2010)\Utilities\bin\x64\fxc.exe

            //// 標準入力への書き込み
            std::string exe("fxc.exe ");
            std::string fileNames = fileName.filename().stem().string()+".hlsl";
            std::string inputfileDirectry("../../data\\ShaderBinary\\Pixel\\");
            std::string input("T ps_5_0 /Fo " + inputfileDirectry+ fileName.filename().stem().string() + ".ps\n");
            std::string test = exe + fileNames + " /"+input;

            CmdPronpt(test, fileName.filename().stem().string());

            targetPath = currentPath.string();
            _chdir(targetPath.c_str());

            CreateRegisterData(name,std::string("data\\ShaderBinary\\Pixel\\"+ fileName.filename().stem().string() + ".ps"));

            shaders_[name].second = LoadPixelShader(std::string("data\\ShaderBinary\\Pixel\\" + fileName.filename().stem().string() + ".ps").c_str());
        }
    }

    RegisterCustom(name);

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
                    ImGui::DragFloat2((bufferKey.string() + var.second.varName).c_str(), (float*)&var.second.data);
                    cbBuf[0] = var.second.data.x;
                    cbBuf[1] = var.second.data.y;
                    cbBuf += 2;
                }
                if (var.second.typeName == "float")
                {
                    ImGui::Text((bufferKey.string() + var.second.varName).c_str());
                    ImGui::DragFloat((bufferKey.string() + var.second.varName).c_str(), (float*)&var.second.data);
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

        constantBufferMap_[key].swap(registerMap);
}

void ShaderMng::CreateRegisterData(const std::wstring& key, const std::string& psPath)
{
    CreateRegisterData(key, psPath, 0, 0);
}
