#include "Types/GlobalBufferData.hlsl"

cbuffer instance : register(b1)
{
	float4x4 instancePositions[1024];
}


float4 main(float3 pos : POSITION, uint instanceID : SV_InstanceID) : SV_POSITION
{
	return  mul(projection, mul(view, mul(instancePositions[instanceID], float4(pos.xyz, 1))));

}

