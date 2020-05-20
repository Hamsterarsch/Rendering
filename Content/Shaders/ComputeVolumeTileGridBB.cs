#include "Types/VolumeTileGridData.hlsl"
#include "Types/BoundingBox.hlsl"
#include "FlattenVolumeTileGridIndex.hlsl"


ConstantBuffer<VolumeTileGridData> gridData : register(b1);
RWStructuredBuffer<BoundingBox> outBox : register(u1);


float3 FindIntersectionWithZPlane(float planeZ, float3 linevec)
{
	const float scale = planeZ / linevec.z;
	return linevec * scale;
	
}

[numthreads(4, 4, 4)]
void main(uint3 dispatchID : SV_DispatchThreadID)
{
	if(any(dispatchID >= gridData.gridDimensions))
	{
		return;
		
	}

	const float bbNear = gridData.nearDistance * pow(1+gridData.fovTermForDepthCompute, dispatchID.z);
	const float bbFar = gridData.nearDistance * pow(1+gridData.fovTermForDepthCompute, dispatchID.z+1);
	
	const float2 tileDimensions = gridData.screenDimensions / gridData.gridDimensions.xy;
	const float2 tileMinUv = dispatchID.xy * tileDimensions / gridData.screenDimensions;
	const float2 tileMaxUv = (dispatchID.xy+1) * tileDimensions / gridData.screenDimensions;
	
	float3 tileMinDir = mul(gridData.inverseProjection, float4(lerp(float2(-1, 1), float2(1, -1), tileMinUv.xy), 0, 1)).xyz;
	float3 tileMaxDir = mul(gridData.inverseProjection, float4(lerp(float2(-1, 1), float2(1, -1), tileMaxUv.xy), 0, 1)).xyz;
			
	const float3 tileMinNear = FindIntersectionWithZPlane(bbNear, tileMinDir);
	const float3 tileMinFar = FindIntersectionWithZPlane(bbFar, tileMinDir);
		
	const float3 tileMaxNear = FindIntersectionWithZPlane(bbNear, tileMaxDir);
	const float3 tileMaxFar = FindIntersectionWithZPlane(bbFar, tileMaxDir);
	
	const float3 bbMin = min(min(min(tileMinNear, tileMinFar), tileMaxNear), tileMaxFar);
	const float3 bbMax = max(max(max(tileMinNear, tileMinFar), tileMaxNear), tileMaxFar);
	
	BoundingBox bb;
	bb.halfExtents = abs(bbMax-bbMin)/2;
	bb.center = (bbMin+bbMax)*.5;
		
	outBox[FlattenVolumeTileGridIndex(dispatchID, gridData.gridDimensions.xy)] = bb;
	
}
	