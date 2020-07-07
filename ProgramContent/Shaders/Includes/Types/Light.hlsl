struct Light
{
	float3 worldPos;
	float radius;
	float3 worldForwardVector;
	float innerBlendRadians;
	float3 color;
	float outerBlendRadians;	
};

struct LightIndexListInfo
{
	uint offset;
	uint size;
};