#pragma once
#include "Ui/Core/UiDecorator.hpp"
#include "Ui/Core/UiBuilder.hpp"


namespace App::Ui::Decorators
{
	
	class AlignmentDecorator final : public Core::UiDecorator
	{
		private: float alignment;
		
		AlignmentDecorator(float alignment) : alignment{ alignment } {}
		
		public:	void Decorate(Core::UiBuilder& builder) override{ builder.DeclareAlignment(alignment); }
		
	};
	
}



