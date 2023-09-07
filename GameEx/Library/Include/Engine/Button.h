#pragma once
//#include "Component.h"
//#include "Material.h"
class Button: public Component
{
public:
	Button();
	virtual ~Button();


	bool Picked(POINT screenPoint);
	void Create(Vec2 screenPos, Vec2 Size, shared_ptr<class Material> material);
	void AddOnClikcedEvent(std::function<void(void)> func);
	void InvokeOncliked();


private:
	std::function<void(void)> onClickedFunction;
	RECT mRect;



};

