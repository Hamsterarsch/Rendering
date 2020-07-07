#pragma once
#include "StateSettings/RootSignatureSettings.hpp"
#include <d3d12.h>
#include <vector>
#include "StateImpl.hpp"


namespace Renderer::DX12
{
	class RootSignatureSettingsImpl final : public RootSignatureSettings
	{
		private: struct State
		{
			std::vector<D3D12_ROOT_PARAMETER1> parameters;

			struct RangeInfo
			{
				std::vector<D3D12_DESCRIPTOR_RANGE1> ranges;
				unsigned currentNumberOfDescriptors{ 0 };				
			};
			
			std::vector<RangeInfo> tableRanges;
			
			size_t currentTableParameterOrdinal;

			size_t currentTableRangesOrdinal;
			
			State();
			
		};

		private: StateImpl<State> state;

		
		
		public:	void ResetAllToDefault() override { state.ResetAllToDefault(); }

		public: void SaveSettings() override { state.SaveSettings(); }

		public: void RestoreSettingsToSaved() override { state.RestoreSettingsToSaved(); }
		
		public: void RestoreSettingsToDefault() override { state.RestoreSettingsToDefault(); }
		
		
		public: RootSignatureSettings &AddConstant(unsigned registerIndex, unsigned num32BitConstants) override;
				
		public: RootSignatureSettings &DeclareTable() override;

			private: void FinalizeTableCreation();
		
		public: RootSignatureSettings &AddTableRange(DescriptorTarget target, unsigned baseRegister, unsigned numDescriptorsInRange) override;
		
		RootSignatureSettings &AddSamplerTable(unsigned baseRegister, unsigned numSamplers) override;


		public: const std::vector<D3D12_ROOT_PARAMETER1> &GetParameters();

		RootSignatureSettings& DeclareDataVolatility() override { return *this; }
		RootSignatureSettings& DeclareDescriptorVolatility() override { return *this; }

	};

	
}
