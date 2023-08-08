#include "Fream_Model.h"
#include "../../imGui/imgui.h"
#include "../Common/Utility.h"

Fream_Model::Fream_Model():freamData_(nullptr,L"")
{
	Init();
}

Fream_Model::~Fream_Model()
{
}

void Fream_Model::Init()
{
	// 選択している数
	nowSelect = nullptr;

	nowSelect = &freamData_;

	nowSelectFreamName_ = L"";

	freamNumber_ = -1;
}

void Fream_Model::Update()
{
	// 要素選択用ウィンドウ
	if (ImGui::Begin("Items", NULL, ImGuiWindowFlags_NoCollapse))
	{
		// ファイルの名前分回す
		for (auto& handleM : handleMap_)
		{
			Tree(handleM);
		}
	}
	ImGui::End();
}

void Fream_Model::SetModelHandle(std::map<std::wstring, int> handle)
{
	handleMap_ = handle;
	for (auto & handleM:handleMap_)
	{
		if (sclMap_.count(handleM.first))
		{
			continue;
		}
		sclMap_.emplace(handleM.first,Vector3(1,1,1));
		posMap_.emplace(handleM.first,Vector3(0,0,0));
		rotMap_.emplace(handleM.first,Vector3(0,0,0));

		freamData_.freamMap_.try_emplace(handleM.first, 
			FreamData(&freamData_, handleM.first));

		//freamData_.freamMap_ = handleM.first;
	}

	
	

}

void Fream_Model::Draw()
{
	if (handleMap_.empty())
	{
		return;
	}

	if (nowSelect->myName == L"")
	{
		return;
	}
	//auto selectHandle = GetElementNum(handleMap_, selected);

    ////MV1SetScale(modelHandle.second, scl.toVECTOR());
    

    for (auto& modelHandle : handleMap_)
    {
        MV1_REF_POLYGONLIST RefPoly;

        // モデルの全フレームのポリゴンの情報を取得
        RefPoly = MV1GetReferenceMesh(modelHandle.second, -1, TRUE);

        auto minRP = RefPoly.MinPosition;
        auto maxRP = RefPoly.MaxPosition;

        VECTOR a1 = { minRP.x,minRP.y,minRP.z };
        VECTOR a2 = { maxRP.x,minRP.y,minRP.z };
        VECTOR a3 = { minRP.x,maxRP.y,minRP.z };
        VECTOR a4 = { maxRP.x,maxRP.y,minRP.z };
        VECTOR a5 = { minRP.x,minRP.y,maxRP.z };
        VECTOR a6 = { maxRP.x,minRP.y,maxRP.z };
        VECTOR a7 = { minRP.x,maxRP.y,maxRP.z };
        VECTOR a8 = { maxRP.x,maxRP.y,maxRP.z };

        int lineCol = 0x000000;
        DrawLine3D(a1, a3, lineCol);
        DrawLine3D(a1, a2, lineCol);
        DrawLine3D(a1, a5, lineCol);
        DrawLine3D(a2, a4, lineCol);
        DrawLine3D(a2, a6, lineCol);
        DrawLine3D(a3, a7, lineCol);
        DrawLine3D(a3, a4, lineCol);
        DrawLine3D(a4, a8, lineCol);
        DrawLine3D(a5, a6, lineCol);
        DrawLine3D(a6, a8, lineCol);
        DrawLine3D(a7, a8, lineCol);
        DrawLine3D(a7, a5, lineCol);

        //DrawSphere3D(minRP, 10, 10, lineCol, lineCol, true);
        //DrawSphere3D(maxRP, 10, 10, lineCol, lineCol, true);
		MV1SetScale(modelHandle.second, sclMap_.at(modelHandle.first).toVECTOR());
		MV1SetPosition(modelHandle.second, posMap_.at(modelHandle.first).toVECTOR());
		MV1SetRotationXYZ(modelHandle.second, rotMap_.at(modelHandle.first).toVECTOR());
        
        MV1DrawModel(modelHandle.second);
        MV1RefreshReferenceMesh(modelHandle.second, -1, TRUE);
    }
}

