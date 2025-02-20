#include "common.hlsli"

float luminance(float2 tc)
{
    float3 source = s_image.Sample(smp_rtlinear, tc).xyz;
    return dot(source, LUMINANCE_VECTOR) * def_hdr;
}

float4 main(p_build I) : SV_Target
{
    float4 final;
	
    final.x = luminance(I.Tex0);
    final.y = luminance(I.Tex1);
    final.z = luminance(I.Tex2);
    final.w = luminance(I.Tex3);

    // OK
    return final;
}

