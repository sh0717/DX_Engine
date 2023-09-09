#include "GlobalHelper.fx"
#include "LightHelper.fx"
#include "ShadowHelper.fx"

Texture2D gHeightMap;
SamplerState samHeightmap
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;

    AddressU = CLAMP;
    AddressV = CLAMP;
};



cbuffer cbPerFrame
{
    DirectionalLight gDirLights[3];
    float3 gEyePosW;

    float gFogStart;
    float gFogRange;
    float4 gFogColor;

	// When distance is minimum, the tessellation is maximum.
	// When distance is maximum, the tessellation is minimum.
    float gMinDist=0.1;
    float gMaxDist=50;

	// Exponents for power of 2 tessellation.  The tessellation
	// range is [2^(gMinTess), 2^(gMaxTess)].  Since the maximum
	// tessellation is 64, this means gMaxTess can be at most 6
	// since 2^6 = 64.
    float gMinTess=1;
    float gMaxTess=64;

    float gTexelCellSpaceU;
    float gTexelCellSpaceV;
    float gWorldCellSpace;
    float2 gTexScale = 50.0f;

    float4 gWorldFrustumPlanes[6];
};




struct VertexIn
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD0;
    float2 BoundsY : TEXCOORD1;
};

struct VertexOut
{
    float3 PosW : POSITIONT;
    float2 Tex : TEXCOORD0;
    float2 BoundsY : TEXCOORD1;
};



VertexOut VS(VertexIn vin)
{
    VertexOut vout;

	// Terrain specified directly in world space.
    vout.PosW = vin.PosL;
    vout.PosW.y = gHeightMap.SampleLevel(samHeightmap, vin.Tex, 0).r;
	// Displace the patch corners to world space.  This is to make 
	// the eye to patch distance calculation more accurate.
    

	// Output vertex attributes to next stage.
    vout.Tex = vin.Tex;
    vout.BoundsY = vin.BoundsY;

    return vout;
}

struct PatchTess
{
    float EdgeTess[4] : SV_TessFactor;
    float InsideTess[2] : SV_InsideTessFactor;
};



float CalcTessFactor(float3 p)
{
    float d = distance(p, CameraPos);

	// max norm in xz plane (useful to see detail levels from a bird's eye).
	//float d = max( abs(p.x-gEyePosW.x), abs(p.z-gEyePosW.z) );

    float s = saturate((d - gMinDist) / (gMaxDist - gMinDist));

    return pow(2, (lerp(gMaxTess, gMinTess, s)));
}

// Returns true if the box is completely behind (in negative half space) of plane.
bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
    float3 n = abs(plane.xyz);

	// This is always positive.
    float r = dot(extents, n);

	// signed distance from center point to plane.
    float s = dot(float4(center, 1.0f), plane);

	// If the center point of the box is a distance of e or more behind the
	// plane (in which case s is negative since it is behind the plane),
	// then the box is completely in the negative half space of the plane.
    return (s + r) < 0.0f;
}

// Returns true if the box is completely outside the frustum.
bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6])
{
    for(int i = 0; i < 6; ++i)
    {
		// If the box is completely behind any of the frustum planes
		// then it is outside the frustum.
        if(AabbBehindPlaneTest(center, extents, frustumPlanes[i]))
        {
            return true;
        }
    }

    return false;
}

