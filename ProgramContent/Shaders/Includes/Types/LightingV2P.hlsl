struct LightingV2P
{	
	float3 worldPos : TEXCOORD1;
	float3 viewPos : TEXCOORD2;
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};