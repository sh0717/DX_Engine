#pragma once
class CBehavior:public MonoBehavior
{
public:

	CBehavior();
	virtual ~CBehavior();

	virtual void Init() override;
	virtual void Update() override;

};

