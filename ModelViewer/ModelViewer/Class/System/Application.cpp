#include <Dxlib.h>
#include "Application.h"
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_impl_dx11.h"
#include "../../imGui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	return 0;
}

Application::Application()
{
}

Application::~Application()
{
}

bool Application::Run()
{
	// システムの初期化
	if (!SysInit())
	{
		return false;
	}

	// 各クラス＆各変数初期化
	if (!Init())
	{
		return false;
	}

	// メインループ
	while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 更新
		freamMng_->Update();

		

		freamMng_->Draw();

		ScreenFlip();

		// 描画
		freamMng_->Render();
	}

	freamMng_->ShutDown();

	// Dxlibの終了
	DxLib_End();

	return true;
}

bool Application::SysInit()
{
	// 描画画面のサイズに対するメインウインドウサイズの比率を設定する(ディスプレイ拡大率)
	SetWindowSizeExtendRate(1.0);
	int DesktopW, DesktopH;
	// 起動時のデスクトップの画面情報を取得する
	GetDefaultState(&DesktopW, &DesktopH, NULL);
	// 画面モードの変更
	SetGraphMode(DesktopW, DesktopH, 32);
	// ウィンドウモードと全画面モードの切り替え
	ChangeWindowMode(true);
	// 背景色
	SetBackgroundColor(60, 60, 60, 255);
	// Direct3Dのバージョン
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	// ウィンドウプロシージャルの設定
	SetHookWinProc(WndProc);
	// ウィンドウを表示するかどうか
	SetWindowVisibleFlag(false);
	// アプリが非アクティブ状態でも処理を実行するかどうかを設定する( TRUE:実行する  FALSE:停止する( デフォルト ) )
	SetAlwaysRunFlag(true);
	// Dxlibの初期化
	if (DxLib_Init() == -1)
	{
		return false;
	}

	return true;
}

bool Application::Init()
{
	freamMng_ = std::make_unique<FreamMng>();

	return true;
}
