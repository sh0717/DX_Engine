#pragma once
#include "BaseCollider.h"
class AABBCollider :
    public BaseCollider
{

public:
	AABBCollider();
	virtual ~AABBCollider();


	virtual void Update();



	virtual bool IsIntersects(Ray& ray, OUT float& distance);
	virtual bool IsIntersects(shared_ptr<BaseCollider>& other);

	
	BoundingBox& GetBoundingBox() { return mBoundingBox; }
private:

	float mRadius = 1.0f;
	BoundingBox  mBoundingBox;



};

