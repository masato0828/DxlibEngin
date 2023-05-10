#pragma once
#include <memory>
#include "../Common/Vector2.h"
#include "Fream_DokingArea.h"
#include "Fream_Stage.h"
#include "Fream_SceneView.h"
#include "Fream_Camera.h"
#include "MousePointCreate.h"
#include "../NewFolder1/Device_Mouse.h"

class FreamMng
{
public:
	FreamMng();
	~FreamMng();

	// �X�V����
	void Update();
	// �`�揈��
	void Draw();
	// ImGui�`��I������
	void Render();
	// ImGui�I������
	void ShutDown();
	// �}�E�X�̈ʒu�̕\��
	void DrawMousePoint();
private:
	// ������
	void Init();
	// ImGui�̃_�C���N�gX�ƃE�B���h�E�̍X�V
	void SysNewFream();
	// �E�B���h�E�̃X�^�C��
	void Style();

	void Inspector(); 

	void OptionWindow();
    
	void CreateMenuBer();

	std::unique_ptr<Fream_SceneView> sceneView_;
	std::unique_ptr<Fream_DokingArea> dokingArea_;
	std::unique_ptr<Fream_Stage> stage_;
	std::unique_ptr<Fream_Camera> camera_;
	std::unique_ptr<Device_Mouse> mouse_;

	bool optionWindowFlg_;

	MousePoint mousePoint_;
};

