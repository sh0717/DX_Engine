#include "GlobalHelper.fx"
#include "LightHelper.fx"




struct VertexIn
{
    float3 PosL : POSITION;
    
};



struct VertexOut
{
    
    float3 PosW : POSITIONT;
  
};


VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    vout.PosW = mul(float4(vin.PosL,1.f), World).xyz;
   
    return vout;
    
}

struct PatchTess
{
    float EdgeTess[3] : SV_TessFactor;
    float InsideTess : SV_InsideTessFactor;
};



PatchTess ConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
    PatchTess pt;

    pt.EdgeTess[0] = 1;
    pt.EdgeTess[1] = 1;
    pt.EdgeTess[2] = 1;
    
    pt.InsideTess = 1;
    
    return pt;
}

struct HullOut
{
   
    float3 PosW : POSITIONT;
   
};


[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 3> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
    HullOut hout;

    hout.PosW = p[i].PosW;
 

    return hout;
}



struct DomainOut
{
    float4 PosH : SV_Position;
};




[domain("tri")]
DomainOut DS(PatchTess patchTess,
	float3 baryCoords : SV_DomainLocation,
	const OutputPatch<HullOut, 3> tri)
{
    DomainOut dout;

    float3 p = baryCoords.x * tri[0].PosW + baryCoords.y * tri[1].PosW + baryCoords.z * tri[2].PosW;

    dout.PosH = mul(float4(p, 1.0f),ViewProj).xyzw;

    return dout;
}


float4 PS(DomainOut pin):SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
}


technique11 Tess
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetHullShader(CompileShader(hs_5_0, HS()));
        SetDomainShader(CompileShader(ds_5_0, DS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}