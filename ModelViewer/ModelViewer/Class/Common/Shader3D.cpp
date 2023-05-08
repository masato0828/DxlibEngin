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

        //// �����̖����� \0 ���L�ڂ��āA���̌��ɕ\������t�@�C���̎w��A�Ō�� \0\0 ���L�q
        //ofn.lpstrFilter = "Shader to cso File\0;*.cso\0\0";

        //// lpstrFile �Ɏw�肷��z��Ƀt�@�C���̃t���p�X���������܂�
        //ofn.lpstrFile = FullPath;
        //ofn.nMaxFile = sizeof(FullPath);

        //// lpstrFileTitle �Ɏw�肷��z��Ƀt�@�C����( �t�H���_�p�X������ )���������܂�
        //ofn.lpstrFileTitle = FileName;
        //ofn.nMaxFileTitle = sizeof(FileName);

        //ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        //ofn.lpstrDefExt = "";

        //// lpstrTitle �ɕ\�������_�C�A���O�̐����������܂�
        //ofn.lpstrTitle = "�摜�t�@�C���̓ǂݍ���";

        //// �p�X�̎w�肪���������ꍇ�� GetOpenFileName �̖߂�l�� 0 �ȊO�ɂȂ�܂�
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

    // ���_�^�C�v�̏�����
    modelShaderType_ = 0;

    // �I���W�i���V�F�[�_���g��
    if (orgShader_)
    {
        // ���f���Ɋ܂܂��g���C�A���O�����X�g�̐����擾����
        modelListNum_ = MV1GetTriangleListNum(modelHandle);
        
        // ���_�^�C�v�̎擾
        for (int i = 0; i < modelListNum_; i++)
        {
            // �g���C�A���O�����X�g�̒��_�^�C�v
            modelShaderType_ = MV1GetTriangleListVertexType(modelHandle, i);
            break;
        }

        // ���_�V�F�[�_�̃Z�b�g
        VertexSet(vertFileName);

        // �s�N�Z���V�F�[�_�̃Z�b�g
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
    // �s�N�Z���V�F�[�_�p�̒萔�o�b�t�@�̃A�h���X�̎擾
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
    // �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
    UpdateShaderConstantBuffer(IntegerBuffer);

    // �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@��萔�o�b�t�@���W�X�^�ɃZ�b�g
    // �����̎O�Ԗڂ̓��W�X�^�ɐݒ肵�Ă���ԍ�
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
    // ���_�V�F�[�_
    vertexShaderHandle_ = -1;
    // �X�L�j���O�œǂݍ��ޒ��_�V�F�[�_��ς���
    VertexType(vertFileName);

    if (vertexShaderHandle_ != -1)
    {
        SetUseVertexShader(vertexShaderHandle_);
    }
    else
    {
#ifdef _DEBUG
       /* MessageBox(NULL,
            "���_�V�F�[�_���ǂݍ��܂�Ă��Ȃ�����\n�f�t�H���g�̒��_�V�F�[�_���g�p���܂�",
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
        // �P�t���[���̉e�����󂯂钸�_
        vertexShaderHandle_ = LoadVertexShader(vertFileName.c_str());
        break;
    case DX_MV1_VERTEX_TYPE_4FRAME:
        // �P�`�S�t���[���̉e�����󂯂钸�_
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_8FRAME:
        // �T�`�W�t���[���̉e�����󂯂钸�_
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_FREE_FRAME:
        // �X�t���[���ȏ�̉e�����󂯂钸�_
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
        // �@���}�b�v�p�̏�񂪊܂܂��P�t���[���̉e�����󂯂钸�_
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:
        // �@���}�b�v�p�̏�񂪊܂܂��P�`�S�t���[���̉e�����󂯂钸�_
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_8FRAME:
        // �@���}�b�v�p�̏�񂪊܂܂��T�`�W�t���[���̉e�����󂯂钸�_
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NMAP_FREE_FRAME:
        // �@���}�b�v�p�̏�񂪊܂܂��X�t���[���ȏ�̉e�����󂯂钸�_
        vertexShaderHandle_ = -1;
        break;
    case DX_MV1_VERTEX_TYPE_NUM:
        // ���_�^�C�v�̐�
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

    // �s�N�Z���V�F�[�_
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
