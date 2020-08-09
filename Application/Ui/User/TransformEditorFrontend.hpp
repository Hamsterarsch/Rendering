#pragma once
#include "Ui/Core/UiFrontend.hpp"
#include "Types/Vector.hpp"


namespace Math { class Matrix; }

namespace App::Ui::Core { class UiLayoutElement; }


namespace App::Ui::User
{
	class TransformEditorFrontend final : public Core::UiFrontend
	{
		private: Math::Matrix *target;

		private: Math::Vector3 posWriteTarget;

		private: Math::Vector3 lastPos;
		
		private: Math::Vector3 scaleWriteTarget;

		private: Math::Vector3 lastScale;

		private: Math::Vector3 rotWriteTarget;

		private: Math::Vector3 lastRot;
		

		
		public: explicit TransformEditorFrontend(Math::Matrix &transformToTarget, Core::UiLayoutElement *parentLayout);
				

		public: void Update(Core::UiBuilder &builder) override;
		
		public: float *GetInputTargetFloat(size_t index) override;

		private: float &GetVectorMemberByIndex(Math::Vector3 &vector, size_t index);
		
	};

	
}
