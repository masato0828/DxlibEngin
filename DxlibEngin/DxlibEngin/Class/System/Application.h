#pragma once
#include <memory>
#include "../Fream/FreamMng.h"

#define lpApplication Application::GetInstance()

class Application
{
public:
	// �C���X�^���X�擾
	static Application& GetInstance()
	{
		static Application s_Instance;
		return s_Instance;
	}

	// �X�V
	bool Run();
private:
	Application();
	~Application();
	// �V�X�e��������
	bool SysInit();

	bool Init();

	std::unique_ptr<FreamMng> freamMng_;
};

