#include "pch.h"
#include "ParticleSystem.h"





ParticleSystem::ParticleSystem()
	:Component(eComponentType::ParticleSystem),bFirstRun(true),mGameTime(300.f),mTimeStep(0.f),mAge(0.f)
{
	mEmitPosW = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mEmitDirW = XMFLOAT3(0.0f, 1.0f, 0.0f);
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Init()
{	
	mMaxParticles = 50;
	BuildVB();
	mRandomTextureSRV = Utils::CreateRandomTexture1DSRV(DEVICE);

}

void ParticleSystem::Update()
{
	mGameTime  +=(DT/1000);
	mTimeStep = (DT/1000);

	mAge += (DT/1000);
}

void ParticleSystem::Render()
{
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	
	shared_ptr<Shader> myShader = mMaterial->GetShader();
	mMaterial->Update();

	uint32 stride = sizeof(Vertex::Particle);
	uint32 offset = 0;
	myShader->PushGlobalBuffer(Camera::GetViewMatrix(), Camera::GetProjectionMatrix(), Camera::GetCameraPosition());

	myShader->GetSRV("RandomTex")->SetResource(mRandomTextureSRV.Get());

	ParticleSystemsDesc desc;
	desc.EmitDirW = mEmitDirW;
	desc.EmitPosW = mEmitPosW;
desc.mAge = mAge;
	desc.mTimeStep = mTimeStep;
	desc.gameTime = mGameTime;
	desc.padding = MathHelper::RandF(-1.f,1.f);

	myShader->PushParticleSystemBuffer(desc);



	if (bFirstRun) {
		CONTEXT->IASetVertexBuffers(0, 1, mInitVB.GetAddressOf(), &stride, &offset);
	}
	else {
		CONTEXT->IASetVertexBuffers(0, 1, mDrawVB.GetAddressOf(), &stride, &offset);
	}

	CONTEXT->SOSetTargets(1, mStreamOutVB.GetAddressOf(), &offset);

	//the tech 0 is streamout tech 
	if (bFirstRun) {
		myShader->Draw(0, 0, 1, 0);
		bFirstRun = false;
	}
	else {
		myShader->DrawAuto(0, 0);
	}



	// done streaming-out--unbind the vertex buffer
	ID3D11Buffer* bufferArray[1] = { 0 };
	CONTEXT->SOSetTargets(1, bufferArray, &offset);

	std::swap(mDrawVB, mStreamOutVB);

	CONTEXT->IASetVertexBuffers(0, 1, mDrawVB.GetAddressOf(), &stride, &offset);
	myShader->DrawAuto(1,0);

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ParticleSystem::Reset()
{
	bFirstRun = true;
	mAge = 0.f;
}

void ParticleSystem::BuildVB()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = uint32(sizeof(Vertex::Particle)*1);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	Vertex::Particle p;
	//ZeroMemory(&p, sizeof(Vertex::Particle));
	p.InitialPos = Vec4(0.f, 0.f, 5.f, 1.f);
	p.Size = Vec2(3.f, 3.f);
	
	p.Age = 0.0f;
	p.Type = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &p;


	DEVICE->CreateBuffer(&vbd, &vinitData, mInitVB.GetAddressOf());

	vbd.ByteWidth = sizeof(Vertex::Particle) * mMaxParticles;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	(DEVICE->CreateBuffer(&vbd, 0, mDrawVB.GetAddressOf()));
	(DEVICE->CreateBuffer(&vbd, 0, mStreamOutVB.GetAddressOf()));
}
