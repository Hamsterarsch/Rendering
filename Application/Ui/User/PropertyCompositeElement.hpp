#pragma once
#include "Ui/Core/UiElement.hpp"
#include "Ui/Elements/GridLayout.hpp"
#include "Ui/Elements/TextElement.hpp"
#include "Ui/Elements/InputElement.hpp"


namespace App::Ui::User
{
	class PropertyCompositeElement : public Core::UiElement
	{
		private: GridLayout layout;				 		

		private: size_t propertyCount;

		private: SharedPtr<Core::UiFrontend> frontend;

		private: static constexpr unsigned columnCount{ 4 };

		
		public: PropertyCompositeElement(const SharedPtr<Core::UiFrontend> &frontend);

		public: template<class t> void AddProperty(const char *name, size_t frontentTargetIndex);

		
		public: void RenderAndQueryInternal(Core::UiBuilder &builder) override;

		
		
	};

	template <class t>
	void PropertyCompositeElement::AddProperty(const char *name, size_t frontendTargetIndex)
	{
		layout.DeclareChildPos(0, propertyCount)
		.AddChild(MakeUnique<TextElement>(name));

		layout.DeclareChildPos(1, propertyCount, columnCount - 1)
		.AddChild(MakeUnique<InputElement<t>>(frontend, frontendTargetIndex, name));

		++propertyCount;
		
	}

	
}
