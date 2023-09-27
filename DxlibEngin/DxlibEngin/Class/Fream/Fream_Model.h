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
    /// 初期化
    /// </summary>
    void Init()override;
    
    /// <summary>
    /// 更新
    /// </summary>
    void Update()override;

    /// <summary>
    /// モデルデータの作成
    /// </summary>
    /// <param name="path">モデルのファイルパス</param>
    void SetModelPath(const std::filesystem::path& path);

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="cubeTexture">キューブマップのハンドル</param>
    void Draw(int cubeTexture);

    /// <summary>
    /// カスタム
    /// </summary>
    void CustomStatus();

   
    /// <summary>
    /// モデルが選択されてるかどうか
    /// </summary>
    /// <returns>選択中：true　選択されていない：false</returns>
    bool IsModelSelect();
  
    /// <summary>
    /// 位置の取得
    /// </summary>
    /// <returns>位置</returns>
    Vector3& GetModelPos();

    /// <summary>
    /// スカイドームの描画
    /// </summary>
    void DrawSkyDome();

private:
    
    /// <summary>
    /// フレームの情報
    /// </summary>
    struct ModelFream
    {
        Vector3 ferampos;// 位置
        Vector3 feramrot;// 回転
        Vector3 feramscl;// 大きさ
    };

    /// <summary>
    /// マテリアル情報
    /// </summary>
    struct Material
    {
        std::wstring materialName;// マテリアルの名前
        std::map<COLOR_TYPE,COLOR_F> color;// 色
        float spcPower;// スペキュラーの強さ
        int difMapTex;// デフュースマップのテクスチャ
        int spcMapTex;// スペキュラーマップのテクスチャ
        int normalMapTex;// ノーマルマップのテクスチャ
    };

    /// <summary>
    /// モデル情報
    /// </summary>
    struct Model
    {
        std::wstring name;// モデルの名前
        int handle;// ハンドル
        Vector3 pos;// 位置
        Vector3 rot;// 回転
        Vector3 scl;// 大きさ
        std::vector<ModelFream>fream;// フレーム情報
        bool isRotation;// 自動回転フラグ
        std::vector<Material> material;// マテリアル情報
        std::map<COLOR_TYPE, bool> allChangeColor;// 色情報
        std::map < SLOT_TYPE, std::pair<std::string, int>> textureHnadle;// 画像のハンドル情報
    };

    // モデル管理
    std::map<std::wstring, Model> model_;
    // モデルのデフォルト情報
    std::map<std::wstring, Model> defoModelData_;

    /// <summary>
    /// フレームツリー
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

    // フレーム番号
    int freamNumber_;

    // フレームツリー作成
    void Tree(std::pair<const std::wstring, Model>& handleData,FreamData& freamData,FreamData*& nowSelect);

    // コンテキストメニューの表示フラグ
    bool contextMenuFlg_;

    // 削除するモデルの名前
    std::wstring deleteModelName_;

    /// <summary>
    /// フレーム移動回転拡縮
    /// </summary>
    void Fream();

    /// <summary>
    /// 同一名のファイルカウント
    /// </summary>
    /// <param name="fileName">ファイル名</param>
    /// <returns></returns>
    int FileCnt(const std::wstring& fileName);

    /// <summary>
    /// コンテキストメニューの表示
    /// </summary>
    void ContextMenu();

    /// <summary>
    /// カラーの変更
    /// </summary>
    /// <param name="tagName">ツリーの名前</param>
    /// <param name="type">カラータイプ</param>
    void ColorEdit(const std::string& tagName, COLOR_TYPE type);

    /// <summary>
    /// すべてのカラーの変更
    /// </summary>
    /// <param name="tagName">ツリーの名前</param>
    void AllColorEdit(const std::string& tagName);

    /// <summary>
    /// シェーダーの読み込み
    /// </summary>
    /// <param name="name">キー値</param>
    void LoadShaderProc(const std::wstring& name);

    /// <summary>
    /// マテリアル情報の作成
    /// </summary>
    /// <param name="handle">モデルのハンドル</param>
    /// <returns>マテリアル情報</returns>
    std::vector<Material> CreateMaterialData(const int& handle);

    /// <summary>
    /// テクスチャ画像の読み込み
    /// </summary>
    /// <param name="tagName">表示用の名前</param>
    /// <param name="type">画像のタイプ</param>
    void LoadTexture(std::string tagName,SLOT_TYPE type);

    // 同一名モデルのカウント
    int handleCnt_;

    // スカイドームモデルのハンドル
    int skyDomeHnadle_;
    
};

