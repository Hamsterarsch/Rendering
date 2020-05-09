#include "Types/GlobalBufferData.hlsl"

float4 main(float3 pos : POSITION) : SV_POSITION
{
	return  mul(projection, mul(view, float4(pos.xyz, 1)));

}