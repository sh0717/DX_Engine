#include "pch.h"
#include "Button.h"

Button::Button()
	:Component(eComponentType::Button)
{

}

Button::~Button()
{

}

bool Button::Picked(POINT screenPoint)
{
	return PtInRect(&mRect, screenPoint);
}

void Button::Create(Vec2 screenPos, Vec2 size, shared_ptr<Material> material)
{
	auto object = GetGameObject();

	float height = GRAPHICS->GetViewPort().Height;
	float width = GRAPHICS->GetViewPort().Width;
	 

	float x = screenPos.x - width / 2;
	float y = height / 2 - screenPos . y;
	Vec3 position = Vec3(x, y, 0.f);
	object->GetOrAddTransform()->SetPosition(position);
	object->GetOrAddTransform()->SetScale(Vec3(size.x, size.y, 1.f));

	object->SetLayerIndex(Layer_UI);

	if (object->GetMeshRenderer()==nullptr) {
		object->AddComponent(make_shared<MeshRenderer>());
	}

	object->GetMeshRenderer()->SetMaterial(material);

	auto mesh = RESOURCES->Get<Mesh>(L"Quad");
	object->GetMeshRenderer()->SetMesh(mesh);

	mRect.left = screenPos.x - size.x / 2;
	mRect.right = screenPos.x + size.x / 2;
	mRect.top = screenPos.y - size.y / 2;
	mRect.bottom = screenPos.y + size.y / 2;

}

void Button::AddOnClikcedEvent(std::function<void(void)> func)
{
	onClickedFunction = func;

}

void Button::InvokeOncliked()
{	
	if (onClickedFunction) {
		onClickedFunction();
	}
}
