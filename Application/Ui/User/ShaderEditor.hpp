#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "AssetTypes/ShaderAsset.hpp"
#include "AssetPtr.hpp"
#include "StringInputTargetImpl.hpp"
#include "Ui/Misc/SerializationFilter.hpp"
#include "Ui/Elements/CollapsibleNodeLayout.hpp"

#include "Ui/User/ObjectList.hpp"


namespace App::Ui
{
	class WindowElement;
	class FloatLayout;	
}

namespace App::Ui::States { class UiState; }


namespace App::Ui::User
{
	class ShaderConstantsSlotFrontend : public Core::UiFrontend
	{
		private: Assets::ConstantsResourceSlot slot;


		public: ShaderConstantsSlotFrontend() = default;

		public: ShaderConstantsSlotFrontend(const Assets::ConstantsResourceSlot &slot) : slot{ slot } {}
		
		public: void Update(Core::UiBuilder &builder) override {}
		
		unsigned *GetInputTargetUnsigned(size_t index) override;
		
		public: Assets::ConstantsResourceSlot GetSlot() const { return slot; }

		public: UniquePtr<Core::UiElement> MakeEditUi();
						
	};



	
	class TextureSlotFrontend : public Core::UiFrontend
	{
		private: Assets::TextureResourceSlot slot;

		

		public: TextureSlotFrontend() = default;

		public: TextureSlotFrontend(const Assets::TextureResourceSlot &slot) : slot{ slot } {}
		
		public: void Update(Core::UiBuilder &builder) override {}

		public: unsigned *GetInputTargetUnsigned(size_t index) override;
		
		public: Assets::TextureResourceSlot GetSlot() const { return slot; }

		public: UniquePtr<Core::UiElement> MakeEditUi();

		
	};


	
	class ShaderEditorFrontend final : public Core::UiFrontend
	{
		private: States::UiState *parent;
		
		private: assetSystem::AssetPtrTyped<Assets::ShaderAsset> editingTarget;

		private: std::string shaderName;

		private: StringInputTargetImpl<Filter::SerializationFilter> codeToEdit;

		private: WindowElement *window;
		
		private: bool shouldSave;

		private: bool shouldAbort;

		private: bool shouldAddTextureResource;

		private: bool shouldRemoveTextureResource;

		private: bool shouldAddCbvResource;

		private: bool shouldRemoveCbvResource;

		private: CollapsibleNodeLayoutFrontend textureSlotsNodeFrontend;

		private: using t_textureSlotFrontend = TextureSlotFrontend;

		private: CollapsibleNodeLayoutFrontend constantSlotsNodeFrontend;

		private: using t_constantsSlotFrontend = ShaderConstantsSlotFrontend;

		private: ObjectListFrontend constantsSlotList;
		
		private: ObjectListFrontend textureSlotList;


		

		public: ShaderEditorFrontend(States::UiState &parent, std::string &&shaderName, const assetSystem::AssetPtr &shaderToEdit);

			private: void PushTextureSlotUi(const Assets::TextureResourceSlot &slot);

			private: void PushConstantsSlotUi(const Assets::ConstantsResourceSlot &slot);
		
		public: void Update(Core::UiBuilder &builder) override;

			private: void HandleResourceSlotUpdates(Core::UiBuilder &builder);

				private: void HandleTextureSlotUpdates(Core::UiBuilder &builder);

				private: void HandleConstantsSlotUpdates(Core::UiBuilder &builder);
		
			private: void SaveChanges();

		public: Core::StringInputTarget *GetInputTargetString(size_t index) override;

		public: bool *GetInputTargetBool(size_t index) override;
		
		
	};
	
	
}
