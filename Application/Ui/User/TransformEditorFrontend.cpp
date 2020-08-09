#include "Ui/User/TransformEditorFrontend.hpp"
#include "Types/Matrix.hpp"
#include "Ui/Elements/FloatLayout.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include "Ui/Elements/GridLayout.hpp"
#include "Ui/Elements/InputElement.hpp"
#include "Ui/Elements/TextElement.hpp"


namespace App::Ui::User
{
	TransformEditorFrontend::TransformEditorFrontend(Math::Matrix &transformToTarget, Core::UiLayoutElement *parentLayout)
		 :
		target{ &transformToTarget },
		scaleWriteTarget{ 1, 1, 1 },
		lastScale{ scaleWriteTarget }
	{
		
		auto ui
		{		
			Element<FloatLayout>(5, true)
				<< Element<TextElement>("Position:")
				<<
				(
					Element<GridLayout>(3, 1)->*Set{&GridLayout::size, {1, 0}}
					+= { {0,0}, Element<InputElement<float>>(*this, 0) }
					+= { {1,0}, Element<InputElement<float>>(*this, 1) }
					+= { {2,0}, Element<InputElement<float>>(*this, 2) }
				)
				<< Element<TextElement>("Rotation:")
				<<
				(				
					Element<GridLayout>(3, 1)->*Set{&GridLayout::size, {1, 0}}
					+= { {0,0}, Element<InputElement<float>>(*this, 3) }
					+= { {1,0}, Element<InputElement<float>>(*this, 4) }
					+= { {2,0}, Element<InputElement<float>>(*this, 5) }	
				)
				<< Element<TextElement>("Scale:")
				<<
				(				
					Element<GridLayout>(3, 1)->*Set{&GridLayout::size, {1, 0}}
					+= { {0,0}, Element<InputElement<float>>(*this, 6) }
					+= { {1,0}, Element<InputElement<float>>(*this, 7) }
					+= { {2,0}, Element<InputElement<float>>(*this, 8) }	
				)
		};

		if(parentLayout)
		{
			parentLayout->AddChild(std::move(ui));
			return;
			
		}

		uiElements.push_front(std::move(ui));
		
		
	}


	
	void TransformEditorFrontend::Update(Core::UiBuilder &builder)
	{
		if(not RenderAndQueryInputForUiElements(builder) && not uiElements.empty())
		{
			return;
			
		}
		

		bool shouldUpdateTarget{ false };
		if(lastPos != posWriteTarget)
		{
			lastPos = posWriteTarget;
			shouldUpdateTarget = true;
		}

		if(lastRot != rotWriteTarget)
		{			
			lastRot = rotWriteTarget;
			shouldUpdateTarget = true;
		}

		if(lastScale != scaleWriteTarget)
		{
			lastScale = scaleWriteTarget;
			shouldUpdateTarget = true;
		}

		if(shouldUpdateTarget)
		{
			*target = 
			Math::Matrix::MakeTranslation(lastPos.x, lastPos.y, lastPos.z)
			* Math::Matrix::MakeRotation(lastRot.x, lastRot.y, lastRot.z)
			* Math::Matrix::MakeScale(lastScale.x, lastScale.y, lastScale.z);			
		}
				
	}


	
	float *TransformEditorFrontend::GetInputTargetFloat(const size_t index)
	{
		if(index < 3)
		{
			return &GetVectorMemberByIndex(posWriteTarget, index);
			
		}

		if(index < 6)
		{
			return &GetVectorMemberByIndex(rotWriteTarget, index);
			
		}
		
		if(index < 9)
		{
			return &GetVectorMemberByIndex(scaleWriteTarget, index);
			
		}

		return nullptr;
		
	}

		float &TransformEditorFrontend::GetVectorMemberByIndex(Math::Vector3 &vector, const size_t index)
		{
			const auto modIndex{ index % 3 };
			switch(modIndex)
			{
			case 0: return vector.x;
			case 1: return vector.y;
			default: return vector.z;
			}
		
		}

	
}
