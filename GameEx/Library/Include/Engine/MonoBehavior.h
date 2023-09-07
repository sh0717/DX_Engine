#pragma once

class MonoBehavior : public Component
{

public:
	MonoBehavior();
	~MonoBehavior();

	virtual void Init() override;
	virtual void Update() override;
};

