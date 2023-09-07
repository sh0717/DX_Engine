#include "GlobalHelper.fx"
#include "LightHelper.fx"


struct VertexIn
{
    float3 position : POSITION;
    float2 Size : SIZE;
};

struct VertexOut
{
    float3 center : POSITIONT;
    float2 size : SIZE;
};


cbuffer cbFixed
{
	//
	// Compute texture coordinates to stretch texture over quad.
	//

    float2 gTexC[4] =
    {
        float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
    };
};



VertexOut VS(VertexIn vin)
{
    VertexOut vout;

	// Just pass data over to geometry shader.
    vout.center = vin.position ;
    vout.size = vin.Size;

    return vout;
}
 
struct GeoOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITIONT;
    float3 NormalW : NORMAL;
    float2 Tex : TEXCOORD;
    uint PrimID : SV_PrimitiveID;
};


[maxvertexcount(4)]
void GS(point VertexOut gin[1], uint primID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{
	//
	// Compute the local coordinate system of the sprite relative to the world
	// space such that the billboard is aligned with the y-axis and faces the eye.
	//

    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 look = CameraPos - gin[0].center;
    look.y = 0.0f; // y-axis aligned, so project to xz-plane
    look = normalize(look);
    float3 right = cross(up, look);

	//
	// Compute triangle strip vertices (quad) in world space.
	//
    float halfWidth = 0.5f * gin[0].size.x;
    float halfHeight = 0.5f * gin[0].size.y;
	
    float4 v[4];
    v[0] = float4(gin[0].center + halfWidth * right - halfHeight * up, 1.0f);
    v[1] = float4(gin[0].center + halfWidth * right + halfHeight * up, 1.0f);
    v[2] = float4(gin[0].center - halfWidth * right - halfHeight * up, 1.0f);
    v[3] = float4(gin[0].center - halfWidth * right + halfHeight * up, 1.0f);

	//
	// Transform quad vertices to world space and output 
	// them as a triangle strip.
	//
    GeoOut gout;
	[unroll]
    for(int i = 0; i < 4; ++i)
    {
        gout.PosH = mul(v[i], ViewProj);
        gout.PosW = v[i].xyz;
        gout.NormalW = look;
        gout.Tex = gTexC[i];
        gout.PrimID = primID;
		
        triStream.Append(gout);
    }
}



float4 PS(GeoOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform bool gAlphaClip, uniform bool gFogEnabled) : SV_Target
{
    float4 totalA = float4(0.f, 0.f, 0.f, 0.f);
    float4 totalD = float4(0.f, 0.f, 0.f, 0.f);
    float4 totalS = float4(0.f, 0.f, 0.f, 0.f);
   
    float3 cameraPos = CameraPos;
    
    float4 color = DiffuseMap.Sample(LinearSampler, pin.Tex);
    
    
    clip(color.a - 0.05f);  
    //return color;
    
    int a = numOfDirLight;
    
    
    [unroll]
    for(int i = 0; i < 1; ++i)
    {
        float4 A, D, S;
        
        ComputeDirectionalLight(Mat,DirLight, pin.NormalW, cameraPos - pin.PosW, A, D, S);
        totalA = totalA + A;
        totalD = totalD + D;
        totalS += totalS + S;
        
    }
       
    
   
    
    
   
    
    
    float4 result;
    result.xyz = color.xyz * (totalA + totalD + totalS).xyz;
    result.a = color.a * Mat.Diffuse.a;
    return result;
    
    
 
}



technique11 Light3
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader(CompileShader(ps_5_0, PS(3, false, false, false)));
    }
}