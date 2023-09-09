#pragma once

class MonoBehavior : public Component
{

public:
	MonoBehavior();
	virtual ~MonoBehavior();

	virtual void Init() override;
	virtual void Update() override;
};




