#pragma once
#include "Ui/Core/UiLayoutElement.hpp"
#include <string>


namespace App::Ui
{
	class GridSlot;
		
	
	class GridLayout final : public Core::UiLayoutElement
	{
		private: static inline unsigned constructedInstances{ 0 };

		private: std::string name;

		private: unsigned numColumns;

		private: unsigned numRows;

		private: UniquePtr<GridSlot> currentSlot;
		

		
		public: GridLayout(unsigned numColumns, unsigned numRows);

		public: ~GridLayout() noexcept override;

		
		public: struct GridCell
		{
			unsigned startColumnIndex{ 0 };
			unsigned startRowIndex{ 0 };
			unsigned spanColumn{ 1 };
			unsigned spanRow{ 1 };
		};
		
		public: GridLayout &AddChildAt(const GridCell &cell, UniquePtr<UiElement> &&child);
		
			private: void DeclareChildPos(unsigned startColumnIndex, unsigned startRowIndex, unsigned spanColumn = 1, unsigned spanRow = 1);
		
		
		protected: void OnChildAdded(UiElement &child) override;

		protected: void OnPreRenderAndQueryChildren(Core::UiBuilder &builder) override;
		
		protected: void OnPreRenderAndQueryChild(Core::UiBuilder &builder, size_t childIndex, UiElement &child) override;
		
		protected: void OnPostRenderAndQueryChild(Core::UiBuilder &builder, size_t childIndex, UiElement &child) override;
		
		protected: void OnPostRenderAndQueryChildren(Core::UiBuilder &builder) override;
		
	};


	
	struct GridChildInfo
	{
		GridLayout::GridCell cellInfo;
		
		UniquePtr<Core::UiElement> child;
		
	};
	
	inline UniquePtr<GridLayout> operator+=(UniquePtr<GridLayout> &&instance, GridChildInfo &&childInfo)
	{
		instance->AddChildAt(childInfo.cellInfo, std::move(childInfo.child));
		
		return std::move(instance);
		
	}
	
	inline UniquePtr<GridLayout> &operator+=(UniquePtr<GridLayout> &instance, GridChildInfo &&childInfo)
	{
		instance->AddChildAt(childInfo.cellInfo, std::move(childInfo.child));
		
		return instance;
		
	}
	
	
}

