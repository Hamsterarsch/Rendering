#include "StateSettings/RootSignatureSettingsImpl.hpp"
#include "StateSettings/DescriptorTargetsImpl.hpp"
#include "Shared/Exception/Exception.hpp"


namespace Renderer::DX12
{
	RootSignatureSettingsImpl::State::State() : currentTableParameterOrdinal{ 0 }, currentTableRangesOrdinal{ 0 } {}


	
	RootSignatureSettings &RootSignatureSettingsImpl::AddConstant(const unsigned registerIndex, const unsigned num32BitConstants)
	{
		D3D12_ROOT_PARAMETER1 desc{};

		desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		desc.Constants.ShaderRegister = registerIndex;
		desc.Constants.Num32BitValues = num32BitConstants;

		state.current.parameters.emplace_back(desc);
		
		return *this;
		
	}


	
	RootSignatureSettings &RootSignatureSettingsImpl::DeclareTable()
	{
		FinalizeTableCreation();
		
		D3D12_ROOT_PARAMETER1 desc{};
		desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		
		state.current.parameters.emplace_back(desc);
		state.current.currentTableParameterOrdinal = state.current.parameters.size();

		state.current.tableRanges.emplace_back();
		state.current.currentTableRangesOrdinal = state.current.tableRanges.size();
				
		return *this;
		
	}

		void RootSignatureSettingsImpl::FinalizeTableCreation()
		{
			if(not state.current.parameters.empty() and state.current.parameters.back().ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
			{
				state.current.parameters.back().DescriptorTable.pDescriptorRanges = state.current.tableRanges.at(state.current.currentTableRangesOrdinal-1).ranges.data();
				state.current.parameters.back().DescriptorTable.NumDescriptorRanges = state.current.tableRanges.at(state.current.currentTableRangesOrdinal-1).ranges.size();
			}
		
		}



	RootSignatureSettings &RootSignatureSettingsImpl::AddTableRange
	(
		DescriptorTarget target,
		unsigned baseRegister,
		unsigned numDescriptorsInRange
	)
	{
		D3D12_DESCRIPTOR_RANGE1 desc{};

		desc.BaseShaderRegister = baseRegister;
		desc.NumDescriptors = numDescriptorsInRange;
		desc.OffsetInDescriptorsFromTableStart = state.current.tableRanges.at(state.current.currentTableRangesOrdinal-1).currentNumberOfDescriptors;				
		desc.RangeType = GetTargetValue(target);
		desc.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;

		state.current.tableRanges.at(state.current.currentTableRangesOrdinal-1).ranges.emplace_back(desc);
		state.current.tableRanges.at(state.current.currentTableRangesOrdinal-1).currentNumberOfDescriptors += numDescriptorsInRange;
		
		return *this;
		
	}



	RootSignatureSettings &RootSignatureSettingsImpl::AddSamplerTable(const unsigned baseRegister, const unsigned numSamplers)
	{
		D3D12_DESCRIPTOR_RANGE1 desc{};
		
		desc.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
		desc.BaseShaderRegister = baseRegister;
		desc.NumDescriptors = numSamplers;
		desc.OffsetInDescriptorsFromTableStart = 0;
		desc.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;

		state.current.tableRanges.emplace_back();
		state.current.tableRanges.back().ranges.emplace_back(desc);		
		
		D3D12_ROOT_PARAMETER1 paramDesc{};					
		paramDesc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		paramDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		paramDesc.DescriptorTable.pDescriptorRanges = state.current.tableRanges.back().ranges.data();
		paramDesc.DescriptorTable.NumDescriptorRanges = state.current.tableRanges.back().ranges.size();

		state.current.parameters.emplace_back(paramDesc);

		return *this;
		
	}
	

	
	const std::vector<D3D12_ROOT_PARAMETER1> &RootSignatureSettingsImpl::GetParameters()
	{
		FinalizeTableCreation();
		
		return state.current.parameters;	

	}

	
}
