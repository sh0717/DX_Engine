#include "pch.h"
#include "04.SamplerDemo.h"


void SamplerDemo::Init()
{
	mShader = make_shared<Shader>(L"04.Sampler.fx");

	mGeo = make_shared<Geometry<Vertex::VertexTextureData>>();
	
	shared_ptr<Geometry<Vertex::VertexFull>> mGeotmp = make_shared<Geometry<Vertex::VertexFull>>();
	//GeometryHelper::CreateCylinder(mGeotmp, 20.f,20.f,20.f, 20, 20);
	GeometryHelper::CreateGrid(mGeotmp, 100.f, 100.f, 256, 256,10,10);
	auto& vertex_tmp = mGeotmp->GetVertices();
	auto& index_tmp = mGeotmp->GetIndices();


	mGeo->SetIndices(index_tmp);
	std::vector<Vertex::VertexTextureData> vertices;
	vertices.reserve(vertex_tmp.size());
	for (auto& V : vertex_tmp) {

		Vertex::VertexTextureData v_tmp{ V.Position,V.TexC };
		vertices.push_back(v_tmp);
	}
	mGeo->SetVertices(vertices);


	//GeometryHelper::CreateRectangle(mGeo);


	mVB = make_shared<VertexBuffer>(DEVICE);
	mVB->CreateBuffer(mGeo->GetVertices());

	mIB = make_shared<IndexBuffer>(DEVICE);
	mIB->CreateBuffer(mGeo->GetIndices());

	///������Ʈ ����� ��ǰ ���̱�
	//������Ʈ ���� ������ device�� ���ؽ�Ʈ �Ѵ� ����
	//1.Ʈ������ 2.ī�޶� 3. ī�޶� ��ũ��Ʈ
	mCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

	mCamera->GetOrAddTransform();
	mCamera->AddComponent(make_shared<Camera>());
	mCamera->AddComponent(make_shared<CameraScript>());

	mCamera->GetTransform()->SetPosition(Vec3(0.f, 70.f, 0.f));



	mTexture=RESOURCES->Load<Texture>(L"Atrox", L"../Resources/Textures/Atrox_drx.jpg");
	

	mCamera->Init();
	mCamera->Start();

}

void SamplerDemo::Update()
{
	mCamera->Update();

}

void SamplerDemo::Render()
{
	Matrix  view = Camera::GetViewMatrix();
	Matrix proj = Camera::GetProjectionMatrix();


	mShader->GetMatrix("World")->SetMatrix((float*)&world);
	mShader->GetMatrix("View")->SetMatrix((float*)&view);
	mShader->GetMatrix("Projection")->SetMatrix((float*)&proj);
	mShader->GetSRV("Texture0")->SetResource(mTexture->GetSRVComPtr().Get());

	mShader->GetScalar("Address")->SetInt(0);


	uint32 stride = mVB->GetStride();
	uint32 offset = mVB->GetOffset();

	//�� ��ǲ ���̾ƿ����� �ڵ����� ���ֳ�..
	CONTEXT->IASetVertexBuffers(0, 1, mVB->GetComPtr().GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(mIB->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	mShader->DrawIndexed(0, 0, mIB->GetCount(), 0, 0);
}
