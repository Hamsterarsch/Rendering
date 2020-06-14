#pragma once
#include "Ui/Core/StringInputTarget.hpp"
#include <string>


namespace App::Ui::Filter
{
	struct PassAllFilter
	{
		static bool CharacterIsForbidden(char character) { return false; }
	};
	
}



namespace App::Ui::User
{
	
	template<class t_filter = Filter::PassAllFilter>
	class StringInputTargetImpl final : public Core::StringInputTarget
	{
		private: bool wasTextChanged{ false };
		
		public: std::string data;
		
		public: bool isReadOnly{ false };



		public: StringInputTargetImpl(bool isReadOnly = false) : isReadOnly{ isReadOnly } {}
		
		public:	void Resize(size_t toLength) override{ data.resize(toLength); }

		public: size_t GetCapacity() const override{ return data.capacity(); }

		public: char* GetBuffer() override { return data.data(); }

		public: bool CharacterIsForbidden(char character) const override { return t_filter::CharacterIsForbidden(character); }

		public: bool IsReadOnly() const override { return isReadOnly; }

		public: void NotifyContentChanged() override { wasTextChanged = true; }

		public: bool ContentWasChanged() const override { return wasTextChanged; }
		
		public: void ClearChangedState() override { wasTextChanged = false; }
		
	};

	
}