void Fream_Model::CustomStatus()
{
	if (handleMap_.empty())
	{
		return;
	}

	if (nowSelect->myName == L"")
	{
		return;
	}

	if (freamNumber_ == -1)
	{

		ImGui::SetCursorPos(ImVec2(30, 50));
		ImGui::Text("scl");

		ImGui::SetCursorPos(ImVec2(30, 90));
		ImGui::Text("pos");

		ImGui::SetCursorPos(ImVec2(30, 130));
		ImGui::Text("rot");

		ImGui::SetCursorPos(ImVec2(70, 40));
		ImGui::DragFloat3("##scl", &sclMap_.at(nowSelect->myName));

		ImGui::SetCursorPos(ImVec2(70, 80));
		ImGui::DragFloat3("##pos", &posMap_.at(nowSelect->myName));

		ImGui::SetCursorPos(ImVec2(70, 120));
		ImGui::DragFloat3("##rot", &rotMap_.at(nowSelect->myName));
	}
	else
	{
		ImGui::SetCursorPos(ImVec2(30, 50));
		ImGui::Text("scl");

		ImGui::SetCursorPos(ImVec2(30, 90));
		ImGui::Text("pos");

		ImGui::SetCursorPos(ImVec2(30, 130));
		ImGui::Text("rot");

		ImGui::SetCursorPos(ImVec2(70, 40));
		ImGui::DragFloat3("##scl", &freamSclVec_.at(freamNumber_));

		ImGui::SetCursorPos(ImVec2(70, 80));
		ImGui::DragFloat3("##pos", &freamPosVec_.at(freamNumber_));

		ImGui::SetCursorPos(ImVec2(70, 120));
		ImGui::DragFloat3("##rot", &freamRotVec_.at(freamNumber_));

		Fream();
	}

	
}

void Fream_Model::Fream()
{
	auto selectHandle = handleMap_.at(nowSelect->parentFream_->myName);
	MATRIX mat = MV1GetFrameLocalMatrix(selectHandle,freamNumber_);

	// フレームの行列から拡大率行列の取得
	auto scl = MGetSize(mat);

	// フレームの行列から回転行列の取得
	auto rot = MGetRotElem(mat);

	// フレームの行列から平行移動行列の取得
	auto pos = MGetTranslateElem(mat);

	// 拡大率用単位行列の取得
	MATRIX mixMat = MGetIdent();

	//////////拡大率///////////////
	{
		auto scale = freamSclVec_.at(freamNumber_);

		// 拡大率行列の乗算
		mixMat = MMult(mixMat, MGetScale(scale.toVECTOR()));
	}

	////////// 回転///////////////
	{
		// 回転用単位行列の取得
		MATRIX mixRot = MGetIdent();
		//// 回転行列三要素の乗算　
		mixRot = MMult(mixRot, MGetRotX(freamRotVec_.at(freamNumber_).x_));
		mixRot = MMult(mixRot, MGetRotY(freamRotVec_.at(freamNumber_).y_));
		mixRot = MMult(mixRot, MGetRotZ(freamRotVec_.at(freamNumber_).z_));
		// 回転行列との乗算(拡大率、回転)
		mixMat = MMult(mixMat, mixRot);
	}
	
	//////////位置///////////////
	{
		auto position = freamPosVec_.at(freamNumber_);
		// 平行移動行列との乗算(拡大率、回転、平行移動)
		mixMat = MMult(mixMat, MGetTranslate(position.toVECTOR()));
	}

	// フレームの行列とする
	MV1SetFrameUserLocalMatrix(selectHandle, freamNumber_, mixMat);
}