PatchTess ConstantHS(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
    PatchTess pt;

	//
	// Frustum cull
	//

	// We store the patch BoundsY in the first control point.
    float minY = patch[0].BoundsY.x;
    float maxY = patch[0].BoundsY.y;

	// Build axis-aligned bounding box.  patch[2] is lower-left corner
	// and patch[1] is upper-right corner.
    float3 vMin = float3(patch[2].PosW.x, minY, patch[2].PosW.z);
    float3 vMax = float3(patch[1].PosW.x, maxY, patch[1].PosW.z);

    float3 boxCenter = 0.5f * (vMin + vMax);
    float3 boxExtents = 0.5f * (vMax - vMin);

    if(AabbOutsideFrustumTest(boxCenter, boxExtents, gWorldFrustumPlanes))
    {
        pt.EdgeTess[0] = 0.0f;
        pt.EdgeTess[1] = 0.0f;
        pt.EdgeTess[2] = 0.0f;
        pt.EdgeTess[3] = 0.0f;

        pt.InsideTess[0] = 0.0f;
        pt.InsideTess[1] = 0.0f;

        return pt;
    }
	//
	// Do normal tessellation based on distance.
	//
    else
    {
		// It is important to do the tess factor calculation based on the
		// edge properties so that edges shared by more than one patch will
		// have the same tessellation factor.  Otherwise, gaps can appear.

		// Compute midpoint on edges, and patch center
        float3 e0 = 0.5f * (patch[0].PosW + patch[2].PosW);
        float3 e1 = 0.5f * (patch[0].PosW + patch[1].PosW);
        float3 e2 = 0.5f * (patch[1].PosW + patch[3].PosW);
        float3 e3 = 0.5f * (patch[2].PosW + patch[3].PosW);
        float3 c = 0.25f * (patch[0].PosW + patch[1].PosW + patch[2].PosW + patch[3].PosW);

        pt.EdgeTess[0] = CalcTessFactor(e0);
        pt.EdgeTess[1] = CalcTessFactor(e1);
        pt.EdgeTess[2] = CalcTessFactor(e2);
        pt.EdgeTess[3] = CalcTessFactor(e3);

        pt.InsideTess[0] = CalcTessFactor(c);
        pt.InsideTess[1] = pt.InsideTess[0];

        return pt;
    }
    
}




struct HullOut
{
    float3 PosW : POSITIONT;
    float2 Tex : TEXCOORD0;
    
};



[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 4> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
    HullOut hout;

	// Pass through shader.
    hout.PosW = p[i].PosW;
    hout.Tex = p[i].Tex;

    return hout;
}


struct DomainOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITIONT;
    float2 Tex : TEXCOORD0;
    float4 ShadowPosH : TEXCOORD1;
};


[domain("quad")]
DomainOut DS(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
    DomainOut dout;

	// Bilinear interpolation.
    dout.PosW = lerp(
		lerp(quad[0].PosW, quad[1].PosW, uv.x),
		lerp(quad[2].PosW, quad[3].PosW, uv.x),
		uv.y);

    dout.Tex = lerp(
		lerp(quad[0].Tex, quad[1].Tex, uv.x),
		lerp(quad[2].Tex, quad[3].Tex, uv.x),
		uv.y);

	// Tile layer textures over terrain.
    dout.PosW.y = gHeightMap.SampleLevel(samHeightmap, dout.Tex, 0).r;

	// Displacement mapping
  

	// NOTE: We tried computing the normal in the shader using finite difference, 
	// but the vertices move continuously with fractional_even which creates
	// noticable light shimmering artifacts as the normal changes.  Therefore,
	// we moved the calculation to the pixel shader.  

	// Project to homogeneous clip space.
    dout.PosH = mul(float4(dout.PosW, 1.0f), ViewProj);
    dout.ShadowPosH = mul(float4(dout.PosW, 1.f), shadowtransform);

    return dout;
}

float4 PS(DomainOut pin):SV_Target
{
   float2 TiledTex= pin.Tex*gTexScale;
    float4 c0 = DiffuseMapArray.Sample(LinearSampler, float3(TiledTex, 0.0f));
    float4 c1 = DiffuseMapArray.Sample(LinearSampler, float3(TiledTex, 1.0f));
    float4 c2 = DiffuseMapArray.Sample(LinearSampler, float3(TiledTex, 2.0f));
    float4 c3 = DiffuseMapArray.Sample(LinearSampler, float3(TiledTex, 3.0f));
    float4 c4 = DiffuseMapArray.Sample(LinearSampler, float3(TiledTex, 4.0f));
    
    float4 t = BlendMap.Sample(LinearSampler, pin.Tex);
    float4 texColor = c0;
    texColor = lerp(texColor, c1, t.r);
    texColor = lerp(texColor, c2, t.g);
    texColor = lerp(texColor, c3, t.b);
    texColor = lerp(texColor, c4, t.a);
    
    float3 shadow = float3(1.0f, 1.0f, 1.0f);
    shadow[0] = CalcShadowFactor(samShadow, ShadowMap, pin.ShadowPosH);
    
    return texColor*shadow[0];
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