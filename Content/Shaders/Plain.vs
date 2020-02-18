cbuffer globals : register(b0)
{
	float4x4 view;
	float4x4 projection;
	float time;
};

float4 main(float3 pos : POSITION) : SV_POSITION
{
	return  mul(projection, mul(view, float4(pos.xyz, 1)));

}