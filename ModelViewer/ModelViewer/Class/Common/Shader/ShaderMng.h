#pragma once
#include <memory>
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


    bool LoadShader(const std::string& name, const std::string& vsPath, const std::string& psPath, int bufferSize);

    void UnloadShader(const std::string& name);

    void Clear();

    int GetShaderVSHandle(const std::string& name) const;

    int GetShaderPSHandle(const std::string& name) const;

    void Draw(const std::string& name);
    void DrawEnd();

    int GetConstansBufferHnadle(const std::string& name) { return constansBuffers_[name]->GetHandle(); };

    void SetTexture(int slot,int imageHnadle);

private:
    std::unordered_map<std::string, std::pair<int, int>> shaders_;
    std::unordered_map<std::string, std::unique_ptr<ConstantBuffer>> constansBuffers_;

    ShaderMng() = default;
    ~ShaderMng();
};