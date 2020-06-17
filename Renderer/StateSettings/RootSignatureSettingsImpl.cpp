#include "StateSettings/RootSignatureSettingsImpl.hpp"
#include "StateSettings/DescriptorTargetsImpl.hpp"
#include "Shared/Exception/Exception.hpp"


namespace Renderer::DX12
{
	RootSignatureSettingsImpl::State::State() : currentTableParameterOrdinal{ 0 }, currentTableRangesOrdinal{ 0 } {}


	
	void RootSignatureSettingsImpl::ResetToDefault()
	{
		saved = current = State{};
				
	}


	
	void RootSignatureSettingsImpl::SaveSettings()
	{
		saved = current;
		
	}


	
	void RootSignatureSettingsImpl::RestoreSettings()
	{
		current = saved;
		
	}


	
	RootSignatureSettings &RootSignatureSettingsImpl::AddConstant(const unsigned registerIndex, const unsigned num32BitConstants)
	{
		D3D12_ROOT_PARAMETER1 desc{};

		desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		desc.Constants.ShaderRegister = registerIndex;
		desc.Constants.Num32BitValues = num32BitConstants;

		current.parameters.emplace_back(desc);
		
		return *this;
		
	}


	
	RootSignatureSettings &RootSignatureSettingsImpl::DeclareTable()
	{
		D3D12_ROOT_PARAMETER1 desc{};
		desc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		
		current.parameters.emplace_back(desc);
		current.currentTableParameterOrdinal = current.parameters.size();

		current.tableRanges.emplace_back();
		current.currentTableRangesOrdinal = current.tableRanges.size();

		return *this;
		
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
		desc.OffsetInDescriptorsFromTableStart = current.tableRanges.at(current.currentTableRangesOrdinal-1).currentNumberOfDescriptors;				
		desc.RangeType = GetTargetValue(target);
		desc.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;

		current.tableRanges.at(current.currentTableRangesOrdinal-1).ranges.emplace_back(desc);
		current.tableRanges.at(current.currentTableRangesOrdinal-1).currentNumberOfDescriptors += numDescriptorsInRange;
		
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

		current.tableRanges.emplace_back();
		current.tableRanges.back().ranges.emplace_back(desc);		
		
		D3D12_ROOT_PARAMETER1 paramDesc{};					
		paramDesc.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		paramDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		paramDesc.DescriptorTable.pDescriptorRanges = current.tableRanges.back().ranges.data();
		paramDesc.DescriptorTable.NumDescriptorRanges = current.tableRanges.back().ranges.size();

		current.parameters.emplace_back(paramDesc);

		return *this;
		
	}
	

	
	const std::vector<D3D12_ROOT_PARAMETER1> &RootSignatureSettingsImpl::GetParameters() const
	{
		return current.parameters;	

	}

	
}
