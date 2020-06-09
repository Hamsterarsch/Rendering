#pragma once
#include "Ui/Core/WidgetBehavior.hpp"
#include <string>
#include "Windows/SelectPathDialog.hpp"


namespace App::Ui::Client
{
	struct PassAllFilter
	{
		static bool CharacterIsForbidden(char character) { return false; }
	};
	
	template<class t_filter = PassAllFilter>
	class StringInputTargetImpl final : public StringInputTarget
	{
		public: std::string data;

		public: bool isReadOnly{ false };


		public: StringInputTargetImpl(bool isReadOnly = false) : isReadOnly{ isReadOnly } {}
		
		public:	void Resize(size_t toLength) override{ data.resize(toLength); }

		public: size_t GetCapacity() const override{ return data.capacity(); }

		public: char* GetBuffer() override { return data.data(); }

		public: bool CharacterIsForbidden(char character) const override { return t_filter::CharacterIsForbidden(character); }

		public: bool IsReadOnly() const override { return isReadOnly; }

	};
	
	
	class CreateProjectDialogBehavior final : public WidgetBehavior
	{
		private: bool shouldSelectFolder;

		private: bool shouldCreateProject;

		public: StringInputTargetImpl<> selectedFolder;

		public: StringInputTargetImpl<Filter::AssetNameFilter> projectName;



		public: CreateProjectDialogBehavior() : shouldSelectFolder{ false }, shouldCreateProject{ false }, selectedFolder{ true } {}
		
		
		public:	void Update() override
		{
			if(shouldSelectFolder)
			{
				const Windows::SelectPathDialog dialog{};
				selectedFolder.data = dialog.GetSelectedItem().string();
			}
			
		}
		
		bool *GetInputTargetBool(size_t index) override
		{
			if(index == 0)
			{
				return &shouldSelectFolder;				
			}

			return &shouldCreateProject;
			
		}
				
		StringInputTarget* GetInputTargetString(size_t index) override
		{
			if(index == 0)
			{
				return &selectedFolder;
			}

			return &projectName;
			
		}
	};
	
	
}
