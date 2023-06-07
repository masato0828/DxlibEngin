#include <Dxlib.h>
#include "Application.h"
#include "../../imGui/imgui.h"
#include "../../imGui/imgui_impl_dx11.h"
#include "../../imGui/imgui_impl_win32.h"



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {

		SetUseHookWinProcReturnValue(TRUE);
		return true;
	}

	////IME�֘A��OS�ɔC����
	//switch (msg)
	//{
	//case WM_IME_SETCONTEXT:
	//case WM_IME_STARTCOMPOSITION:
	//case WM_IME_ENDCOMPOSITION:
	//case WM_IME_COMPOSITION:
	//case WM_IME_NOTIFY:
	//case WM_IME_REQUEST:
	//	SetUseHookWinProcReturnValue(TRUE);
	//	return DefWindowProc(hWnd, msg, wParam, lParam);

	//case WM_SYSCOMMAND:
	//	if ((wParam & 0xfff0) == SC_KEYMENU) { // Disable ALT application menu
	//		SetUseHookWinProcReturnValue(TRUE);
	//		return 0;
	//	}
	//	break;
	//}

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
	// �V�X�e���̏�����
	if (!SysInit())
	{
		return false;
	}

	

	// �e�N���X���e�ϐ�������
	if (!Init())
	{
		return false;
	}

	// ���C�����[�v
	while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// �X�V
		freamMng_->Update(true);

		freamMng_->Draw();

		ScreenFlip();

		// �`��
		freamMng_->Render();
	}

	freamMng_->ShutDown();

	// Dxlib�̏I��
	DxLib_End();

	return true;
}

bool Application::SysInit()
{
	// �`���ʂ̃T�C�Y�ɑ΂��郁�C���E�C���h�E�T�C�Y�̔䗦��ݒ肷��(�f�B�X�v���C�g�嗦)
	SetWindowSizeExtendRate(1.0);
	int DesktopW, DesktopH;
	// �N�����̃f�X�N�g�b�v�̉�ʏ����擾����
	GetDefaultState(&DesktopW, &DesktopH, NULL);
	// ��ʃ��[�h�̕ύX
	SetGraphMode(DesktopW, DesktopH, 32);
	// �E�B���h�E���[�h�ƑS��ʃ��[�h�̐؂�ւ�
	ChangeWindowMode(true);
	// �w�i�F
	SetBackgroundColor(60, 60, 60, 255);
	// Direct3D�̃o�[�W����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	// �E�B���h�E�v���V�[�W�����̐ݒ�
	SetHookWinProc(WndProc);
	// �E�B���h�E��\�����邩�ǂ���
	SetWindowVisibleFlag(false);
	// �A�v������A�N�e�B�u��Ԃł����������s���邩�ǂ�����ݒ肷��( TRUE:���s����  FALSE:��~����( �f�t�H���g ) )
	SetAlwaysRunFlag(true);

	//SetUseTSFFlag(false);

	SetUseIMEFlag(true);

	// Dxlib�̏�����
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
