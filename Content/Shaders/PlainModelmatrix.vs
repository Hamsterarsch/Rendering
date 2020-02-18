cbuffer globals : register(b0)
{
	float4x4 view;
	float4x4 projection;
	float time;
};

cbuffer instance : register(b1)
{
	float4x4 model;
}


float4 main(float3 pos : POSITION) : SV_POSITION
{
	return  mul(projection, mul(view, mul(model, float4(pos.xyz, 1))));

}

