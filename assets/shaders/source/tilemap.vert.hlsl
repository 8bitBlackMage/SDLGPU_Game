
struct TileData {
    float3 Position;
    float Padding;
    float2 Scale;
    float Padding1;
    float Padding2;
    float TexU, TexV, TexW, TexH;
    float4 Colour;
};

struct Output
{
    float2 TexCoord: TEXCOORD0;
    float4 Color : TEXCOORD1;
    float4 Position : SV_Position;
};

cbuffer UniformBlock : register(b0, space1)
{
    float4x4 ViewMatrix : packoffset(c0);
};

StructuredBuffer<TileData> DataBuffer: register(t0,space0);

static const uint triangleIndices[6] = {0,1,2,3,2,1};

static const float2 vertexPos[4] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f}
};

Output main(uint id: SV_VertexID)
{
    uint tileIndex = id /6;
    uint vert = triangleIndices[id % 6];
    TileData tile = DataBuffer[tileIndex];

    float2 texcoord[4] = {
        {tile.TexU,               tile.TexV              },
        {tile.TexU + tile.TexW,   tile.TexV              },
        {tile.TexU,               tile.TexV + tile.TexH  },
        {tile.TexU + tile.TexW,   tile.TexV + tile.TexH  }
    };

    float2 coord = vertexPos[vert];
    coord *= tile.Scale;
    float3 coordWithDepth = float3(coord + tile.Position.xy, tile.Position.z);

    Output output;

    output.Position = mul(ViewMatrix , float4(coordWithDepth, 1.0f));
    output.TexCoord = texcoord[vert];
    output.Color = tile.Colour;

    return output;
}