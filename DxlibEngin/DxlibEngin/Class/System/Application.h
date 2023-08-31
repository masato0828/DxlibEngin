#pragma once
#include <memory>
#include "../Fream/FreamMng.h"

#define lpApplication Application::GetInstance()

class Application
{
public:
	// インスタンス取得
	static Application& GetInstance()
	{
		static Application s_Instance;
		return s_Instance;
	}

	// 更新
	bool Run();
private:
	Application();
	~Application();
	// システム初期化
	bool SysInit();

	bool Init();

	std::unique_ptr<FreamMng> freamMng_;
};

