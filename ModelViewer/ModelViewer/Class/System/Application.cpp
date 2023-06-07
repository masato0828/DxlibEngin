#include <Dxlib.h>
#include "Application.h"
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_impl_dx11.h"
#include "../../imGui/imgui_impl_win32.h"



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI DefaultWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {

		SetUseHookWinProcReturnValue(TRUE);
		return true;
	}
	return 0;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	

	switch (msg)
	{
		// IME関連のメッセージを処理する
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	{
		// IMEの文字列を取得する
		std::wstring imeText = L"";
		DWORD dwSize = 0;
		if (ImmGetCompositionStringW(ImmGetContext(hWnd), GCS_RESULTSTR, NULL, 0) > 0)
		{
			dwSize = ImmGetCompositionStringW(ImmGetContext(hWnd), GCS_RESULTSTR, NULL, 0);
			std::vector<wchar_t> buffer(dwSize);
			ImmGetCompositionStringW(ImmGetContext(hWnd), GCS_RESULTSTR, &buffer[0], dwSize);
			imeText = std::wstring(&buffer[0]);
		}

		// ImGuiにIMEの入力文字列を追加する
		for (wchar_t c : imeText)
		{
			ImGui::GetIO().AddInputCharacter(c);
		}

		break;
	}
	default:
		break;
	}

	// DxLibのデフォルトのウィンドウプロシージャを呼び出す
	return DefaultWndProc(hWnd, msg, wParam, lParam);
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

	//SetUseTSFFlag(false);

	//SetUseIMEFlag(true);

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
