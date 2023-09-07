#include "pch.h"
#include "05.ColorBoxDemo.h"


void ColorBoxDemo::Init()
{
	mShader = make_shared<Shader>(L"03.worldviewproj.fx");

	mGeo = make_shared<Geometry<Vertex::VertexTextureData>>();
	

	std::vector<Vertex::VertexColorData> vertices =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), Color(1.0,0.0,0.0,1.0)},
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), Color(1.0,0.0,0.0,1.0)   },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), Color(1.0,1.0,0.0,1.0)     },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), Color(1.0,0.0,1.0,1.0)   },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), Color(1.0,0.0,0.5,1.0)    },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), Color(1.0,0.0,0.0,1.0)  },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), Color(1.0,0.7,0.0,1.0)   },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), Color(1.0,0.0,0.0,1.0)}
	};

	std::vector<UINT> indices = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};



	//GeometryHelper::CreateRectangle(mGeo);


	mVB = make_shared<VertexBuffer>(DEVICE);
	mVB->CreateBuffer(vertices);

	mIB = make_shared<IndexBuffer>(DEVICE);
	mIB->CreateBuffer(indices);



	///오브젝트 만들고 부품 붙이기
	//오브젝트 만들 때에는 device랑 컨텍스트 둘다 주자
	//1.트랜스폼 2.카메라 3. 카메라 스크립트
	mCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

	mCamera->GetOrAddTransform();
	mCamera->AddComponent(make_shared<Camera>());
	//mCamera->AddComponent(make_shared<CameraScript>());

	//mCamera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -2.f));




	

	mCamera->Init();
	mCamera->Start();

}

void ColorBoxDemo::Update()
{
	mCamera->Update();

}

void ColorBoxDemo::Render()
{
	Matrix  view = Camera::GetViewMatrix();
	Matrix proj = Camera::GetProjectionMatrix();


	//view = Matrix::Identity;
	//proj = Matrix::Identity;

	mShader->GetMatrix("World")->SetMatrix((float*)&world);
	mShader->GetMatrix("View")->SetMatrix((float*)&view);
	mShader->GetMatrix("Projection")->SetMatrix((float*)&proj);
	


	uint32 stride = mVB->GetStride();
	uint32 offset = mVB->GetOffset();

	//와 인풋 레이아웃까지 자동으로 해주넴..
	CONTEXT->IASetVertexBuffers(0, 1, mVB->GetComPtr().GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(mIB->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	mShader->DrawIndexed(0, 0, mIB->GetCount(), 0, 0);
}

