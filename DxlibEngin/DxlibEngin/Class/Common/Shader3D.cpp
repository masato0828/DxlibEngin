#include <Dxlib.h>
#include "Shader3D.h"

Shader3D::Shader3D(PixelShaderType type,int model)
{
    if (type == PixelShaderType::Texture)
    {
        //int GrHandle = 0;
        //char FullPath[MAX_PATH], FileName[MAX_PATH];

        //OPENFILENAME ofn;

        //memset(&ofn, 0, sizeof(OPENFILENAME));
        //memset(FullPath, 0, sizeof(FullPath));
        //memset(FileName, 0, sizeof(FileName));
        //ofn.lStructSize = sizeof(OPENFILENAME);
        //ofn.hwndOwner = GetMainWindowHandle();

        //// 説明の末尾に \0 を記載して、その後ろに表示するファイルの指定、最後に \0\0 を記述
        //ofn.lpstrFilter = "Shader to cso File\0;*.cso\0\0";

        //// lpstrFile に指定する配列にファイルのフルパスが代入されます
        //ofn.lpstrFile = FullPath;
        //ofn.nMaxFile = sizeof(FullPath);

        //// lpstrFileTitle に指定する配列にファイル名( フォルダパスが無い )が代入されます
        //ofn.lpstrFileTitle = FileName;
        //ofn.nMaxFileTitle = sizeof(FileName);

        //ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        //ofn.lpstrDefExt = "";

        //// lpstrTitle に表示されるダイアログの説明を代入します
        //ofn.lpstrTitle = "画像ファイルの読み込み";

        //// パスの指定が成功した場合は GetOpenFileName の戻り値は 0 以外になります
        //if (GetOpenFileName(&ofn) != 0)
        //{
        //    
        //    //Init(model, FullPath, "x64/Debug/NormTexturePixelShader.cso");
        //}

        Init(model, "x64/Debug/NonFreamVertexShader.cso", "x64/Debug/NormTexturePixelShader.cso");
    }
    else
    {
        Init(model, "", "");
    }
    addImage_ = -1;
    addImage2_ = -1;
    addImage3_ = -1;
    addImage4_ = -1;
}

Shader3D::Shader3D(
    int modelHandle, 
    std::string vertFileName,
    std::string pixelFileName,
    std::string addImageName,
    std::string addImageName2,
    std::string addImageName3,
    std::string addImageName4
)
{

    AddImageLoad(addImageName, addImageName2, addImageName3, addImageName4);

    Init(modelHandle, vertFileName, pixelFileName);

    
}

void Shader3D::AddImageLoad(std::string& addImageName, std::string& addImageName2, std::string& addImageName3, std::string& addImageName4)
{
    addImage_ = -1;
    addImage2_ = -1;
    addImage3_ = -1;
    addImage4_ = -1;

    if (addImageName != "")
    {
        addImage_ = LoadGraph(addImageName.c_str());
    }
    if (addImageName2 != "")
    {
        addImage2_ = LoadGraph(addImageName.c_str());
    }
    if (addImageName3 != "")
    {
        addImage3_ = LoadGraph(addImageName.c_str());
    }
    if (addImageName4 != "")
    {
        addImage4_ = LoadGraph(addImageName.c_str());
    }
}

void Shader3D::AddImage(int& imageHandle, int& imageHandle2, int& imageHandle3, int& imageHandle4)
{
    addImage_ = imageHandle;
    addImage2_ = imageHandle2;
    addImage3_ = imageHandle3;
    addImage4_ = imageHandle4;
}

Shader3D::~Shader3D()
{
    DeleteShader(vertexShaderHandle_);
    DeleteShader(pixelShaderHandle_);
}

void Shader3D::ShaderSetUp(int DX_DIRECT3D_)
{
    SetUseDirect3DVersion(DX_DIRECT3D_);
}

void Shader3D::Init(int modelHandle, std::string vertFileName, std::string pixelFileName)
{
    orgShader_ = true;

    // 頂点タイプの初期化
    modelShaderType_ = 0;

    // オリジナルシェーダを使う
    if (orgShader_)
    {
        // モデルに含まれるトライアングルリストの数を取得する
        modelListNum_ = MV1GetTriangleListNum(modelHandle);
        
        // 頂点タイプの取得
        for (int i = 0; i < modelListNum_; i++)
        {
            // トライアングルリストの頂点タイプ
            modelShaderType_ = MV1GetTriangleListVertexType(modelHandle, i);
            break;
        }

        // 頂点シェーダのセット
        VertexSet(vertFileName);

        // ピクセルシェーダのセット
        PixelSet(pixelFileName);
    }
}

