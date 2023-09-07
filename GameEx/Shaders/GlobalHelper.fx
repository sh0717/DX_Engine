#ifndef _GLOBALHELPER_FX_
#define _GLOBALHELPER_FX_


cbuffer GlobalBuffer
{
    matrix View;
    matrix Projection;
    matrix ViewProj;
    float3 CameraPos;
    float pad;
};


cbuffer FrustumBuffer
{
    float frumstumplane[6];
    
};
//#define MAX_MODEL_TRANSFORMS 50
//cbuffer BoneBuffer
//{
//    matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
//};


cbuffer TransformBuffer
{
    matrix World;
    matrix TexTransform;
};


struct Vertex
{
    float4 position : POSITION;
};

struct VertexTexture
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexColor
{
    float4 position : POSITION;
    float4 color : COLOR;
};


struct VertexTextureNormal
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    
};
struct VertexTextureNormalTangent
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexTextureNormalTangentBlend
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 blendIndices : BLEND_INDICES;
    float4 blendWeights : BLEND_WEIGHTS;
    
};


struct InstancingVertexTextureNormalTangent
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint instanceID : SV_INSTANCEID;
    matrix world : INST;
};


struct InstancingVertexTextureNormalTangetBlend
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 blendIndices : BLEND_INDICES;
    float4 blendWeights : BLEND_WEIGHTS;
    
    uint instanceID : SV_INSTANCEID;
    matrix world : INST;
};

//////////////////
// VertexOutput //
//////////////////

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
   
};

struct MeshOutput
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITIONT;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
   
   
};

//////////////////
// SamplerState //
//////////////////

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};
SamplerState AnisoSampler
{
    Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

/////////////////////
// RasterizerState //
/////////////////////

RasterizerState FillModeWireFrame
{
    FillMode = WireFrame;
};

///////////
// Macro //
///////////

#define PASS_VP(name, vs, ps)						\
pass name											\
{													\
	SetVertexShader(CompileShader(vs_5_0, vs()));	\
	SetPixelShader(CompileShader(ps_5_0, ps()));	\
}

//////////////
// Function //
//////////////

float3 GetCameraPosition()
{
    return -View._41_42_43;
}
#endif
