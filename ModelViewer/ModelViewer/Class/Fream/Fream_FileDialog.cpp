#include "Fream_FileDialog.h"
#include "../../imGui/ImGuiFileDialog.h"
#include "../../imGui/CustomFont.h"
#include "../Common/FileDialog.h"
#include <filesystem>
#include <iostream>


Fream_FileDialog::Fream_FileDialog()
{
	Init();
}

Fream_FileDialog::~Fream_FileDialog()
{
}

void Fream_FileDialog::Init()
{
	fileDialogFlg_ = false;
}

void Fream_FileDialog::Update()
{
	if (ImGui::Begin("fileDialog"))
	{
		//現在のカンレントディレクトリを取得
		auto directory = std::filesystem::current_path();
		// プログラム起動時にファイルの出力
		// その位置からツリーを開く

		if (ImGui::TreeNode("file"))
		{
			int eraseNum = directory.string().find_last_of('\\');
			//// 
			std::string file = directory.string().erase(0, eraseNum);
			//// 
			std::string fileN = file.erase(0, 1);

			const char* test = fileN.c_str();;

			std::string fi = test;

			if (ImGui::TreeNode(fi.c_str()))
			{
				//auto fii = ".\\" + fi;
				std::filesystem::directory_iterator e = std::filesystem::directory_iterator(directory.string());
				for (auto f : e) {

					int eraseNums = f.path().string().find_last_of('\\');
					//// 
					std::string files = f.path().string().erase(0, eraseNums);
					//// 
					std::string fileNs = files.erase(0, 1);

					const char* tests = fileNs.c_str();;

					std::string fis = tests;

					//std::cout << f.path().string() << std::endl;
					ImGui::TreeNode(fis.c_str());
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			//for (/*ファイルの数分回す*/)
			//{
				//ファイルの名前の取得
				//ImGui::TreeNode("取得したファイル名");
				// 
				//ImGui::TreePop();
			//}
			ImGui::TreePop();
		}



		ImGui::End();
	}


	if (ImGui::Begin("FileDialog"))
	{
		if (ImGui::Button("open"))
		{
			fileDialogFlg_ = true;
		}
		ImGui::End();
	}

	if (fileDialogFlg_)
	{
		OpenFileDialog()();
		fileDialogFlg_ = false;
	}
}
