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
    /// シェーダーの読み込み
    /// </summary>
    /// <param name="name">map用key値</param>
    /// <param name="vsPath">頂点シェーダのパス</param>
    /// <param name="psPath">ピクセルシェーダのパス</param>
    /// <param name="bufferSize">バッファのサイズ</param>
    /// <returns>読み込み成功：true　失敗：false</returns>
    bool LoadShader(const std::string& name, const std::string& vsPath, const std::string& psPath, int bufferSize);

    /// <summary>
    /// 指定されたシェーダーハンドル、バッファの解放
    /// </summary>
    /// <param name="name">key値</param>
    void UnloadShader(const std::string& name);

    /// <summary>
    /// 全てのシェーダーハンドル、バッファの解放
    /// </summary>
    void Clear();

    /// <summary>
    /// 頂点シェーダハンドルの取得
    /// </summary>
    /// <param name="name">key値</param>
    /// <returns>成功：ハンドル  失敗：-1</returns>
    int GetShaderVSHandle(const std::string& name) const;

    /// <summary>
    /// ピクセルシェーダハンドルの取得
    /// </summary>
    /// <param name="name">key値</param>
    /// <returns>成功：ハンドル  失敗：-1</returns>
    int GetShaderPSHandle(const std::string& name) const;

    /// <summary>
    /// シェーダーを使用した描画の開始
    /// </summary>
    /// <param name="name">key値</param>
    void DrawBegin(const std::string& name);

    /// <summary>
    /// シェーダーを使用した描画の終了
    /// </summary>
    void DrawEnd();

    /// <summary>
    /// バッファハンドルの取得
    /// </summary>
    /// <param name="name">key値</param>
    /// <returns>バッファハンドル</returns>
    int GetConstansBufferHnadle(const std::string& name) { return constansBuffers_[name]->GetHandle(); };

    /// <summary>
    /// テクスチャのセット(DrawBeginの後)
    /// </summary>
    /// <param name="slot">シェーダーのどのスロットにセットするか</param>
    /// <param name="imageHnadle">画像のハンドル</param>
    void SetTexture(int slot,int imageHnadle);

private:

    // シェーダーの管理(key値,頂点シェーダハンドル,ピクセルシェーダハンドル)
    std::unordered_map<std::string, std::pair<int, int>> shaders_;

    // バッファの管理(key値,コンスタントバッファ)
    std::unordered_map<std::string, std::unique_ptr<ConstantBuffer>> constansBuffers_;

    ShaderMng() = default;
    ~ShaderMng();
};