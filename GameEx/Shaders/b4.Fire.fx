#include "GlobalHelper.fx"
#include "LightHelper.fx"


cbuffer ParticleSystemBuffer
{
    float gameTime;
    float mTimeStep;
    float mAge;
    float padding1;
    float3 EmitPosW;
    float padding_;
    float3 EmitDirW;
    float padd_;
};

Texture1D RandomTex;
float3 RandUnitVec3(float offset)
{
	// Use game time plus offset to sample random texture.
    float u = (gameTime + offset+padding1);

	// coordinates in [-1,1]
    float3 v = RandomTex.SampleLevel(LinearSampler, u, 0).xyz;

	// project onto unit sphere
    return normalize(v);
}

cbuffer cbFixed
{
	// Net constant acceleration used to accerlate the particles.
    float3 gAccelW = { 0.0f, 7.8f, 0.0f };

	// Texture coordinates used to stretch texture over quad 
	// when we expand point particle into a quad.
    float2 gQuadTexC[4] =
    {
        float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
    };
};


#define PT_EMITTER 0
#define PT_FLARE 1
DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState NoDepthWrites
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

BlendState AdditiveBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};
struct Particle
{
    float4 InitialPosW : POSITIONT;
    float3 InitialVelW : VELOCITY;
    float2 SizeW : SIZE;
    float Age : AGE;
    uint Type : TYPE;
};

struct ParticleGS
{
    float4 InitialPosW : SV_Position;
    float3 InitialVelW : VELOCITY;
    float2 SizeW : SIZE;
    float Age : AGE;
    uint Type : TYPE;
};


Particle StreamOutVS(Particle vin)
{
    return vin;
}


[maxvertexcount(2)]
void StreamOutGS(point Particle giin[1], inout PointStream<ParticleGS> ptStream)
{
    giin[0].Age += mTimeStep;

    if(giin[0].Type == PT_EMITTER)
    {
		// time to emit a new particle?
        if(giin[0].Age > 0.01f)
        {
            float3 vRandom = RandUnitVec3(0.0f);
            vRandom.x *= 0.5f;
            
            vRandom.z *= 0.5f;

            ParticleGS p;
            p.InitialPosW = float4(EmitPosW.xyz,1.f);
            p.InitialVelW =  4* vRandom;
            p.SizeW = float2(3.0f, 3.0f);
            p.Age = 0.0f;
            p.Type = PT_FLARE;

            ptStream.Append(p);

			// reset the time to emit
            giin[0].Age = 0.0f;
        }

        ParticleGS p=giin[0];
       
		// always keep emitters
        ptStream.Append(p);
    }
    else
    {
		// Specify conditions to keep particle; this may vary from system to system.
        if(giin[0].Age <= 1.0f)
        {
            ParticleGS p = giin[0];
		// always keep emitters
            ptStream.Append(p);
        }
          
    }
}

GeometryShader gsStreamOut = ConstructGSWithSO(CompileShader(gs_5_0, StreamOutGS()), "SV_Position.xyzw;VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");


technique11 StreamOutTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, StreamOutVS()));
        SetGeometryShader(gsStreamOut);

		// disable pixel shader for stream-out only
        SetPixelShader(NULL);

		// we must also disable the depth buffer for stream-out only
        SetDepthStencilState(DisableDepth, 0);
    }
}





/////////////////////////////
////////////////////////////

//이 아래로는 실제로 그리는 거



struct VertexOut
{
    float3 PosW : POSITIONT;
    float2 SizeW : SIZE;
    float4 Color : COLOR;
    uint Type : TYPE;
};

VertexOut DrawVS(Particle vin)
{
    VertexOut vout;

    float t = vin.Age;

	// constant acceleration equation
    vout.PosW = 0.5f * t * t * gAccelW + t * vin.InitialVelW + vin.InitialPosW.xyz;
   // vout.PosW = vin.InitialPosW;
	// fade color with time
    float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
    vout.Color = float4(1.f, 0.2f, 0.2f, opacity);

    vout.SizeW = vin.SizeW;
    vout.Type = vin.Type;

    return vout;
}

struct GeoOut
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

// The draw GS just expands points into camera facing quads.
[maxvertexcount(4)]
void DrawGS(point VertexOut gin[1],
	inout TriangleStream<GeoOut> triStream)
{
	// do not draw emitter particles.
    if(gin[0].Type != PT_EMITTER)
    {
		//
		// Compute world matrix so that billboard faces the camera.
		//
        float3 look = normalize(CameraPos.xyz - gin[0].PosW);
        float3 right = normalize(cross(float3(0, 1, 0), look));
        float3 up = cross(look, right);

		//
		// Compute triangle strip vertices (quad) in world space.
		//
        float halfWidth = 0.5f * gin[0].SizeW.x;
        float halfHeight = 0.5f * gin[0].SizeW.y;

        float4 v[4];
        v[0] = float4(gin[0].PosW + halfWidth * right - halfHeight * up, 1.0f);
        v[1] = float4(gin[0].PosW + halfWidth * right + halfHeight * up, 1.0f);
        v[2] = float4(gin[0].PosW - halfWidth * right - halfHeight * up, 1.0f);
        v[3] = float4(gin[0].PosW - halfWidth * right + halfHeight * up, 1.0f);

		//
		// Transform quad vertices to world space and output 
		// them as a triangle strip.
		//
        GeoOut gout;
		[unroll]
        for(int i = 0; i < 4; ++i)
        {
            gout.PosH = mul(v[i], ViewProj);
            gout.Tex = gQuadTexC[i];
            gout.Color = gin[0].Color;
            triStream.Append(gout);
        }
    }
}

float4 DrawPS(GeoOut pin) : SV_TARGET
{ 
    return DiffuseMapArray.Sample(LinearSampler, float3(pin.Tex,0))*pin.Color;

}

technique11 DrawTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, DrawVS()));
        SetGeometryShader(CompileShader(gs_5_0, DrawGS()));
        SetPixelShader(CompileShader(ps_5_0, DrawPS()));

        SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetDepthStencilState(NoDepthWrites, 0);
    }
}