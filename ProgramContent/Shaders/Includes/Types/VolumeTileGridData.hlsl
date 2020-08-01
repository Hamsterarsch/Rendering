struct VolumeTileGridData
{
	float4x4 inverseProjection;	
	uint3 gridDimensions;
	float fovTermForDepthCompute;
	float2 screenDimensions;
	float nearDistance;
	float farDistance;
		
};