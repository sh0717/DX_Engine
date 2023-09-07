#include "pch.h"
#include "06.HeightMapDemo.h"


void HeightMapDemo::Init()
{
	mShader = make_shared<Shader>(L"05.heightmap.fx");

	mGeo = make_shared<Geometry<Vertex::VertexTextureData>>();

	shared_ptr<Geometry<Vertex::VertexFull>> mGeotmp = make_shared<Geometry<Vertex::VertexFull>>();
	//GeometryHelper::CreateCylinder(mGeotmp, 20.f,20.f,20.f, 20, 20);
	GeometryHelper::CreateGrid(mGeotmp, 100.f, 100.f, 256, 256, 1, 1);
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

	///오브젝트 만들고 부품 붙이기
	//오브젝트 만들 때에는 device랑 컨텍스트 둘다 주자
	//1.트랜스폼 2.카메라 3. 카메라 스크립트
	mCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

	mCamera->AddComponent(make_shared<CameraTransform>());
	mCamera->AddComponent(make_shared<Camera>());
	mCamera->AddComponent(make_shared<CameraScript>());

	mCamera->GetTransform()->SetPosition(Vec3(0.f, 7.f, 0.f));



	grassTexture = RESOURCES->Load<Texture>(L"grass", L"../Resources/Textures/Terrain/grass.jpg");
	HeightMap = RESOURCES->Load<Texture>(L"height", L"../Resources/Textures/Terrain/height.png");


	Matrix a = Matrix::Identity;

	Matrix b = MathHelper::InverseTranspose(a);

	mCamera->Init();
	mCamera->Start();

}

void HeightMapDemo::Update()
{
	mCamera->Update();

}

void HeightMapDemo::Render()
{
	Matrix  view = Camera::GetViewMatrix();
	Matrix proj = Camera::GetProjectionMatrix();


	//view = Matrix::Identity;
	//proj = Matrix::Identity;

	mShader->GetMatrix("World")->SetMatrix((float*)&world);
	mShader->GetMatrix("View")->SetMatrix((float*)&view);
	mShader->GetMatrix("Projection")->SetMatrix((float*)&proj);
	
	mShader->GetSRV("Texture0")->SetResource(grassTexture->GetSRVComPtr().Get());
	mShader->GetSRV("HeightMap")->SetResource(HeightMap->GetSRVComPtr().Get());


	uint32 stride = mVB->GetStride();
	uint32 offset = mVB->GetOffset();

	//와 인풋 레이아웃까지 자동으로 해주넴..
	CONTEXT->IASetVertexBuffers(0, 1, mVB->GetComPtr().GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(mIB->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	mShader->DrawIndexed(0, 0, mIB->GetCount(), 0, 0);
}

