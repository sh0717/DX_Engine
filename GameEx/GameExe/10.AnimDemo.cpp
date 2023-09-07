#include "pch.h"
#include "10.AnimDemo.h"

void AnimDemo::Init()
{


	shared_ptr<Texture> CubeMapTexture = make_shared<Texture>();
	CubeMapTexture->CreateCubeMap(L"../Resources/Textures/grasscube1024.dds");
	shared_ptr<Shader> cubeShader = make_shared<Shader>(L"08.CubeMap.fx");
	

	shared_ptr<Material> cubeMaterial = make_shared<Material>();
	cubeMaterial->SetShader(cubeShader);
	cubeMaterial->SetCubeTexture(CubeMapTexture);



	modelShader = make_shared<Shader>(L"16.AnimDemo.fx");

	



	///오브젝트 만들고 부품 붙이기
	//오브젝트 만들 때에는 device랑 컨텍스트 둘다 주자
	//1.트랜스폼 2.카메라 3. 카메라 스크립트
	mCamera = make_shared<DefaultObject>(DEVICE, CONTEXT);

	mCamera->AddComponent(make_shared<CameraTransform>());
	mCamera->AddComponent(make_shared<Camera>());
	mCamera->AddComponent(make_shared<CameraScript>());

	mCamera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -5.f));



	//object용 머티리얼 만들기 .(머티리얼데스크, 쉐이더, 텍스쳐)






	cubeObj = make_shared<DefaultObject>(DEVICE, CONTEXT);
	
	cubeObj->GetOrAddTransform()->SetScale(Vec3(30.f, 30.f, 30.f));
	cubeObj->AddComponent(make_shared<MeshRenderer>());
	{
		cubeObj->GetMeshRenderer()->SetMaterial(cubeMaterial);
	}

	{
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		cubeObj->GetMeshRenderer()->SetMesh(mesh);
	}

	

	mCamera->Init();
	mCamera->Start();

	CreateKachujin();

}

void AnimDemo::Update()
{
	mCamera->Update();
	cubeObj->GetOrAddTransform()->SetPosition(Camera::GetCameraPosition());
	

	
	static float f = 0.0f;
	static int counter = 0;


	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	{
		DirectionalLight lightDesc;
		lightDesc.Ambient = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
		lightDesc.Diffuse = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.Specular = Vec4(0.f, 0.f, 0.f, 8.f);
		lightDesc.Direction = Vec3(-1.f, -1.f, -1.f);
		modelShader->PushDirLightBuffer(lightDesc);
	}

	
	cubeObj->Update();


	
	_obj->Update();


}

void AnimDemo::Render()
{
	
}

void AnimDemo::CreateKachujin()
{
	shared_ptr<class Model> m1 = make_shared<Model>();
	m1->ReadModel(L"Kachujin/Kachujin");
	m1->ReadMaterial(L"Kachujin/Kachujin");
	m1->ReadAnimation(L"Kachujin/Idle");
	m1->ReadAnimation(L"Kachujin/Run");
	m1->ReadAnimation(L"Kachujin/Slash");

	_obj = make_shared<DefaultObject>(DEVICE,CONTEXT);
	_obj->GetOrAddTransform()->SetPosition(Vec3(0, 0, 0));
	_obj->GetOrAddTransform()->SetScale(Vec3(0.01f));

	_obj->AddComponent(make_shared<ModelAnimator>(modelShader));
	{
		_obj->GetModelAnimator()->SetModel(m1);
		//_obj->GetModelAnimator()->SetPass(1);
	}
}
