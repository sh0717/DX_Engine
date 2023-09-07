

Texture2D ShadowMap;

cbuffer ShadowTransform
{
    float4x4 shadowtransform;
    float4x4 shadowview;
    float4x4 shadowproj;
};


SamplerComparisonState samShadow
{
    Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    AddressU = BORDER;
    AddressV = BORDER;
    AddressW = BORDER;
    BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ComparisonFunc = LESS;
};




static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;



float CalcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowPosH)
{
    shadowPosH.xyz /= shadowPosH.w;
	/*w division  */
	
    float depth = shadowPosH.z;
	/*light 에서 봤을 때의 깊이 => depth */
	
    const float dx = SMAP_DX;
	
    float percent = 0.0f;
	
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    };
	
	/*pcf part <=*/ 
	
	[unroll]
    for(int i = 0; i < 9; i++)
    {
		/*LESS_EQUAL
		통과하면 1 
		*/
        ////이거 그 뭐였지? 그 울림현상 방지하는건데 
        percent += shadowMap.SampleCmpLevelZero(samShadow, shadowPosH.xy + offsets[i], depth-0.01).r;

    }
    percent /= 9.0f;
    return percent;
	
}
