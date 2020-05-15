#include "Types/BoundingBox.hlsl"
#include "Types/VolumeTileGridData.hlsl"
#include "Types/Light.hlsl"


ConstantBuffer<VolumeTileGridData> gridData : register(b1);

StructuredBuffer<Light> lights : register(t0);
StructuredBuffer<BoundingBox> gridAABBs : register(t1);
StructuredBuffer<uint> activeTiles : register(t2);

RWBuffer<uint> relevantLightIndices : register(u2);//bound to UINT16 formatted buffer
RWBuffer<LightIndexListInfo> offsetsToRelevantLightChunks : register(u3);


[numthreads(4, 4, 4)]
void main(uint3 dispatchID : SV_DispatchThreadID)
{
	uint stride;	
	uint numberOfTiles;
	activeTiles.GetDimensions(numberOfTiles, stride);
	
	if(dispatchID.x >= numberOfTiles)
	{
		return;
		
	}
	
	uint numberOfLights;
	lights.GetDimensions(numberOfLights, stride);
		
	BoundingBox tileAABB = activeTiles[dispatchID.x]
	
	uint intersectedLights = 0;
	for(uint lightIndex = 0; lightIndex < numberOfLights; ++lightIndex)
	{
		if(LightIntersectsAABB( lights[lightIndex], tileAABB ))
		{
			relevantLightIndices[numberOfLights*dispatchID.x +intersectedLights] = lightIndex;
			++intersectedLight;
		}				
	}
	
	LightIndexListInfo info;
	info.offset = numberOfLights * dispatchID.x;
	info.size = intersectedLights;
	
	offsetsToRelevantLightChunks[activeTiles[dispatchID.x]] = info;	
	
}