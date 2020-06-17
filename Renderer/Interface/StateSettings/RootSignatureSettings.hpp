#pragma once
#include "StateSettings/StateSettings.hpp"
#include "DescriptorTargets.hpp"


namespace Renderer
{	
	class RootSignatureSettings : public StateSettings
	{
		public: virtual RootSignatureSettings &AddConstant(unsigned registerIndex, unsigned num32BitConstants) = 0;

		public: virtual RootSignatureSettings &DeclareDataVolatility() = 0;

		public: virtual RootSignatureSettings &DeclareDescriptorVolatility() = 0;
		
		public: virtual RootSignatureSettings &AddDescriptor(DescriptorTarget target, unsigned registerIndex) = 0;
		
		public: virtual RootSignatureSettings &DeclareTable() = 0;

		public: virtual RootSignatureSettings &AddTableRange(DescriptorTarget target, unsigned baseRegister, unsigned numDescriptorsInRange) = 0;

		public: virtual RootSignatureSettings &AddSamplerTable(unsigned baseRegister, unsigned numSamplers) = 0;
							   				 						
	};
	
	
}
