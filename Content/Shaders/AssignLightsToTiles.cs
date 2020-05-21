#include "Types/GlobalBufferData.hlsl"
#include "Types/BoundingBox.hlsl"
#include "Types/VolumeTileGridData.hlsl"
#include "Types/Light.hlsl"


ConstantBuffer<VolumeTileGridData> gridData : register(b1);

StructuredBuffer<Light> lights : register(t0);
StructuredBuffer<BoundingBox> gridAABBs : register(t1);
StructuredBuffer<uint> activeTiles : register(t2);

RWBuffer<uint> relevantLightIndices : register(u1);//bound to UINT16 formatted buffer
RWBuffer<LightIndexListInfo> offsetsToRelevantLightChunks : register(u2);

bool LightIntersectsAABB(float lightRadius, float3 viewLightPos, BoundingBox tileBB)
{
	const float3 minTile = tileBB.center - tileBB.halfExtents;
	const float3 maxTile = tileBB.center + tileBB.halfExtents;
	
	const float3 minLight = viewLightPos - lightRadius;
	const float3 maxLight = viewLightPos + lightRadius;
	
	bool3 r1 = minLight <= maxTile;
	bool3 r2 = maxLight >= minTile;
	
	return dot(r1, r2) >= 3;
		
}

[numthreads(32, 1, 1)]
void main(uint3 dispatchID : SV_DispatchThreadID)
{	
	uint numberOfTiles = activeTiles[0]-1;
	
	if(dispatchID.x >= numberOfTiles)
	{
		return;
		
	}
	
	uint numberOfLights;
	uint stride;
	lights.GetDimensions(numberOfLights, stride);
		
	BoundingBox tileAABB = gridAABBs[activeTiles[dispatchID.x+1]];
	
	uint intersectedLights = 0;
	for(uint lightIndex = 0; lightIndex < numberOfLights; ++lightIndex)
	{
		const float3 viewLightPos = mul(view, float4(lights[lightIndex].worldPos, 1)).xyz;
		if(LightIntersectsAABB( lights[lightIndex].radius, viewLightPos, tileAABB ))
		{
			relevantLightIndices[numberOfLights*dispatchID.x +intersectedLights] = lightIndex;
			++intersectedLights;
		}				
	}
	
	LightIndexListInfo info;
	info.offset = numberOfLights * dispatchID.x;
	info.size = intersectedLights;
	
	offsetsToRelevantLightChunks[activeTiles[dispatchID.x+1]] = info;	
	
}