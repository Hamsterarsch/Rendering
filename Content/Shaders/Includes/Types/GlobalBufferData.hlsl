cbuffer globalData : register(b0)
{
	float4x4 view;
	float4x4 projection;
	float4x4 inverseViewProjection;
	float time;
};
