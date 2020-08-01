#pragma once
#include "StaticMeshData.hpp"
#include "ThirdParty/tiny_obj_loader.h"

namespace App::Assets
{
	class StaticMeshFactory
	{
		public: static StaticMeshData LoadMeshData(const char *absoluteFilePath);
		
	};
	
	
}
