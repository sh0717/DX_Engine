

Texture2DArray<float4> Input;
RWTexture2DArray<float4> Output;

[numthreads(32, 32, 1)]
void CS(uint3 id : SV_DispatchThreadID)
{
	float4 color = Input.Load(int4(id, 0));

	//Output[id] = color;
	Output[id] = 1.0f - color;
	//Output[id] = (color.r + color.g + color.b) / 3.0f;
}

[numthreads(32, 32, 1)]
void CS1(uint3 id : SV_DispatchThreadID)
{
    float4 color = Input.Load(int4(id, 0));
    color.r = 1.f;
	//Output[id] = color;
    Output[id] = color;
	//Output[id] = (color.r + color.g + color.b) / 3.0f;
}

[numthreads(32, 32, 1)]
void CS2(uint3 id : SV_DispatchThreadID)
{
    float4 color = Input.Load(int4(id, 0));

	//Output[id] = color;
    Output[id] = color - float4(0.3f,0.3f,0.3f,0.f);
	//Output[id] = (color.r + color.g + color.b) / 3.0f;
}
[numthreads(32, 32, 1)]
void CS3(uint3 id : SV_DispatchThreadID)
{
    float4 color = Input.Load(int4(id, 0));
    color.r = 0.f;
    color.b = 0.f;
	//Output[id] = color;
    Output[id] = color;
	//Output[id] = (color.r + color.g + color.b) / 3.0f;
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
    pass P1
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CS1()));
    }
    pass P2
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CS2()));
    }
    pass P3
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CS3()));
    }
};
