
static const char* g_pixelShader = R"(
#define CONCAT(a, b) a##b

#define DIFFUSE_PS_CPP 1
#define DIFFUSE_PS_HLSL CONCAT(t, DIFFUSE_PS_CPP)

#define SAMPLER_PS_CPP 1
#define SAMPLER_PS_HLSL CONCAT(s, SAMPLER_PS_CPP)

Texture2D ShaderTexture : register(DIFFUSE_PS_HLSL);
SamplerState SampleType : register(SAMPLER_PS_HLSL);

struct UIPixelInput
{
    float4 screenPosition : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

float4 main(UIPixelInput input) : SV_TARGET
{
    clip(input.color.a - 0.0001f);

    float4 color = ShaderTexture.Sample(SampleType, input.tex) * input.color;

    return color;
})";

static const char* g_vertexShader = R"(
#define CONCAT(a, b) a##b

#define PER_OBJECT_VS_CPP 3
#define PER_OBJECT_VS_HLSL CONCAT(b, PER_OBJECT_VS_CPP)

cbuffer CBuf : register(PER_OBJECT_VS_HLSL)
{
    matrix M;
};

struct UIVertexInputType
{
    float2 position : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

struct UIPixelInput
{
    float4 screenPosition : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

UIPixelInput main(UIVertexInputType input)
{
    UIPixelInput output;
    output.screenPosition = mul(float4(input.position, 0.0f, 1.0f), M);
    output.screenPosition.x -= 1.0f;
    output.screenPosition.y -= 1.0f;
    output.screenPosition.y *= -1;
    output.color = input.color;
    output.tex.x = input.tex.x;
    output.tex.y = input.tex.y;
    
    return output;
})";