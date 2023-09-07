#pragma once
#include "BaseCollider.h"
class SphereCollider :
    public BaseCollider
{
public:
    SphereCollider();
    virtual ~SphereCollider();


    virtual void Update();



    virtual bool IsIntersects(Ray& ray, OUT float& distance);
    virtual bool IsIntersects(shared_ptr<BaseCollider>& other);

	void SetRadius(float radius) { mRadius = radius; }
	BoundingSphere& GetBoundingSphere() { return mBoundingSphere; }
 private:

        float mRadius = 1.0f;
        BoundingSphere  mBoundingSphere;




};

