matrix World;
matrix View;
matrix Projection;
Texture2D Texture0;
Texture2D HeightMap;


struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float fa : COLOR;
};


SamplerState Sampler0
{
    AddressU = Wrap;
    AddressV = Wrap;
};

VertexOutput VS(VertexInput input)
{
    //VS 에서 사용하려면 SampleLevel 함수를 사용한다.
    float height = HeightMap.SampleLevel(Sampler0, input.uv,1).x*25;
    
    input.position.y = height;
    VertexOutput output;
    output.position = mul(input.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);

    output.uv = input.uv;
        output.fa = input.position.y;
    return output;
}



float4 PS(VertexOutput input) : SV_TARGET
{
   float4 color = Texture0.Sample(Sampler0, input.uv);
    color.r = input.fa;
    return color;
}

RasterizerState FillModeWireFrame
{
    FillMode = Wireframe;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetRasterizerState(FillModeWireFrame);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};
