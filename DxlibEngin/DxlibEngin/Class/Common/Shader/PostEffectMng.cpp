#include "PostEffectMng.h"
#include "PostEffect_Noise.h"
#include "PostEffect_ScanLine.h"
#include "PostEffect_Transition_OuterCircumference.h"
#include "PostEffect_Transition_Circle.h"
#include "PostEffect_Glow.h"
#include "PostEffect_Led.h"
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
	// 要素の追加
	postEffect_[L"none"] = nullptr;
	postEffect_[L"noise"] = std::make_unique<PostEffect_Noise>();
	postEffect_[L"scanLine"] = std::make_unique<PostEffect_ScanLine>();
	postEffect_[L"outerCircumference"] = std::make_unique<PostEffect_Transition_OuterCircumference>();
	postEffect_[L"circle"] = std::make_unique<PostEffect_Transition_Circle>();
	postEffect_[L"glow"] = std::make_unique<PostEffect_Glow>();
	postEffect_[L"led"] = std::make_unique<PostEffect_Led>();

	// 初期要素の名前
	selectName_ = L"none";
}

void PostEffectMng::Update()
{
	// 指定要素の検索
	auto itr = postEffect_.find(selectName_);
	
	if (itr != postEffect_.end())
	{
		// 指定要素があれば
		if (itr->second != nullptr)
		{
			// 指定要素の更新
			itr->second->Update();
		}
	}
}

void PostEffectMng::Draw(const int screen)
{
	//	指定要素を探す
	auto itr = postEffect_.find(selectName_);

	if (itr != postEffect_.end())
	{
		// 指定要素があれば
		if (itr->second != nullptr)
		{
			// 指定要素で描画
			itr->second->Draw(selectName_, screen);
		}
		else
		{
			// 通常描画
			DrawGraph(0,0,screen,true);
		}
		
	}
}

void PostEffectMng::Custom()
{
	//const char* listbox_items[] = { "none","noise" };
	// 要素追加時に数値を増やす
	const char* listbox_items[7];

	std::vector<std::string>name;

	// 要素の名前を入れる
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

	// リストボックスの番号
	static int listbox_item_current = 1;

	// リストボックスを作成
	ImGui::ListBox("postEffect\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

	// 指定要素の名前を取得
	std::string selectName = listbox_items[listbox_item_current];

	selectName_ = Utility::StringToWideString(selectName);

	// 要素の検索
	auto itr = postEffect_.find(selectName_);

	if (itr != postEffect_.end())
	{
		// 指定要素があれば
		if (itr->second != nullptr)
		{
			// 指定要素
			itr->second->Custom();
		}
	}
}
