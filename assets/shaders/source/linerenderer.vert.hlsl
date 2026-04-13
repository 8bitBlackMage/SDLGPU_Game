

struct Input 
{
    float2 vert: TEXCOORD0;
};

struct Output
{
    float4 Color : TEXCOORD0;
    float4 Position : SV_Position;
};

cbuffer UniformBlock : register(b0, space1)
{
    float4x4 ViewMatrix : packoffset(c0);
};

StructuredBuffer<float2> DataBuffer: register(t0, space0);


Output main(uint id: SV_VertexID)
{
    float2 position = DataBuffer[id];
    Output output;

    output.Position = mul(ViewMatrix, float4(position,1.0f,1.0f));
    output.Color = float4(1.0f,0.0f,0.0f,1.0f);

    return output;
}
