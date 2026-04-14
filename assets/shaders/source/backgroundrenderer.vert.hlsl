
struct Output
{
    float4 Color : TEXCOORD0;
    float4 Position : SV_Position;
};
cbuffer UniformBlock : register(b0, space1)
{
    float4 colour : packoffset(c0);
};

static const uint triangleIndices[6] = {0,1,2,3,2,1};

static const float2 vertexPos[4] = {
    {-1.0f, -1.0f},
    {1.0f, -1.0f},
    {-1.0f, 1.0f},
    {1.0f, 1.0f}
};

Output main(uint id: SV_VertexID)
{
    uint vert = triangleIndices[id % 6];
    float2 coord = vertexPos[vert];
    Output output;
    output.Position = float4(coord,1.0f,1.0f);
    output.Color = colour;


    return output;
}