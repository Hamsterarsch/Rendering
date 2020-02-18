cbuffer globals : register(b0)
{
	float4x4 view;
	float4x4 projection;
	float time;
};

cbuffer instance : register(b1)
{
	float4x4 instancePositions[1024];
}


float4 main(float3 pos : POSITION, uint instanceID : SV_InstanceID) : SV_POSITION
{
	return  mul(projection, mul(view, mul(instancePositions[instanceID], float4(pos.xyz, 1))));

}

