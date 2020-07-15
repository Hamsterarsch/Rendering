#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace Renderer{ class VertexLayoutSettings; }


namespace App::Assets
{
	class MeshData
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(MeshData)
		
		
		public: virtual const void *GetVertexData() const = 0;

		public: virtual size_t GetVertexDataSizeInBytes() const = 0;

		public: virtual const unsigned *GetIndexData() const = 0;

		public: virtual size_t GetIndexDataSizeInBytes() const = 0;

		public: virtual unsigned GetNumIndices() const = 0;
		
		public: virtual void ConfigureVertexLayout(Renderer::VertexLayoutSettings &forSettings) = 0;
				
	};

	
}
