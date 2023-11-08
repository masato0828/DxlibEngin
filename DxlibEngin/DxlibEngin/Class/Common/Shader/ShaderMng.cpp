#include <DxLib.h>
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

std::unordered_map<std::string, ShaderMng::BufferData>& ShaderMng::DataAcsess(const std::wstring& key, const std::string& registerMapKey)
{
    return constantBufferMap_[key].registerMap[registerMapKey].bufferData;
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
    //ImGui::Begin("register");
    std::filesystem::path bufferKey = key;

    for (auto& b : constantBufferMap_[key].registerMap)
    {
        // シェーダー用定数バッファハンドルの定数バッファのアドレスを取得する
        auto* cbBuf = (float*)GetBufferShaderConstantBuffer(b.second.bufferHandle);
        for (auto& var : b.second.bufferData)
        {
            if (var.second.typeName == "float4")
            {
                ImGui::Text((bufferKey.string() + var.second.varName).c_str());
                ImGuiCustom::ColorEdit4(("##" + bufferKey.string() + var.second.varName).c_str(), &var.second.data);
                cbBuf[0] = var.second.data.x;
                cbBuf[1] = var.second.data.y;
                cbBuf[2] = var.second.data.z;
                cbBuf[3] = var.second.data.w;
                cbBuf += 4;
            }
            if (var.second.typeName == "float3")
            {
                ImGui::Text((bufferKey.string() + var.second.varName).c_str());
                ImGuiCustom::ColorEdit3(("##" + bufferKey.string() + var.second.varName).c_str(), &var.second.data);
                cbBuf[0] = var.second.data.x;
                cbBuf[1] = var.second.data.y;
                cbBuf[2] = var.second.data.z;
                cbBuf[3] = var.second.data.w;
                cbBuf += 4;
            }
            if (var.second.typeName == "float2")
            {
                ImGui::Text((bufferKey.string() + var.second.varName).c_str());
                ImGui::DragFloat2(("##" + bufferKey.string() + var.second.varName).c_str(), (float*)&var.second.data, 0.1f);
                cbBuf[0] = var.second.data.x;
                cbBuf[1] = var.second.data.y;
                cbBuf += 2;
            }
            if (var.second.typeName == "float")
            {
                ImGui::Text((bufferKey.string() + var.second.varName).c_str());
                ImGui::DragFloat(("##" + bufferKey.string() + var.second.varName).c_str(), (float*)&var.second.data, 0.1f);
                cbBuf[0] = var.second.data.x;
                cbBuf += 1;
            }
        }
        // シェーダー用定数バッファハンドルの定数バッファへの変更を適用する
        UpdateShaderConstantBuffer(b.second.bufferHandle);
        // シェーダー用定数バッファハンドルの定数バッファを指定のシェーダーの指定のスロットにセットする
        SetShaderConstantBuffer(b.second.bufferHandle, DX_SHADERTYPE_PIXEL, b.second.registerNumber);
    }

    ImGui::Separator();
    if (ImGui::Button("output"))
    {
        ExportFile(key);
    }

    if (ImGui::Button("input"))
    {
        ImportFile(OpenFileDialog()().string());
    }
    //ImGui::End();
}

void ShaderMng::RegisterUpdate(const std::wstring& key)
{
    std::filesystem::path bufferKey = key;

    for (auto& b : constantBufferMap_[key].registerMap)
    {
        // シェーダー用定数バッファハンドルの定数バッファのアドレスを取得する
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
            // シェーダー用定数バッファハンドルの定数バッファへの変更を適用する
            UpdateShaderConstantBuffer(b.second.bufferHandle);
            // シェーダー用定数バッファハンドルの定数バッファを指定のシェーダーの指定のスロットにセットする
            SetShaderConstantBuffer(b.second.bufferHandle, DX_SHADERTYPE_PIXEL, b.second.registerNumber);
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

    D3D12_SHADER_DESC desc{};
    shaderRef->GetDesc(&desc);

    std::unordered_map<std::string, RegisterData> registerMap;
    ShaderDatas shaderData;

    const auto cbCount = desc.ConstantBuffers;
    for (auto i = registerNumberLoss; i < cbCount; ++i)
    {
        D3D12_SHADER_BUFFER_DESC shaderBuffer{};
        auto cbuffer = shaderRef->GetConstantBufferByIndex(i);
        cbuffer->GetDesc(&shaderBuffer);

        std::unordered_map<std::string, BufferData> bufferData;
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

            // データ取得
            std::string varName = varDesc.Name;
            std::string var = typeDesc.Name;
            size_t varSize = varDesc.Size;
            std::string Name = varDesc.Name;

            // バッファデータの格納
            bufferData.emplace(varName.c_str(), BufferData(var, varName, varSize, {1,1,1,1}));

            // registerデータの格納
            registerData = RegisterData(constantHandle, i + registerNumber, bufferData);
        }

        // シェーダーデータの格納
        shaderData.registerMap.emplace(shaderBuffer.Name, registerData);
    }
    shaderData.filePath = psPath;

    // コンスタントバッファのごとの格納
    constantBufferMap_.emplace(key, shaderData);
}

