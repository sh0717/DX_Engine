#include "GlobalHelper.fx"
#include "LightHelper.fx"

struct VertexOut
{
    float4 position : SV_POSITION;
    float3 PosL : POSITION1;
};
 

VertexOut VS(VertexTextureNormalTangent input)
{
  
    VertexOut vout;
  
    
    float4x4 WVP = mul(World, ViewProj);
    
    //vout.position = mul(input.position, World);
    //vout.position = mul(vout.position, ViewProj);
    
    vout.position = mul(float4(input.position,1.f), WVP);
    
    
    vout.position = vout.position.xyzz;
    
    vout.PosL = input.position.xyz;
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
   // return float4(0.0f,1.f,0.f,1.f);
    return CubeMap.Sample(LinearSampler, pin.PosL);
}


RasterizerState NoCull
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
	// Make sure the depth function is LESS_EQUAL and not just LESS.  
	// Otherwise, the normalized depth values at z = 1 (NDC) will 
	// fail the depth test if the depth buffer was cleared to 1.
    DepthFunc = LESS_EQUAL;
};


technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
       // SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
        
        SetRasterizerState(NoCull);
        SetDepthStencilState(LessEqualDSS, 0);
    }
};
