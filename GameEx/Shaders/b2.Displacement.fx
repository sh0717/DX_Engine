#include "GlobalHelper.fx"
#include "LightHelper.fx"

cbuffer cbPerFrame
{

    float gHeightScale=0.07f;
    float gMaxTessDistance=1.f;
    float gMinTessDistance=25.f;
    float gMinTessFactor=1.f;
    float gMaxTessFactor=5.f;
};






struct VertexOut
{
    float3 PosW : POSITIONT;
    float2 Tex : TEXCOORD;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    
    float TessFactor : TESS;    
};


//vertex shader 
VertexOut VS(InstancingVertexTextureNormalTangent Vin)
{
    VertexOut vout;
    
    vout.PosW = mul(float4(Vin.position, 1.f), Vin.world).xyz;
    vout.NormalW = mul(Vin.normal, (float3x3) Vin.world);
    vout.TangentW = mul(Vin.tangent, (float3x3) Vin.world);
    vout.Tex = Vin.uv;
    
    //calculate distance to deciede tessilation factor 
    float d = distance(vout.PosW, CameraPos);
    
    vout.NormalW = normalize(vout.NormalW);
    vout.TangentW = normalize(vout.TangentW);
    
    
    float tess = saturate((gMinTessDistance - d) / (gMinTessDistance - gMaxTessDistance));

    // Rescale [0,1] --> [gMinTessFactor, gMaxTessFactor].
    vout.TessFactor = gMinTessFactor + tess * (gMaxTessFactor - gMinTessFactor);
    
    return vout;
    
}



struct PatchTess
{
    float EdgeTess[3] : SV_TessFactor;
    float InsideTess : SV_InsideTessFactor;
};




PatchTess PatchHS(InputPatch<VertexOut, 3> patch,
    uint patchID : SV_PrimitiveID)
{
    PatchTess pt;

   
    pt.EdgeTess[0] = 0.5f * (patch[1].TessFactor + patch[2].TessFactor);
    pt.EdgeTess[1] = 0.5f * (patch[2].TessFactor + patch[0].TessFactor);
    pt.EdgeTess[2] = 0.5f * (patch[0].TessFactor + patch[1].TessFactor);
    pt.InsideTess = pt.EdgeTess[0];

    return pt;
}


struct HullOut
{
    float3 PosW : POSITIONT;
    float2 Tex : TEXCOORD;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    
};
struct DomainOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITIONT;
    float2 Tex : TEXCOORD;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
   
};



[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchHS")]
HullOut HS(InputPatch<VertexOut, 3> p,
    uint i : SV_OutputControlPointID,
    uint patchId : SV_PrimitiveID)
{
    HullOut hout;

    // Pass through shader.
    hout.PosW = p[i].PosW;
    hout.NormalW = p[i].NormalW;
    hout.TangentW = p[i].TangentW;
    hout.Tex = p[i].Tex;

    return hout;
}



[domain("tri")]
DomainOut DS(PatchTess patchTess,
    float3 bary : SV_DomainLocation,
    const OutputPatch<HullOut, 3> tri)
{
    DomainOut dout;

    // Interpolate patch incide  vertex 
    dout.PosW = bary.x * tri[0].PosW + bary.y * tri[1].PosW + bary.z * tri[2].PosW;
    dout.NormalW = bary.x * tri[0].NormalW + bary.y * tri[1].NormalW + bary.z * tri[2].NormalW;
    dout.TangentW = bary.x * tri[0].TangentW + bary.y * tri[1].TangentW + bary.z * tri[2].TangentW;
    dout.Tex = bary.x * tri[0].Tex + bary.y * tri[1].Tex + bary.z * tri[2].Tex;

    // Interpolating normal can unnormalize it, so normalize it.
    dout.NormalW = normalize(dout.NormalW);

    //
    // Displacement mapping.
    //
    
    const float MipInterval = 20.0f;
    float mipLevel = clamp((distance(dout.PosW, CameraPos) - MipInterval) / MipInterval, 0.0f, 6.0f);
    [flatten]
    if(IsNormalMapLevel(dout.Tex, mipLevel))
    {
        // Choose the mipmap level based on distance to the eye; specifically, choose
        // the next miplevel every MipInterval units, and clamp the miplevel in [0,6].
       

        // Sample height map (stored in alpha channel).
    
        float h = NormalMap.SampleLevel(LinearSampler, dout.Tex, mipLevel).a;
        // Offset vertex along normal.
        dout.PosW += (gHeightScale * (h - 1.0)) * dout.NormalW;
    }

    // Project to homogeneous clip space.
    dout.PosH = mul(float4(dout.PosW, 1.0f), ViewProj);

    return dout;
}



float4 PS(DomainOut input):SV_Target
{
    //return float4(1.f, 0.f, 0.f, 1.f);
    
    
    float4 totalA = float4(0.f, 0.f, 0.f, 0.f);
    float4 totalD = float4(0.f, 0.f, 0.f, 0.f);
    float4 totalS = float4(0.f, 0.f, 0.f, 0.f);
   
    float3 cameraPos = CameraPos;
    
    float3 normal = input.NormalW;
    float3 normalsample = NormalMap.Sample(LinearSampler, input.Tex);
    
    [flatten]
    if(IsNormalMap(input.Tex))
    {
        normal = NormalMapExtract(normalsample, normalize(input.NormalW), normalize(input.TangentW));
    }
    
    
    
    int a = numOfDirLight;
    [unroll]
    for(int i = 0; i < 1; ++i)
    {
        float4 A, D, S;
        
        ComputeDirectionalLight(Mat, dir_light[0], normal, cameraPos - input.PosW, A, D, S);
        totalA = totalA + A;
        totalD = totalD + D;
        totalS += totalS + S;
        
    }
       
    
   
    
    
    float4 color = DiffuseMap.Sample(LinearSampler, input.Tex);
   
    float4 result;
    result.xyz = color.xyz * (totalA + totalD + totalS).xyz;
    result.a = color.a * Mat.Diffuse.a;
    return result;
}




technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetHullShader(CompileShader(hs_5_0, HS()));
        SetDomainShader(CompileShader(ds_5_0, DS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}