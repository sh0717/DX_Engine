#pragma once
#include "BaseCollider.h"
class OBBCollider:public BaseCollider
{
public:
	OBBCollider();
	virtual ~OBBCollider();


	virtual void Update();



	virtual bool IsIntersects(Ray& ray, OUT float& distance);
	virtual bool IsIntersects(shared_ptr<BaseCollider>& other);

	
	BoundingOrientedBox& GetBoundingBox() { return mBoundingBox; }
private:

	float mRadius = 1.0f;
	BoundingOrientedBox  mBoundingBox;



};