void Shader3D::CreateBuffer4(int& IntegerBuffer)
{
    IntegerBuffer = CreateShaderConstantBuffer(sizeof(float) * 4);
}

void Shader3D::CreateBuffer8(int& IntegerBuffer)
{
    IntegerBuffer = CreateShaderConstantBuffer(sizeof(float) * 8);
}

void Shader3D::SetConstantBuffer(int& IntegerBuffer, float registerFloat1, float registerFloat2, float registerFloat3, float registerFloat4)
{
    // ピクセルシェーダ用の定数バッファのアドレスの取得
    float* cbBuf = (float*)GetBufferShaderConstantBuffer(IntegerBuffer);

    cbBuf[0] = registerFloat1;
    if (registerFloat2 != NULL)
    {
        cbBuf[1] = registerFloat2;
    }
    if (registerFloat3 != NULL)
    {
        cbBuf[2] = registerFloat3;
    }
    if (registerFloat4 != NULL)
    {
        cbBuf[3] = registerFloat4;
    }
}

void Shader3D::Update(int IntegerBuffer, int registerNo)
{
    // ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
    UpdateShaderConstantBuffer(IntegerBuffer);

    // ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
    // 引数の三番目はレジスタに設定している番号
    SetShaderConstantBuffer(IntegerBuffer, DX_SHADERTYPE_PIXEL, registerNo);
}

void Shader3D::Draw(void)
{
    MV1SetUseOrigShader(true);
}

void Shader3D::DrawEnd(void)
{
    MV1SetUseOrigShader(false);
    
}

void Shader3D::VertexSet(std::string vertFileName)
{
    // 頂点シェーダ
    vertexShaderHandle_ = -1;
    // スキニングで読み込む頂点シェーダを変える
    VertexType(vertFileName);

    if (vertexShaderHandle_ != -1)
    {
        SetUseVertexShader(vertexShaderHandle_);
    }
    else
    {
#ifdef _DEBUG
       /* MessageBox(NULL,
            "頂点シェーダが読み込まれていないため\nデフォルトの頂点シェーダを使用します",
            "",MB_OK);*/
#endif // _DEBUG
        vertexShaderHandle_ = LoadVertexShader(vertFileName.c_str());
        SetUseVertexShader(vertexShaderHandle_);
    }
}

void Shader3D::VertexType(std::string& vertFileName)
{
    switch (modelShaderType_)
    {
    case DX_MV1_VERTEX_TYPE_1FRAME:
        // １フレームの影響を受ける頂点
        vertexShaderHandle_ = LoadVertexShader(vertFileName.c_str());
        break;
    case DX_MV1_VERTEX_TYPE_4FRAME:
        // １～４フレームの影響を受ける頂点
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_8FRAME:
        // ５～８フレームの影響を受ける頂点
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_FREE_FRAME:
        // ９フレーム以上の影響を受ける頂点
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
        // 法線マップ用の情報が含まれる１フレームの影響を受ける頂点
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:
        // 法線マップ用の情報が含まれる１～４フレームの影響を受ける頂点
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_8FRAME:
        // 法線マップ用の情報が含まれる５～８フレームの影響を受ける頂点
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_FREE_FRAME:
        // 法線マップ用の情報が含まれる９フレーム以上の影響を受ける頂点
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NUM:
        // 頂点タイプの数
        vertexShaderHandle_ = -1;
        break;
    default:
        break;
    }
}

void Shader3D::PixelSet(std::string pixelFileName)
{
    //"x64/Debug/PixelShader.cso"
    pixelShaderHandle_ = LoadPixelShader(pixelFileName.c_str());

    // ピクセルシェーダ
    SetUsePixelShader(pixelShaderHandle_);

    ShaderTexture();

}

void Shader3D::ShaderTexture()
{
    if (addImage_ != -1)
    {
        SetUseTextureToShader(1, addImage_);
    }
    if (addImage2_ != -1)
    {
        SetUseTextureToShader(2, addImage2_);
    }
    if (addImage3_ != -1)
    {
        SetUseTextureToShader(3, addImage3_);
    }
    if (addImage4_ != -1)
    {
        SetUseTextureToShader(4, addImage4_);
    }
}
