#include "pch.h"
#include "07.NormalTestDemo.h"


void NormalTestDemo::Init()
{
	mShader = make_shared<Shader>(L"06.normalsample.fx");

	world = XMMatrixScaling(2, 2, 2);
	

	///������Ʈ ����� ��ǰ ���̱�
	//������Ʈ ���� ������ device�� ���ؽ�Ʈ �Ѵ� ����
	//1.Ʈ������ 2.ī�޶� 3. ī�޶� ��ũ��Ʈ
	mCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

	mCamera->AddComponent(make_shared<CameraTransform>());
	mCamera->AddComponent(make_shared<Camera>());
	mCamera->AddComponent(make_shared<CameraScript>());

	mCamera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -5.f));


	mTexture = RESOURCES->Load<Texture>(L"Atrox", L"../Resources/Textures/Atrox_drx.jpg");
	mMesh = RESOURCES->Get<Mesh>(L"Sphere");



	mCamera->Init();
	mCamera->Start();

}

void NormalTestDemo::Update()
{
	mCamera->Update();

}

void NormalTestDemo::Render()
{
	Matrix  view = Camera::GetViewMatrix();
	Matrix proj = Camera::GetProjectionMatrix();

	shared_ptr<VertexBuffer> mVB = mMesh->GetVB();
	shared_ptr<IndexBuffer> mIB = mMesh->GetIB();


	//view = Matrix::Identity;
	//proj = Matrix::Identity;

	mShader->GetMatrix("World")->SetMatrix((float*)&world);
	mShader->GetMatrix("View")->SetMatrix((float*)&view);
	mShader->GetMatrix("Projection")->SetMatrix((float*)&proj);
	

	mShader->GetSRV("Texture0")->SetResource(mTexture->GetSRVComPtr().Get());
	mShader->GetVector("LightDir")->SetFloatVector((float*)&_lightDir);


	uint32 stride = mVB->GetStride();
	uint32 offset = mVB->GetOffset();

	//�� ��ǲ ���̾ƿ����� �ڵ����� ���ֳ�..
	CONTEXT->IASetVertexBuffers(0, 1, mVB->GetComPtr().GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(mIB->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	mShader->DrawIndexed(0, 0, mIB->GetCount(), 0, 0);
}

