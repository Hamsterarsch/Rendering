#include "Types/GlobalBufferData.hlsl"
#include "Types/LightingV2P.hlsl"

cbuffer instance : register(b1)
{
	float4x4 model;
}

struct App2V
{
	float3 pos : POSITION;
	float3 normal : NORMAL;	
};

LightingV2P main(App2V IN)
{
	LightingV2P OUT;
	
	OUT.pos = mul(projection, mul(view, mul(model, float4(IN.pos, 1))))
	OUT.normal = normalize(mul(view, mul(model, float4(IN.normal, 1))));
		
	return OUT;

}

