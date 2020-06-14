#pragma once
#include "Ui/Core/UiFrontend.hpp"


namespace App::Ui
{
	class SetVisibilityFrontend final : public Core::UiFrontend
	{
		private: bool wasNotified;

		private: bool onlyVisibleWhileTrue;

		private: bool toggleInsteadOfSet;

		private: Core::UiElement *targetElement;


		public: SetVisibilityFrontend();

		public: SetVisibilityFrontend(Core::UiElement &targetElement, bool onlyVisibleWhileTrue = false, bool toggleInsteadOfSet = false);
		
		public:	void Update(Core::UiBuilder &builder) override;

			private: void DoToggleUpdate();

			private: void DoSetUpdate();
		
		public: bool *GetInputTargetBool(size_t index) override;
		
	};
	
	
}
