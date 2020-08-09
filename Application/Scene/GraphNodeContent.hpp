#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Shared/PtrTypes.hpp"
#include "Ui/Core/UiFrontend.hpp"


namespace App::Ui::Core
{
	class UiLayoutElement;
}

namespace Math{ class Matrix; }

namespace App::Scene
{
	class GraphVisitor;
	
	class GraphNodeContent
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(GraphNodeContent)

		
		public: virtual void Accept(GraphVisitor &visitor) = 0;

		public: virtual UniquePtr<Ui::Core::UiFrontend> MakeContentEditor(Ui::Core::UiLayoutElement &layoutParent) = 0;

		public: virtual UniquePtr<Ui::Core::UiFrontend> MakeTransformEditor(Math::Matrix &transformToEdit, Ui::Core::UiLayoutElement &layoutParent) = 0;
										
	};
	
	
}
