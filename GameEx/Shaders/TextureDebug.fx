#include "GlobalHelper.fx"
#include "LightHelper.fx"
float4x4 gWorldViewProj;
Texture2D gTexture;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
    float3 NormalL : NORMAL;
  
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    vout.PosH = mul(float4(vin.PosL, 1.0f), World);
   vout.PosH = mul(vout.PosH, ViewProj);
    vout.Tex = vin.Tex;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
   
    return DiffuseMap.Sample(samLinear, pin.Tex).rrrr;
}

float4 PSrgba(VertexOut pin) : SV_Target
{
   
    return DiffuseMap.Sample(samLinear, pin.Tex).rgba;
}



technique11 ViewArgbTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }


    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSrgba()));
    }
   
}
