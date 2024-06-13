#include "common_iostructs.hlsli"

uniform float4 scaled_screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_TL main ( v_TL_positiont I )
{
	v2p_TL O;

	{
		I.P.xy += 0.5f;
		O.HPos.x = I.P.x * scaled_screen_res.z * 2 - 1;
		O.HPos.y = (I.P.y * scaled_screen_res.w * 2 - 1)*-1;
		O.HPos.zw = I.P.zw;
	}

	O.Tex0 = I.Tex0;
	O.Color = I.Color.bgra;	//	swizzle vertex colour

 	return O;
}