#pragma once

#include "InstancingBuffer.h"


class DefaultObject;
class InstancingManager
{

	DECLARE_SINGLE(InstancingManager);


public:
	//������Ʈ ������ ������ �� ������Ʈ���� world �� mbuffers �� ä���ְ� instancing rendering ����
	void Render(vector<shared_ptr<DefaultObject>>& gameObjects);
	void Clear() { mBuffers.clear(); }

	//������ mBuffers �� ä������ instancingbuffer �� �����͵��� �� �����ش�.��¥�� �ٽ�ä�� 
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

