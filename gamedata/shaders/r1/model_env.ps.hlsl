#include "common.hlsli"

struct v2p
{
    float4 diffuse : COLOR0;
    float4 tc0 : TEXCOORD0; // projector
    float4 tc1 : TEXCOORD1; // env
    float4 tc2 : TEXCOORD2; // base
};

uniform sampler2D s_projector;

// Pixel
float4 main(v2p I) : COLOR
{
    float4 light = I.diffuse + tex2D(s_projector, I.tc0);
    float4 t_env = texCUBE(s_env, I.tc1);
    float4 t_base = tex2D(s_base, I.tc2);
    float4 base = lerp(t_env, t_base, t_base.a);
    return light * base * 2;
}