void Fream_Model::Tree(std::pair<const std::wstring, int>& handleData)
{
	ImGuiTreeNodeFlags node_flags = 0;

	auto data = freamData_.freamMap_.try_emplace(handleData.first, FreamData(&freamData_, handleData.first)).first;

	bool is_selected = &data->second.myName == &handleData.first;

	if (is_selected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	auto freamNum = MV1GetFrameNum(handleData.second);

	if (freamNum <= 0)
	{
		node_flags = node_flags |= ImGuiTreeNodeFlags_Leaf;
	}
	else
	{
		node_flags & ~ImGuiTreeNodeFlags_Leaf;
	}

	auto is_open = ImGui::TreeNodeEx((void*)&data->second, node_flags,Utility::WideStringToString(handleData.first).c_str());
	
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		// 現在選択中
		nowSelect = &data->second;
		nowSelectFreamName_ = handleData.first;
		freamNumber_ = -1;
	}

	if (is_open)
	{
		nowSelect->parentFream_->myName = handleData.first;

		ImGuiTreeNodeFlags node_fream_flags = 0;

		int freamNum = MV1GetFrameNum(handleData.second);
		for (auto itr = 0; itr < freamNum; itr++)
		{
			auto freamName = MV1GetFrameName(handleData.second, itr);
			std::filesystem::path fileNameP = freamName;

			auto data = freamData_.freamMap_.try_emplace(fileNameP,
				FreamData(&freamData_, fileNameP)).first;

			bool is_fream_selected = freamNumber_ == itr;

			if (is_fream_selected)
				node_fream_flags |= ImGuiTreeNodeFlags_Selected;

			auto childNum = MV1GetFrameChildNum(handleData.second, itr);

			if (childNum <= 0)
			{
				node_fream_flags = node_fream_flags |= ImGuiTreeNodeFlags_Leaf;
			}
			else
			{
				node_fream_flags & ~ImGuiTreeNodeFlags_Leaf;
			}

			auto is = ImGui::TreeNodeEx((void*)&data->second, node_fream_flags, fileNameP.u8string().c_str());
			
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				nowSelect = &data->second;
				nowSelectFreamName_ = fileNameP;
				freamNumber_ = itr;
			}

			// ローカル行列を取得
		MATRIX matrix = MV1GetFrameLocalMatrix(handleData.second, itr);

		// 位置を取得
		Vector3 position;
		position.x_ = matrix.m[3][0];
		position.y_ = matrix.m[3][1];
		position.z_ = matrix.m[3][2];

		// 回転を取得
		Vector3 rotation;
		rotation.x_ = atan2f(matrix.m[2][1], matrix.m[2][2]);
		rotation.y_ = atan2f(-matrix.m[2][0], sqrtf(matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]));
		rotation.z_ = atan2f(matrix.m[1][0], matrix.m[0][0]);

		// 拡大率を取得
		Vector3 scale;
		scale.x_ = sqrtf(matrix.m[0][0] * matrix.m[0][0] + matrix.m[0][1] * matrix.m[0][1] + matrix.m[0][2] * matrix.m[0][2]);
		scale.y_ = sqrtf(matrix.m[1][0] * matrix.m[1][0] + matrix.m[1][1] * matrix.m[1][1] + matrix.m[1][2] * matrix.m[1][2]);
		scale.z_ = sqrtf(matrix.m[2][0] * matrix.m[2][0] + matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]);
	
		freamSclVec_.push_back(scale);
		freamPosVec_.push_back(position);
		freamRotVec_.push_back(rotation);

			if (is)
			{
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}

}


