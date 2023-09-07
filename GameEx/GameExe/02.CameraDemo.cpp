#include "pch.h"
#include "02.CameraDemo.h"


void CameraDemo::Init()
{
	mShader = make_shared<Shader>(L"03.worldviewproj.fx");

	mGeo = make_shared<Geometry<Vertex::VertexColorData>>();
	GeometryHelper::CreateRectangle(mGeo, Color(0.f, 1.f, 1.f, 1.f));

	mVB = make_shared<VertexBuffer>(DEVICE);
	mVB->CreateBuffer(mGeo->GetVertices());

	mIB = make_shared<IndexBuffer>(DEVICE);
	mIB->CreateBuffer(mGeo->GetIndices());

	///������Ʈ ����� ��ǰ ���̱�
	//������Ʈ ���� ������ device�� ���ؽ�Ʈ �Ѵ� ����
	//1.Ʈ������ 2.ī�޶� 3. ī�޶� ��ũ��Ʈ
	mCamera = make_shared<DefaultObject>(DEVICE,CONTEXT);
	
	mCamera->GetOrAddTransform();
	mCamera->AddComponent(make_shared<Camera>());
	mCamera->AddComponent(make_shared<CameraScript>());

	mCamera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -2.f));


	mCamera->Init();
	mCamera->Start();


}

void CameraDemo::Update()
{
	mCamera->Update();

}

void CameraDemo::Render()
{
	Matrix  view = Camera::GetViewMatrix();
	Matrix proj = Camera::GetProjectionMatrix();


	mShader->GetMatrix("World")->SetMatrix((float*)&world);
	mShader->GetMatrix("View")->SetMatrix((float*)&view);
	mShader->GetMatrix("Projection")->SetMatrix((float*)&proj);

	uint32 stride = mVB->GetStride();
	uint32 offset = mVB->GetOffset();

	//�� ��ǲ ���̾ƿ����� �ڵ����� ���ֳ�..
	CONTEXT->IASetVertexBuffers(0, 1, mVB->GetComPtr().GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(mIB->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	mShader->DrawIndexed(0, 0, mIB->GetCount(), 0, 0);

}
