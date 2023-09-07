#pragma once
class ParticleSystem:public Component
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

	virtual void Init() override;

	virtual void Update() override;
	void Render();
	void Reset();





	inline void SetMaterial(shared_ptr<Material> mat) { mMaterial = mat; }
private:
	void BuildVB();


private:
	bool bFirstRun ;
	uint32 mMaxParticles = 0;

	float mGameTime;
	float mTimeStep;
	float mAge;


	shared_ptr<Material> mMaterial;
	

	ComPtr<ID3D11Buffer> mInitVB;
	ComPtr<ID3D11Buffer> mDrawVB;
	ComPtr<ID3D11Buffer> mStreamOutVB;

	Vec3 mEmitPosW;
	Vec3 mEmitDirW;
	
	ComPtr<ID3D11ShaderResourceView> mRandomTextureSRV;


};

