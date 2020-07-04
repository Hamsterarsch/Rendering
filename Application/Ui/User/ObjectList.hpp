#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "Ui/Elements/CollapsibleNodeLayoutFrontend.hpp"
#include <vector>
#include <functional>


namespace App::Ui{ class FloatLayout; }


namespace App::Ui::User
{
	class ObjectListFrontend final : public Core::UiFrontend
	{
		private: std::vector<UniquePtr<CollapsibleNodeLayoutFrontend>> nodeFrontends;

		private: FloatLayout *list;



		public: ObjectListFrontend();

		public: explicit ObjectListFrontend(Core::UiLayoutElement &parent);

		
		public: void Update(Core::UiBuilder &builder) override;

		public: size_t GetNumEntries() const { return nodeFrontends.size(); }
		
		public: void PushBack(const char *name, UniquePtr<Core::UiElement> &&editUi, UniquePtr<UiFrontend> &&editFrontend);

		public: void PopBack();

		public: void ForEachFrontend(const std::function<void(UiFrontend &)> &operation);

	};

	
}
