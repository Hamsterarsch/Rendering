cbuffer globalData : register(b0)
{
	float4x4 view;
	float4x4 projection;
	float4x4 inverseView;
	float4x4 inverseProjection;
	float time;
};
