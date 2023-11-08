#pragma once
#include <memory>
#include <string>
#include <DxLib.h>
#include <unordered_map>
#include "ConstantBuffer.h"
#include "../../../nlohmann/json.hpp"
#include <wrl/client.h>
#include <assert.h>
#include <vector>
#include <map>

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
    SUB_0,
    SUB_1,
    SUB_2,
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
    /// シェーダーの読み込み
    /// </summary>
    /// <param name="name">map用key値</param>
    /// <param name="vsPath">頂点シェーダのパス</param>
    /// <param name="psPath">ピクセルシェーダのパス</param>
    /// <param name="bufferSize">バッファのサイズ</param>
    /// <returns>読み込み成功：true　失敗：false</returns>
    bool LoadShader(const std::wstring& name, const std::string& vsPath, const std::string& psPath, int registerNumber,int registerNumberLoss);
    bool LoadShader(const std::wstring& name, const std::string& vsPath, const std::string& psPath,int bufferSize);

    /// <summary>
    /// 指定されたシェーダーハンドル、バッファの解放
    /// </summary>
    /// <param name="name">key値</param>
    void UnloadShader(const std::wstring& name);

    /// <summary>
    /// 全てのシェーダーハンドル、バッファの解放
    /// </summary>
    void Clear();

    /// <summary>
    /// 頂点シェーダハンドルの取得
    /// </summary>
    /// <param name="name">key値</param>
    /// <returns>成功：ハンドル  失敗：-1</returns>
    int GetShaderVSHandle(const std::wstring& name) const;

    /// <summary>
    /// ピクセルシェーダハンドルの取得
    /// </summary>
    /// <param name="name">key値</param>
    /// <returns>成功：ハンドル  失敗：-1</returns>
    int GetShaderPSHandle(const std::wstring& name) const;

    /// <summary>
    /// シェーダーを使用した描画の開始
    /// </summary>
    /// <param name="name">key値</param>
    void DrawBegin(const std::wstring& name);

    /// <summary>
    /// シェーダーを使用した描画の終了
    /// </summary>
    void DrawEnd();

    /// <summary>
    /// バッファハンドルの取得
    /// </summary>
    /// <param name="name">key値</param>
    /// <returns>バッファハンドル</returns>
    int GetConstansBufferHnadle(const std::wstring& constansBufferMapKey) { return constansBuffers_[constansBufferMapKey]->GetHandle(); };

    /// <summary>
    /// テクスチャのセット(DrawBeginの後)
    /// </summary>
    /// <param name="slot">シェーダーのどのスロットにセットするか</param>
    /// <param name="imageHnadle">画像のハンドル</param>
    void SetModelTexture(SLOT_TYPE slot,int imageHnadle);
    void SetTexture(int slotNum, int imageHnadle);

    /// <summary>
    /// テクスチャの削除
    /// </summary>
    /// <param name="slot">スロット番号</param>
    void EndTextere(SLOT_TYPE slot);
    void EndTexture(int slotNum);

    /// <summary>
    /// モデルの頂点タイプで読み込むシェーダーを変える
    /// </summary>
    /// <param name="name">シェーダーマップのキー値</param>
    /// <param name="modelHandle">読み込むモデルハンドル</param>
    void SetSkiningVertex(const std::wstring& shaderMapKey,const int& modelHandle);

    /// <summary>
    /// ピクセルシェーダーのコンパイル
    /// </summary>
    /// <param name="name">シェーダーマップのキー値</param>
    /// <param name="filePath">読み込むシェーダーのパス</param>
    /// <returns>成功：true 失敗：false</returns>
    bool LoadShaderFile(const std::wstring& shaderMapKey,const std::wstring& filePath);


    /// <summary>
    /// シェーダーのカスタム用
    /// </summary>
    /// <param name="name">ファイルパスマップのキー値</param>
    void Updater(const std::wstring& fileMapKey);

    void RegisterCustom(const std::wstring& key);
    void RegisterUpdate(const std::wstring& key);
    void RegisterUpdate();

    void CreateRegisterData(const std::wstring& key, const std::string& psPath, const int registerNumber, const int registerNumberLoss);
    void CreateRegisterData(const std::wstring& key, const std::string& psPath);

    /// <summary>
    /// registerデータファイルの出力
    /// </summary>
    /// <param name="key">キー値</param>
    void ExportFile(const std::wstring& key);

    /// <summary>
    /// registerデータファイルの読み込み
    /// </summary>
    /// <param name="filePath">ファイルパス</param>
    void ImportFile(const std::string& filePath);

    /// <summary>
    /// jsonファイルからのデータ取得
    /// </summary>
    /// <param name="json"></param>
    void GetContents(nlohmann::json json);
private:

    // シェーダーの管理(key値,頂点シェーダハンドル,ピクセルシェーダハンドル)
    std::unordered_map<std::wstring, std::pair<int, int>> shaders_;

    std::unordered_map <std::wstring, std::string> pixelFilePath_;

    // バッファの管理(key値,コンスタントバッファ)
    std::unordered_map<std::wstring, std::unique_ptr<ConstantBuffer>> constansBuffers_;

    ID3DBlob* pPSBlob = nullptr;// ピクセルシェーダー

    ShaderMng() = default;
    ~ShaderMng();

    /// <summary>
    /// バッファデータ構造体
    /// </summary>
    struct BufferData
    {
        std::string typeName;   // 変数の名前
        std::string varName;    // 変数のタイプ(float等)
        size_t varSize;         // 変数のサイズ
        FLOAT4 data;            // データ
        BufferData() {};
        BufferData(std::string t, std::string v, size_t vs, FLOAT4 d)
        {
            typeName = t;
            varName = v;
            varSize = vs;
            data = d;
        }
    };

    /// <summary>
    /// registerデータ構造体
    /// </summary>
    struct RegisterData
    {
        int bufferHandle;   //バッファハンドル
        int registerNumber; // registerに割り当てた番号
        std::unordered_map<std::string, BufferData> bufferData;  // バッファデータ
        RegisterData() {};
        RegisterData(int b, int r, std::unordered_map<std::string, BufferData> bd)
        {
            bufferHandle = b;
            registerNumber = r;
            bufferData = bd;
        }

    };

    /// <summary>
    /// シェーダデータ構造体
    /// </summary>
    struct ShaderDatas
    {
        std::string filePath;  //pixelshaderのファイルパス
        std::map<std::string, RegisterData> registerMap; // registerのデータ
    };

    /// <summary>
    /// コンスタントバッファ管理
    /// </summary>
    std::map<std::wstring, ShaderDatas> constantBufferMap_;

    std::vector<std::string> registerName_;

    void CmdPronpt(std::string fileName, std::string outPutFile);



public:
    std::unordered_map<std::string, ShaderMng::BufferData>& DataAcsess(const std::wstring& key, const std::string& registerMapKey);
};