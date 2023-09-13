#include "PostEffectMng.h"
#include "PostEffect_Noise.h"
#include "PostEffect_ScanLine.h"
#include "PostEffect_Transition_OuterCircumference.h"
#include "PostEffect_Transition_Circle.h"
#include "../../../imGui/imgui.h"
#include "../../../imGui/imgui_internal.h"
#include "../../Common/Utility.h"
#include <string>
#include <DxLib.h>

PostEffectMng::PostEffectMng()
{
	Init();
}

PostEffectMng::~PostEffectMng()
{
}

void PostEffectMng::Init()
{
	// �v�f�̒ǉ�
	postEffect_[L"none"] = nullptr;
	postEffect_[L"noise"] = std::make_unique<PostEffect_Noise>();
	postEffect_[L"scanLine"] = std::make_unique<PostEffect_ScanLine>();
	postEffect_[L"outerCircumference"] = std::make_unique<PostEffect_Transition_OuterCircumference>();
	postEffect_[L"circle"] = std::make_unique<PostEffect_Transition_Circle>();

	// �����v�f�̖��O
	selectName_ = L"none";
}

void PostEffectMng::Update()
{
	// �w��v�f�̌���
	auto itr = postEffect_.find(selectName_);
	
	if (itr != postEffect_.end())
	{
		// �w��v�f�������
		if (itr->second != nullptr)
		{
			// �w��v�f�̍X�V
			itr->second->Update();
		}
	}
}

void PostEffectMng::Draw(const int screen)
{
	//	�w��v�f��T��
	auto itr = postEffect_.find(selectName_);

	if (itr != postEffect_.end())
	{
		// �w��v�f�������
		if (itr->second != nullptr)
		{
			// �w��v�f�ŕ`��
			itr->second->Draw(selectName_, screen);
		}
		else
		{
			// �ʏ�`��
			DrawGraph(0,0,screen,true);
		}
		
	}
}

void PostEffectMng::Custom()
{
	//const char* listbox_items[] = { "none","noise" };
	// �v�f�ǉ����ɐ��l�𑝂₷
	const char* listbox_items[5];

	std::vector<std::string>name;

	// �v�f�̖��O������
	for (auto& map : postEffect_)
	{
		std::string postName = Utility::WStringToUTF8(map.first);
		name.push_back(postName);
	}

	auto cnt = 0;
	for (auto& map : name)
	{
		listbox_items[cnt] = map.c_str();
		cnt++;
	}

	// ���X�g�{�b�N�X�̔ԍ�
	static int listbox_item_current = 1;

	// ���X�g�{�b�N�X���쐬
	ImGui::ListBox("postEffect\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

	// �w��v�f�̖��O���擾
	std::string selectName = listbox_items[listbox_item_current];

	selectName_ = Utility::StringToWideString(selectName);

	// �v�f�̌���
	auto itr = postEffect_.find(selectName_);

	if (itr != postEffect_.end())
	{
		// �w��v�f�������
		if (itr->second != nullptr)
		{
			// �w��v�f
			itr->second->Custom();
		}
	}
}
