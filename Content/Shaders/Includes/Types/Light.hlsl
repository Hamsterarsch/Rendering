struct Light
{
	float3 viewPos;
	float radius;
	float3 forwardVector;
	float innerBlendRadians;
	float3 color;
	float outerBlendRadians;	
};

struct LightIndexListInfo
{
	uint offset;
	uint size;
};