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
    /// シェーダーの読み込み
    /// </summary>
    /// <param name="name">map用key値</param>
    /// <param name="vsPath">頂点シェーダのパス</param>
    /// <param name="psPath">ピクセルシェーダのパス</param>
    /// <param name="bufferSize">バッファのサイズ</param>
    /// <returns>読み込み成功：true　失敗：false</returns>
    bool LoadShader(const std::wstring& name, const std::string& vsPath, const std::string& psPath, int bufferSize);

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
    int GetConstansBufferHnadle(const std::wstring& name) { return constansBuffers_[name]->GetHandle(); };

    /// <summary>
    /// テクスチャのセット(DrawBeginの後)
    /// </summary>
    /// <param name="slot">シェーダーのどのスロットにセットするか</param>
    /// <param name="imageHnadle">画像のハンドル</param>
    void SetTexture(SLOT_TYPE slot,int imageHnadle);

    void EndTextere(SLOT_TYPE slot);

    void SetSkiningVertex(const std::wstring& name,const int& modelHandle);

    void Draw(const std::wstring& name,const int& modelHandle);

    bool LoadShaderFile(const std::wstring& name,const std::wstring& filePath);

    void Updater(const std::wstring& name);


private:

    // シェーダーの管理(key値,頂点シェーダハンドル,ピクセルシェーダハンドル)
    std::unordered_map<std::wstring, std::pair<int, int>> shaders_;

    std::unordered_map <std::wstring, std::string> pixelFilePath_;

    // バッファの管理(key値,コンスタントバッファ)
    std::unordered_map<std::wstring, std::unique_ptr<ConstantBuffer>> constansBuffers_;

    ID3DBlob* pPSBlob = nullptr;// ピクセルシェーダー

    ShaderMng() = default;
    ~ShaderMng();
};