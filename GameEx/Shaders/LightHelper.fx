#ifndef _LIGHTHELPER_FX_
#define _LIGHTHELPER_FX_
#include "GlobalHelper.fx"
struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
  
    
    float3 Direction;
    float pad;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    
    float3 Position;
    float Range;

    float3 Att;
    float pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

    float3 Direction;
    float Spot;

    float3 Att;
    float pad;
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
    float4 Emissive;
};



#define MAX_DIRLIGHT_NUM  10
///////////////////////
//////ConstBuffer//////
///////////////////////
cbuffer LightBuffer
{
    DirectionalLight DirLight;
};


cbuffer DirLightBuffer
{
    int numOfDirLight;
    float3 padding;
    DirectionalLight dir_light[MAX_DIRLIGHT_NUM];
};

cbuffer PointLightBuffer
{
    int numOfPointLight;
    float3 padding3;
    DirectionalLight point_light[MAX_DIRLIGHT_NUM];
};

cbuffer DirLightBuffer
{
    int numOfSpotLight;
    float3 padding33;
    DirectionalLight spot_light[MAX_DIRLIGHT_NUM];
};



cbuffer MaterialBuffer
{
    Material Mat;
};

//Texture
Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;
TextureCube CubeMap;

//Texture Array용
Texture2DArray DiffuseMapArray;
Texture2D BlendMap;



//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight(Material mat, DirectionalLight L,
                             float3 normal, float3 toEye,
					         out float4 ambient,
						     out float4 diffuse,
						     out float4 spec)
{
	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -L.Direction;
    lightVec = normalize(lightVec);
    normal = normalize(normal);
    toEye = normalize(toEye);
	// Add ambient term.
    ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if(diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
        
     
    }
    else
    {   
      
        

    }
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a point light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
				   out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
    float3 lightVec = L.Position - pos;
		
	// The distance from surface to light.
    float d = length(lightVec);
	
	// Range test.
    if(d > L.Range)
        return;
		
	// Normalize the light vector.
    lightVec /= d;
	
	// Ambient term.
    ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if(diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }

	// Attenuate
    float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));

    diffuse *= att;
    spec *= att;
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
				  out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
    float3 lightVec = L.Position - pos;
		
	// The distance from surface to light.
    float d = length(lightVec);
	
	// Range test.
    if(d > L.Range)
        return;
		
	// Normalize the light vector.
    lightVec /= d;
	
	// Ambient term.
    ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if(diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
	
	// Scale by spotlight factor and attenuate.
    float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// Scale by spotlight factor and attenuate.
    float att = spot / dot(L.Att, float3(1.0f, d, d * d));

    ambient *= spot;
    diffuse *= att;
    spec *= att;
}

 
 ////input sample value and unit Normal and tangent 
////it return unit real normal as float3



/// bool function (Is there normal map? )
bool IsNormalMap(float2 uv)
{
    float3 sample = NormalMap.Sample(LinearSampler, uv).xyz;
    if(sample.r == 0.f && sample.g == 0.f && sample.b == 0.f)
    {   
        return false;
    }
    return true;
    
}
bool IsNormalMapLevel(float2 uv, float MipLevel)
{
    float3 sample = NormalMap.SampleLevel(LinearSampler, uv,MipLevel).xyz;
    if(sample.r == 0.f && sample.g == 0.f && sample.b == 0.f)
    {
        return false;
    }
    return true;
    
}




float3 NormalMapExtract(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
    //normalmap 없으면 걍 해라 
    if(normalMapSample.r == 0 && normalMapSample.g == 0 && normalMapSample.b == 0)
    {
        return unitNormalW;
    }
    float3 normalT = 2.0f * normalMapSample - 1.0f;
	
    float3 N = unitNormalW;
    float3 T = tangentW - dot(tangentW, N) * N;
    T = normalize(T);
	
    float3 B = cross(N, T);
	
    float3x3 TBN = float3x3(T, B, N);
    float3 ret = mul(normalT, TBN);
    ret = normalize(ret);
    return ret;
	
	
}




#endif