#include "GlobalHelper.fx"
#include "LightHelper.fx"
#include "ShadowHelper.fx"




struct VertexOut
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITIONT;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 ShadowPosH : TEXCOORD1;
};



VertexOut VS(InstancingVertexTextureNormalTangetBlend input)
{
    VertexOut output;
    output.position = mul(float4(input.position,1.f), input.world);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);

    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) World);
    output.normal = normalize(output.normal);
    
    
    output.tangent = mul(input.tangent, (float3x3) World);
    output.tangent = normalize(output.tangent);
    output.ShadowPosH = mul(float4(output.worldPosition, 1.0f), shadowtransform);
    
    return output;
}



float4 PS(VertexOut input) : SV_TARGET
{   
    
    //return DirLight.Ambient;
    
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
    float3 shadow = float3(1.0f, 1.0f, 1.0f);
    shadow[0] = CalcShadowFactor(samShadow, ShadowMap, input.ShadowPosH);
    
    
    [unroll]
    for(int i = 0; i <1; ++i)
    {
        float4 A, D, S;
        
            ComputeDirectionalLight(Mat, dir_light[0], normal, cameraPos - input.worldPosition, A, D, S);
            totalA =totalA+ A;
            totalD =totalD+shadow[i]*D;
            totalS = totalS+shadow[i]*S;
        
        //totalD += D;
        //totalS += S;    
        
    }
       
  
   
  
    
    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
    
    
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
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetRasterizerState(FillModeWireFrame);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};