//// ノードを開いているかどうか
	//		// ツリーデータの設定
	//ImGuiTreeNodeFlags node_flags = 0;

	//auto data = freamData_.freamMap_.try_emplace(handleData.first,
	//	FreamData(&freamData_, handleData.first)).first;

	//const bool is_selected = &data->second == nowSelect;

	//// 選択中
	//if (is_selected)
	//	node_flags |= ImGuiTreeNodeFlags_Selected;

	//auto freamNum = MV1GetFrameNum(handleData.second);

	////// フレームがない場合
	//if (freamNum <= 0)
	//{
	//	node_flags = node_flags |= ImGuiTreeNodeFlags_Leaf;
	//}
	//else
	//{
	//	node_flags & ~ ImGuiTreeNodeFlags_Leaf;
	//}

	//if (Utility::IsMatch(Utility::WideStringToString(handleData.first), nowSelectFreamName_))
	//{
	//	// ツリーを開く
	//	ImGui::SetNextItemOpen(true);
	//}
	//// ノードを開いているかどうか
	////auto is_open = ImGui::TreeNodeEx((void*)&dataItr->second, node_flags, directoryName.c_str());
	//auto is_open = ImGui::TreeNodeEx((void*)&data->second, node_flags,
	//	Utility::WideStringToString(handleData.first).c_str());

	//// ツリーを選択
	//if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	//{
	//	// 現在選択中
	//	nowSelect = &data->second;
	//	nowSelectFreamName_ = handleData.first;
	//	freamNumber_ = -1;
	//}

	//// ノードが開かれている
	//if (is_open)
	//{
	//	auto freamNum = MV1GetFrameNum(handleData.second);
	//	for (auto itr = 0; itr < freamNum; itr++)
	//	{
	//		auto freamName = MV1GetFrameName(handleData.second, itr);
	//		std::filesystem::path fileNameP = freamName;

	//		auto childNum = MV1GetFrameChildNum(handleData.second, itr);
	//		
	//		auto data = freamData_.freamMap_.try_emplace(fileNameP,
	//			FreamData(&freamData_, fileNameP)).first;

	//		const bool is_selected = &data->second == nowSelect;

	//		// 選択中
	//		if (is_selected)
	//			node_flags |= ImGuiTreeNodeFlags_Selected;
	//		
	//		//// フレームがない場合
	//		if (childNum <= 0)
	//		{
	//			node_flags = node_flags |= ImGuiTreeNodeFlags_Leaf;
	//		}
	//		else
	//		{
	//			node_flags & ~ImGuiTreeNodeFlags_Leaf;
	//		}
	//		//std::pair<const std::wstring, int> pair(fileNameP, childNum);
	//		//Tree(pair);
	//		auto is = ImGui::TreeNodeEx((void*)&data->second, node_flags,fileNameP.u8string().c_str());

	//			// ローカル行列を取得
	//		MATRIX matrix = MV1GetFrameLocalMatrix(handleData.second, itr);

	//		// 位置を取得
	//		Vector3 position;
	//		position.x_ = matrix.m[3][0];
	//		position.y_ = matrix.m[3][1];
	//		position.z_ = matrix.m[3][2];

	//		// 回転を取得
	//		Vector3 rotation;
	//		rotation.x_ = atan2f(matrix.m[2][1], matrix.m[2][2]);
	//		rotation.y_ = atan2f(-matrix.m[2][0], sqrtf(matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]));
	//		rotation.z_ = atan2f(matrix.m[1][0], matrix.m[0][0]);

	//		// 拡大率を取得
	//		Vector3 scale;
	//		scale.x_ = sqrtf(matrix.m[0][0] * matrix.m[0][0] + matrix.m[0][1] * matrix.m[0][1] + matrix.m[0][2] * matrix.m[0][2]);
	//		scale.y_ = sqrtf(matrix.m[1][0] * matrix.m[1][0] + matrix.m[1][1] * matrix.m[1][1] + matrix.m[1][2] * matrix.m[1][2]);
	//		scale.z_ = sqrtf(matrix.m[2][0] * matrix.m[2][0] + matrix.m[2][1] * matrix.m[2][1] + matrix.m[2][2] * matrix.m[2][2]);
	//	
	//		// ツリーを選択
	//		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	//		{
	//			// 現在選択中
	//			//nowSelect = &data->second;
	//			nowSelectFreamName_ = fileNameP;
	//			freamNumber_ = itr;
	//		}

	//		if (is)
	//		{
	//			ImGui::TreePop();
	//		}
	//	}

	//	// 再起処理
	//	//Tree2(itr->path(), dataItr->second);
	//	//Tree(itr->path(), data->second, nowSelect);
	//	// ツリーの終了
	//	ImGui::TreePop();
	//}

	//