#include "common.hlsli"

#define SMAA_HLSL_3
uniform float4 screen_res;
#define SMAA_RT_METRICS screen_res.zwxy
#define SMAA_PRESET_ULTRA

#include "smaa.hlsli"

SMAATexture2D(s_blendtex);

struct p_smaa
{
    float2 tc0 : TEXCOORD0; // Texture coordinates         (for sampling maps)
};

float4 main(p_smaa I) : COLOR
{
    float4 offset;
    SMAANeighborhoodBlendingVS(I.tc0, offset);
    return SMAANeighborhoodBlendingPS(I.tc0, offset, s_image, s_blendtex);
}
