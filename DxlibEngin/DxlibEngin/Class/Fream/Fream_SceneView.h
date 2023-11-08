#pragma once
#include "FreamBase.h"
#include "../Common/Vector2.h"
#include <string>
#include <filesystem>
#include <map>

class Fream_SceneView :
    public FreamBase
{
public:
    Fream_SceneView();
    ~Fream_SceneView();
    void Init() override;
    void Update() override;
    /// <summary>
    /// ウィンドウの作成
    /// </summary>
    void Create();

    /// <summary>
    /// 画面割合の係数取得
    /// </summary>
    /// <returns>係数</returns>
    float GetFactor();
    
    /// <summary>
    /// 画像サイズ取得
    /// </summary>
    /// <returns>画像サイズ</returns>
    Vector2 GetDefaultImageSize();
    
    /// <summary>
    /// 画像の左上の取得
    /// </summary>
    /// <returns>画面の左上</returns>
    Vector2Flt GetImageLeftUpCornor();

    /// <summary>
    /// 画像の右下の取得
    /// </summary>
    /// <returns>画面の右下</returns>
    Vector2Flt GetImageRightDownCornor();

    /// <summary>
    /// ウィンドウの真ん中位置の取得
    /// </summary>
    /// <returns>ウィンドウの中央位置</returns>
    Vector2 GetWindowCenterPoint();

    /// <summary>
    /// ウィンドウのサイズの取得
    /// </summary>
    /// <returns>ウィンドウのサイズ</returns>
    Vector2Flt GetScreenSize();

    /// <summary>
    /// ドラッグアンドドロップのパス取得
    /// </summary>
    /// <returns>パス</returns>
    std::filesystem::path GetDropModelPath();

private:

    // 画面割合係数
    float factor_;

    // 画像サイズ
    Vector2 defaultImageSize_;

    // ウィンドウ内の画像の左上
    Vector2Flt imageLeftUpCornor_;
    // ウィンドウ内の画像の右下
    Vector2Flt imageRightDownCornor_;

    // スクリーンのサイズ
    Vector2Flt screenSize_;

    // ドラッグアンドドロップのパス取得用
    std::filesystem::path dropModelPath_;

    // ファイルロードを行ったか
    bool is_FileLoad_;

    /// <summary>
   /// ドラッグアンドドロップしてきたデータを取得
   /// </summary>
    void CreateDragAndDropHandle();
};

