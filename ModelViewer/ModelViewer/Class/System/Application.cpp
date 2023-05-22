#include <Dxlib.h>
#include "Application.h"
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_impl_dx11.h"
#include "../../imGui/imgui_impl_win32.h"



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool window_open = false;

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		SendMessage(hWnd, WM_CLOSE, 0, 0);
		return 0;
		// タスクバーでアイコンがクリックされたときの処理
	case WM_ACTIVATEAPP:
	{
		// wParam が 0 の場合は非アクティブ状態（タスクバーでクリックされた）
		if (wParam == 0)
		{
			// ImGui ウィンドウを非表示にするなどの処理を行う
			if (window_open)
			{
				window_open = true;
			}
		}
		else
		{
			// ImGui ウィンドウを再表示するなどの処理を行う
			if (!window_open)
			{
				window_open = true;
			}
		}
		break;
	}

	};
	//return DefWindowProc(hwnd, msg, wp, lp);
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
		freamMng_->Update(true);

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
