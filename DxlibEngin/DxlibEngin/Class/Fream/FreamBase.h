#pragma once
class FreamBase
{
public:
	FreamBase();
	~FreamBase();

	virtual void Init() = 0;
	virtual void Update() = 0;
private:

};

