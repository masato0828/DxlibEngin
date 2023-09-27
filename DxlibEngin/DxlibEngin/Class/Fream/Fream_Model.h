#pragma once
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include "FreamBase.h"
#include "../Common/Vector2.h"
#include "../Common/Geometry.h"
#include "../Common/Shader/ShaderMng.h"

enum class COLOR_TYPE
{
    DIF,
    SPC,
    AMB,
    EMI,
};

class Fream_Model :
    public FreamBase
{
public:
    Fream_Model();
    ~Fream_Model();

    /// <summary>
    /// ������
    /// </summary>
    void Init()override;
    
    /// <summary>
    /// �X�V
    /// </summary>
    void Update()override;

    /// <summary>
    /// ���f���f�[�^�̍쐬
    /// </summary>
    /// <param name="path">���f���̃t�@�C���p�X</param>
    void SetModelPath(const std::filesystem::path& path);

    /// <summary>
    /// �`��
    /// </summary>
    /// <param name="cubeTexture">�L���[�u�}�b�v�̃n���h��</param>
    void Draw(int cubeTexture);

    /// <summary>
    /// �J�X�^��
    /// </summary>
    void CustomStatus();

   
    /// <summary>
    /// ���f�����I������Ă邩�ǂ���
    /// </summary>
    /// <returns>�I�𒆁Ftrue�@�I������Ă��Ȃ��Ffalse</returns>
    bool IsModelSelect();
  
    /// <summary>
    /// �ʒu�̎擾
    /// </summary>
    /// <returns>�ʒu</returns>
    Vector3& GetModelPos();

    /// <summary>
    /// �X�J�C�h�[���̕`��
    /// </summary>
    void DrawSkyDome();

private:
    
    /// <summary>
    /// �t���[���̏��
    /// </summary>
    struct ModelFream
    {
        Vector3 ferampos;// �ʒu
        Vector3 feramrot;// ��]
        Vector3 feramscl;// �傫��
    };

    /// <summary>
    /// �}�e���A�����
    /// </summary>
    struct Material
    {
        std::wstring materialName;// �}�e���A���̖��O
        std::map<COLOR_TYPE,COLOR_F> color;// �F
        float spcPower;// �X�y�L�����[�̋���
        int difMapTex;// �f�t���[�X�}�b�v�̃e�N�X�`��
        int spcMapTex;// �X�y�L�����[�}�b�v�̃e�N�X�`��
        int normalMapTex;// �m�[�}���}�b�v�̃e�N�X�`��
    };

    /// <summary>
    /// ���f�����
    /// </summary>
    struct Model
    {
        std::wstring name;// ���f���̖��O
        int handle;// �n���h��
        Vector3 pos;// �ʒu
        Vector3 rot;// ��]
        Vector3 scl;// �傫��
        std::vector<ModelFream>fream;// �t���[�����
        bool isRotation;// ������]�t���O
        std::vector<Material> material;// �}�e���A�����
        std::map<COLOR_TYPE, bool> allChangeColor;// �F���
        std::map < SLOT_TYPE, std::pair<std::string, int>> textureHnadle;// �摜�̃n���h�����
    };

    // ���f���Ǘ�
    std::map<std::wstring, Model> model_;
    // ���f���̃f�t�H���g���
    std::map<std::wstring, Model> defoModelData_;

    /// <summary>
    /// �t���[���c���[
    /// </summary>
    struct FreamData
    {
        FreamData(FreamData* parent, std::wstring name)
        {
            parentFream_ = parent;
            myName = name;
        }
        std::map<std::wstring, FreamData> freamMap_;
        FreamData* parentFream_;
        std::wstring myName;
    };
    FreamData freamData_;
    FreamData* nowSelect_;
    std::wstring nowSelectFreamName_;

    // �t���[���ԍ�
    int freamNumber_;

    // �t���[���c���[�쐬
    void Tree(std::pair<const std::wstring, Model>& handleData,FreamData& freamData,FreamData*& nowSelect);

    // �R���e�L�X�g���j���[�̕\���t���O
    bool contextMenuFlg_;

    // �폜���郂�f���̖��O
    std::wstring deleteModelName_;

    /// <summary>
    /// �t���[���ړ���]�g�k
    /// </summary>
    void Fream();

    /// <summary>
    /// ���ꖼ�̃t�@�C���J�E���g
    /// </summary>
    /// <param name="fileName">�t�@�C����</param>
    /// <returns></returns>
    int FileCnt(const std::wstring& fileName);

    /// <summary>
    /// �R���e�L�X�g���j���[�̕\��
    /// </summary>
    void ContextMenu();

    /// <summary>
    /// �J���[�̕ύX
    /// </summary>
    /// <param name="tagName">�c���[�̖��O</param>
    /// <param name="type">�J���[�^�C�v</param>
    void ColorEdit(const std::string& tagName, COLOR_TYPE type);

    /// <summary>
    /// ���ׂẴJ���[�̕ύX
    /// </summary>
    /// <param name="tagName">�c���[�̖��O</param>
    void AllColorEdit(const std::string& tagName);

    /// <summary>
    /// �V�F�[�_�[�̓ǂݍ���
    /// </summary>
    /// <param name="name">�L�[�l</param>
    void LoadShaderProc(const std::wstring& name);

    /// <summary>
    /// �}�e���A�����̍쐬
    /// </summary>
    /// <param name="handle">���f���̃n���h��</param>
    /// <returns>�}�e���A�����</returns>
    std::vector<Material> CreateMaterialData(const int& handle);

    /// <summary>
    /// �e�N�X�`���摜�̓ǂݍ���
    /// </summary>
    /// <param name="tagName">�\���p�̖��O</param>
    /// <param name="type">�摜�̃^�C�v</param>
    void LoadTexture(std::string tagName,SLOT_TYPE type);

    // ���ꖼ���f���̃J�E���g
    int handleCnt_;

    // �X�J�C�h�[�����f���̃n���h��
    int skyDomeHnadle_;
    
};

