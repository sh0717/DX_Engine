#include "GlobalHelper.fx"
#include "LightHelper.fx"



#define MAX_MODEL_TRANSFORMS 250
cbuffer BoneBuffer
{
    matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};



uint BoneIndex;

MeshOutput VS(InstancingVertexTextureNormalTangetBlend input)
{
	MeshOutput output;
  
    output.position = mul(float4(input.position,1.f), BoneTransforms[BoneIndex]);
	//output.position = mul(output.position, input.world);
    output.position = mul(float4(input.position, 1.f), input.world);
    output.worldPosition = output.position.xyz;
	output.position = mul(output.position,ViewProj);
   
	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)World);
	output.tangent = mul(input.tangent, (float3x3)World);

	return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
	 
   
    float4 totalA = float4(0.f, 0.f, 0.f, 0.f);
    float4 totalD = float4(0.f, 0.f, 0.f, 0.f);
    float4 totalS = float4(0.f, 0.f, 0.f, 0.f);
   
    float3 cameraPos = CameraPos;
    
    
    float3 normal = input.normal;
    float3 normalsample = NormalMap.Sample(LinearSampler, input.uv);
   
    //if there is normalmap texture then change the normal vector 
    if(IsNormalMap(input.uv))
    {
        normal = NormalMapExtract(normalsample, input.normal, input.tangent);
        

    }
    
    
    
    int a = numOfDirLight;
    [unroll]
    for(int i = 0; i < 1; ++i)
    {
        float4 A, D, S;
        
        ComputeDirectionalLight(Mat, DirLight, normal, cameraPos - input.worldPosition, A, D, S);
        totalA = totalA + A;
        totalD = totalD + D;
        totalS = totalS + S;
        
    }
       
    
   
    
    
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
   
   
  
    float4 result;
    result.xyz = color.xyz * (totalA + totalD + totalS).xyz;
    result.a = color.a * Mat.Diffuse.a;
    return result;
}

float4 PS_RED(MeshOutput input) : SV_TARGET
{
	return float4(1, 0, 0, 1);
}

technique11 T0
{
	PASS_VP(P0, VS, PS)
	//PASS_RS_VP(P1, FillModeWireFrame, VS, PS_RED)
};
