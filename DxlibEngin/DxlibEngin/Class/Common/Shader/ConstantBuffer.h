#pragma once

class ConstantBuffer
{
public:
    ConstantBuffer() : bufferHandle_(-1) {}
    ~ConstantBuffer();

    /// <summary>
    /// バッファの作成
    /// </summary>
    /// <param name="bufferSize">バッファのサイズ</param>
    /// <returns>成功：true　失敗：false</returns>
    bool Create(int bufferSize);

    /// <summary>
    /// バッファの解放
    /// </summary>
    void Release();

    /// <summary>
    /// バッファの取得
    /// </summary>
    /// <returns>バッファのハンドル</returns>
    int GetHandle() const { return bufferHandle_; }

private:
    // バッファのハンドル
    int bufferHandle_;
};
