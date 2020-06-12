#pragma once
#include "Ui/Core/UiDecorator.hpp"
#include "Ui/Core/UiBuilder.hpp"


namespace App::Ui::Decorators
{	
	class SizeDecorator final : public Core::UiDecorator
	{
		private: float width;

		private: float height;


		
		public: SizeDecorator(float width, float height)
			:
			width{ width },
			height{ height }
		{}
		
		
		public:	void Decorate(Core::UiBuilder& builder) override{ builder.DeclareSize(width, height); }
		
	};
	
}