void ShaderMng::CreateRegisterData(const std::wstring& key, const std::string& psPath)
{
    CreateRegisterData(key, psPath, 0, 0);
}

void ShaderMng::ExportFile(const std::wstring& key)
{
    std::ofstream writing_file;
    std::string filename = "outputfile.json";
    auto file = ExpoterFileDialog()(nullptr, &filename);
    writing_file.open(file.string().c_str(), std::ios::out);

    nlohmann::json j;

    std::filesystem::path fileKey = key;

    for (auto& b : constantBufferMap_[key].registerMap)
    {
        j[fileKey.string().c_str()]["path"] = constantBufferMap_[key].filePath;
        j[fileKey.string().c_str()]["registerNumber"] = b.second.registerNumber;
        for (auto& var : b.second.bufferData)
        {
            j[fileKey.string().c_str()][b.first.c_str()][var.second.varName.c_str()]["typeName"]
                = var.second.typeName;
            j[fileKey.string().c_str()][b.first.c_str()][var.second.varName.c_str()]["varName"]
                = var.second.varName;
            j[fileKey.string().c_str()][b.first.c_str()][var.second.varName.c_str()]["varSize"]
                = var.second.varSize;
            j[fileKey.string().c_str()][b.first.c_str()][var.second.varName.c_str()]["data"]
                = {
                var.second.data.x ,
                var.second.data.y ,
                var.second.data.z ,
                var.second.data.w
            };
        }
    }

    writing_file << std::setw(4) << j << std::endl;

    writing_file.close();
}

void ShaderMng::ImportFile(const std::string& filePath)
{
    std::ifstream reading(filePath, std::ios::in);
    if (reading.is_open())
    {
        nlohmann::json j;
        reading >> j;

        GetContents(j);
    }
}

void ShaderMng::GetContents(nlohmann::json json)
{
    std::string shaderPSpath;
    int registerNumber = 0;
    std::string typeName;
    FLOAT4 data = { 0,0,0,0 };
    std::string varName;
    size_t varSize = 0;
    std::unordered_map<std::string, BufferData> bufferDataMap;
    std::map<std::string, RegisterData> registerDataMap;
    ShaderDatas shaderData;
    std::string shaderAcessKey;
    std::string jsonSecondLevelName;
    std::string jsonThirdLevelName;
    std::string jsonForceLevelName;

    std::string cbufferName;

    /// json1階層目{}
    for (nlohmann::json::iterator firstMap = json.begin(); firstMap != json.end(); ++firstMap)
    {
        shaderAcessKey = firstMap.key();
        /// json2階層目{}
        for (nlohmann::json::iterator secondMap = (*firstMap).begin(); secondMap != (*firstMap).end(); ++secondMap)
        {
            jsonSecondLevelName = secondMap.key();
            if (cbufferName.empty())
            {
                cbufferName = jsonSecondLevelName;
            }

            if (jsonSecondLevelName == "path")
            {
                shaderPSpath = secondMap.value().get<std::string>();
            }
            else if (jsonSecondLevelName == "registerNumber")
            {
                registerNumber = secondMap.value().get<int>();
            }
            else
            {
                /// json3階層目{}
                for (nlohmann::json::iterator tihrdMap = (*secondMap).begin(); tihrdMap != (*secondMap).end(); ++tihrdMap)
                {
                    jsonThirdLevelName = tihrdMap.key();
                    /// json4階層目{}
                    for (nlohmann::json::iterator forceMap = (*tihrdMap).begin(); forceMap != (*tihrdMap).end(); ++forceMap)
                    {
                        jsonForceLevelName = forceMap.key();
                        if (jsonForceLevelName == "data")
                        {
                            data.x = forceMap.value()[0].get<float>();
                            data.y = forceMap.value()[1].get<float>();
                            data.z = forceMap.value()[2].get<float>();
                            data.w = forceMap.value()[3].get<float>();
                        }
                        else if (jsonForceLevelName == "typeName")
                        {
                            typeName = forceMap.value().get<std::string>();
                        }
                        else if (jsonForceLevelName == "varName")
                        {
                            varName = forceMap.value().get<std::string>();
                        }
                        else if (jsonForceLevelName == "varSize")
                        {
                            varSize = forceMap.value().get<int>();
                        }
                    }
                    BufferData bufferData = { typeName,varName,varSize,data };
                    bufferDataMap.emplace(jsonThirdLevelName, bufferData);
                }
            }

        }
        RegisterData registerData
            = { LoadPixelShader(shaderPSpath.c_str()),registerNumber, bufferDataMap };
        registerDataMap.emplace(cbufferName, registerData);
    }
    shaderData = { shaderPSpath,registerDataMap };

    std::filesystem::path mapkey = shaderAcessKey;

    // データが入ってる？
    auto it = constantBufferMap_.find(mapkey.wstring());
    if (it != constantBufferMap_.end())
    {
        // 入っている
        for (auto& buff : bufferDataMap)
        {
            // 入っているデータを変更
            constantBufferMap_[mapkey.wstring()].registerMap[cbufferName].bufferData[buff.first].data = buff.second.data;
        }
    }
    else
    {
        // 入っていない
        constantBufferMap_.emplace(mapkey.wstring(), shaderData);
    }
}
