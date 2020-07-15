#include "AssetFactories/StaticMeshFactory.hpp"
#include "Shared/Debugging.hpp"
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <array>


namespace App::Assets
{
	struct DataIndexToRealIndexMap
	{
		std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, size_t>>> mapping;

		bool Contains(const tinyobj::index_t &index)
		{
			auto itrViMap{ mapping.find(index.vertex_index) };
			if(itrViMap == mapping.end())
			{
				return false;
			}

			auto itrNormMap{ itrViMap->second.find(index.normal_index) };
			if(itrNormMap == itrViMap->second.end())
			{
				return false;
			}

			auto itrUvMap{ itrNormMap->second.find(index.texcoord_index) };
			if(itrUvMap == itrNormMap->second.end())
			{
				return false;
			}

			return true;			
		}

		size_t &operator[](const tinyobj::index_t &index)
		{
			return mapping[index.vertex_index][index.normal_index][index.texcoord_index];
			
		}
		
	};
	
	StaticMeshData StaticMeshFactory::LoadMeshData(const char* absoluteFilePath)
	{
		tinyobj::attrib_t vertexData{};
		std::vector<tinyobj::shape_t> shapes;
				
		std::string warning, error;

		std::ifstream file{ absoluteFilePath, std::ios_base::in };
		if(not file.is_open())
		{
			Exception::DebugBreak();
		}
		
		if(not tinyobj::LoadObj(&vertexData, &shapes, nullptr, &warning, &error, &file))
		{
			Exception::DebugBreak();	
		}

		if(shapes.empty())
		{
			Exception::DebugBreak();	
		}

		if(shapes.size() > 1)
		{
			Exception::DebugBreak();
		}

		StaticMeshData out{};

		auto &mesh{ shapes.at(0).mesh };

		DataIndexToRealIndexMap dataIndexToRealIndex;
		std::vector<std::array<size_t, 3>> faceIndexList;
		
		int numSubgroups{ 1 };
		size_t offsetIntoIndices{ 0 };
		for(unsigned faceIndex{ 0 }; faceIndex < mesh.num_face_vertices.size(); ++faceIndex)
		{
			//find number of material slots
			numSubgroups = std::max(numSubgroups, mesh.material_ids.at(faceIndex)+1);

			//construct real vertices from disconnected data
			for(uint32_t faceIndexIndex{ 0 }; faceIndexIndex < mesh.num_face_vertices.at(faceIndex); ++faceIndexIndex)
			{
				const auto &indexData{ mesh.indices.at(offsetIntoIndices+faceIndexIndex) };
								
				if(dataIndexToRealIndex.Contains(indexData))
				{
					if(faceIndexIndex == 0)
					{
						faceIndexList.emplace_back();
					}
					faceIndexList[faceIndex][faceIndexIndex] = dataIndexToRealIndex[indexData];
					continue;
					
				}
				
				StaticMeshData::VertexData vertex{};
				
				vertex.pos.x = vertexData.vertices.at(indexData.vertex_index*3);
				vertex.pos.y = vertexData.vertices.at(indexData.vertex_index*3+1);
				vertex.pos.z = vertexData.vertices.at(indexData.vertex_index*3+2);

				if(indexData.normal_index >= 0)
				{
					vertex.normal.x = vertexData.normals.at(indexData.normal_index*3);
					vertex.normal.y = vertexData.normals.at(indexData.normal_index*3+1);
					vertex.normal.z = vertexData.normals.at(indexData.normal_index*3+2);
				}
				
				if(indexData.texcoord_index >= 0)
				{
					vertex.uv.x = vertexData.texcoords.at( indexData.texcoord_index*2 );
					vertex.uv.y = vertexData.texcoords.at( indexData.texcoord_index*2+1 );					
				}

				out.vertices.emplace_back(vertex);
				dataIndexToRealIndex[indexData] = out.vertices.size()-1;

				if(faceIndexIndex == 0)
				{
					faceIndexList.emplace_back();
				}				
				faceIndexList[faceIndex][faceIndexIndex] = out.vertices.size()-1;
			}
			offsetIntoIndices += mesh.num_face_vertices.at(faceIndex);
						
		}
		out.submeshes.resize(numSubgroups);

		//todo: add material slot support
		//generate indices and store subisland info		
		for(int submeshIndex{ 0 }; submeshIndex < static_cast<int>(out.submeshes.size()); ++submeshIndex)
		{
			out.submeshes.at(submeshIndex).offsetToIndices = static_cast<uint32_t>(out.indices.size());
						
			size_t offsetIntoIndices{ 0 };
			for(unsigned faceIndex{ 0 }; faceIndex < mesh.num_face_vertices.size(); ++faceIndex)
			{				
				if(mesh.material_ids.at(faceIndex)+1 == submeshIndex)
				{					
					for(auto &&indexToAdd : faceIndexList.at(faceIndex))
					{					
						out.indices.emplace_back(indexToAdd);						
					}
				}
				offsetIntoIndices += mesh.num_face_vertices.at(faceIndex);
			}
			out.submeshes.at(submeshIndex).numIndices = static_cast<uint32_t>(out.indices.size()) - out.submeshes.at(submeshIndex).offsetToIndices;			
		}

		return out;		
		
	}

	
}
