#include "Types/GlobalBufferData.hlsl"
#include "Types/LightingV2P.hlsl"

cbuffer instance : register(b1)
{
	float4x4 instancePositions[1024];
}

struct App2V
{
	float3 pos : POSITION;
	float3 normal : NORMAL;	
	float2 uv : TEXCOORD;
	uint instanceID : SV_InstanceID;
};

LightingV2P main(App2V IN)
{
	LightingV2P OUT;
	
	OUT.worldPos = mul(instancePositions[IN.instanceID], float4(IN.pos, 1)).xyz;
	OUT.viewPos = mul(view, float4(OUT.worldPos)).xyz;
	OUT.pos = mul(projection, float4(OUT.viewPos, 1));
	OUT.normal = normalize(mul(instancePositions[IN.instanceID], float4(IN.normal, 0))).xyz;
	OUT.uv = IN.uv;
		
	return OUT;

}

