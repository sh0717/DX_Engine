#pragma once
#include "MonoBehavior.h"
class ChBehavior :
    public MonoBehavior
{
	ChBehavior();
	virtual ~ChBehavior();

	virtual void Init() override;
	virtual void Update() override;


};

