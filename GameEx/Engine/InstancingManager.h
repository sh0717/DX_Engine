#pragma once

#include "InstancingBuffer.h"


class DefaultObject;
class InstancingManager
{

	DECLARE_SINGLE(InstancingManager);


public:
	//오브젝트 집단을 넣으면 그 오브젝트들의 world 로 mbuffers 를 채워주고 instancing rendering 진행
	void Render(vector<shared_ptr<DefaultObject>>& gameObjects);
	void Clear() { mBuffers.clear(); }

	//이전에 mBuffers 에 채워놨던 instancingbuffer 의 데이터들을 다 지워준다.어짜피 다시채워 
	void ClearData();

private:
	void RenderAll(vector<shared_ptr<DefaultObject>>& gameObjects);

	void RenderMeshRenderer(vector<shared_ptr<DefaultObject>>& gameObjects);
	void RenderModelRenderer(vector<shared_ptr<DefaultObject>>& gameObjects);
	void RenderAnimRenderer(vector<shared_ptr<DefaultObject>>& gameObjects);

private:
	void AddData(InstanceID instanceId, InstancingData& data);

private:
	map<InstanceID/*instanceId*/, shared_ptr<InstancingBuffer>> mBuffers;

};

