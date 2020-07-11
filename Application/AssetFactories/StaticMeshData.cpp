#include "AssetFactories/StaticMeshData.hpp"
#include "Renderer/Interface/StateSettings/VertexLayoutSettings.hpp"
#include "IO/Archive.hpp"
#include "Shared/Exception/Exception.hpp"


namespace App::Assets
{
	assetSystem::io::Archive &StaticMeshData::Serialize(assetSystem::io::Archive &archive)
	{
		if(vertices.size() > std::numeric_limits<uint32_t>::max())
		{
			throw Exception::Exception{"StaticMeshData: Static mesh asset can not be serialized because it contains too many vertices"};
		}		
		auto numVertices{ static_cast<uint32_t>(vertices.size()) };
		
		archive.EnterSubobject("vertexData")
		.Serialize("numVertices", numVertices);
		vertices.resize(numVertices);

		static_assert(sizeof VertexData::pos.x == 4);
		archive.Serialize("vertexData", reinterpret_cast<unsigned char *>(vertices.data()), numVertices*8, 4)
		.LeaveSubobject()
		.EnterSubobject("indexData");

		if(indices.size() > std::numeric_limits<uint32_t>::max())
		{
			throw Exception::Exception{"StaticMeshData: Static mesh asset can not be serialized because it contains too many vertices"};
		}		
		auto numIndices{ static_cast<uint32_t>(indices.size()) };
		archive.Serialize("numIndices", numIndices);
		indices.resize(numIndices);

		return archive
		.Serialize("indexData", reinterpret_cast<unsigned char *>(indices.data()), numIndices, sizeof(decltype(indices)::value_type))
		.LeaveSubobject();
				
	}


	
	void StaticMeshData::ConfigureVertexLayout(Renderer::VertexLayoutSettings &forSettings)
	{		
		forSettings
		.AddLayoutElementDesc(&Renderer::SemanticTargets::TargetPosition, 0, &Renderer::FormatTargets::R32G32B32_Float, 0)
		.AddLayoutElementDesc(&Renderer::SemanticTargets::TargetNormal, 0, &Renderer::FormatTargets::R32G32B32_Float, sizeof VertexData::pos)
		.AddLayoutElementDesc(&Renderer::SemanticTargets::TargetTexcoord, 0, &Renderer::FormatTargets::R32G32_Float, sizeof VertexData::pos + sizeof VertexData::normal);
		
	}

	
}
