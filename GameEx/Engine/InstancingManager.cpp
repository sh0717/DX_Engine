#include "pch.h"
#include "InstancingManager.h"

void InstancingManager::Render(vector<shared_ptr<DefaultObject>>& gameObjects)
{
	ClearData();

	//야 보니까 이거 순서를 맞춰야 겠는데?
	RenderMeshRenderer(gameObjects);
	RenderModelRenderer(gameObjects);
	RenderAnimRenderer(gameObjects);
}

void InstancingManager::ClearData()
{
	for (auto& pr : mBuffers)
	{
		shared_ptr<InstancingBuffer>& buffer = pr.second;
		buffer->ClearData();
	}
}




void InstancingManager::RenderAll(vector<shared_ptr<DefaultObject>>& gameObjects)
{

}

void InstancingManager::RenderMeshRenderer(vector<shared_ptr<DefaultObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<DefaultObject>>> cache;

	for (shared_ptr<DefaultObject>& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		const InstanceID instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const vector<shared_ptr<DefaultObject>>& vec = pair.second;


		{
			const InstanceID instanceId = pair.first;

			for (int32 i = 0; i < vec.size(); i++)
			{
				const shared_ptr<DefaultObject>& gameObject = vec[i];
				InstancingData data;
				data.world = gameObject->GetTransform()->GetWorldMatrix();

				AddData(instanceId, data);
			}

			shared_ptr<InstancingBuffer>& buffer = mBuffers[instanceId];
			vec[0]->GetMeshRenderer()->RenderInstancing(buffer);
		}
	}
}

void InstancingManager::RenderModelRenderer(vector<shared_ptr<DefaultObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<DefaultObject>>> cache;

	for (shared_ptr<DefaultObject>& gameObject : gameObjects)
	{
		if (gameObject->GetModelRenderer() == nullptr)
			continue;

		const InstanceID instanceId = gameObject->GetModelRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}


	for (auto& pair : cache)
	{
		const vector<shared_ptr<DefaultObject>>& vec = pair.second;

		int a = vec.size();
		int b = 10;

		{
			const InstanceID instanceId = pair.first;

			for (int32 i = 0; i < vec.size(); i++)
			{
				const shared_ptr<DefaultObject>& gameObject = vec[i];
				InstancingData data;
				data.world = gameObject->GetTransform()->GetWorldMatrix();

				AddData(instanceId, data);
			}

			shared_ptr<InstancingBuffer>& buffer = mBuffers[instanceId];
			vec[0]->GetModelRenderer()->RenderInstancing(buffer);
		}
	}



}

void InstancingManager::RenderAnimRenderer(vector<shared_ptr<DefaultObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<DefaultObject>>> cache;

	for (shared_ptr<DefaultObject>& gameObject : gameObjects)
	{
		if (gameObject->GetModelAnimator() == nullptr)
			continue;

		const InstanceID instanceId = gameObject->GetModelAnimator()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	for (auto& pair : cache)
	{

		shared_ptr<InstancedTweenDesc> tweenDesc = make_shared<InstancedTweenDesc>();
		const vector<shared_ptr<DefaultObject>>& vec = pair.second;

		int a = vec.size();
		int b = 10;
		{
			const InstanceID instanceId = pair.first;

			for (int32 i = 0; i < vec.size(); i++)
			{
				const shared_ptr<DefaultObject>& gameObject = vec[i];
				InstancingData data;
				data.world = gameObject->GetTransform()->GetWorldMatrix();

				AddData(instanceId, data);


				gameObject->GetModelAnimator()->UpdateTweenData();
				tweenDesc->tweens[i] = gameObject->GetModelAnimator()->GetTweenDesc();
			}

			//tweendesc 밀어넣기
			vec[0]->GetModelAnimator()->GetShader()->PushInstancedTweenBuffer(*tweenDesc.get());


			shared_ptr<InstancingBuffer>& buffer = mBuffers[instanceId];
			vec[0]->GetModelAnimator()->RenderInstancing(buffer);
		}
	}


}

void InstancingManager::AddData(InstanceID instanceId, InstancingData& data)
{
	//이 id 가 처음이라면?
	if (mBuffers.find(instanceId) == mBuffers.end()) {
		mBuffers[instanceId] = make_shared<InstancingBuffer>();
	}
	mBuffers[instanceId]->AddData(data);
}
