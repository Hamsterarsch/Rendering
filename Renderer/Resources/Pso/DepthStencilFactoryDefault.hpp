#pragma once
#include "Resources/Pso/DepthStencilFactory.hpp"



#include <d3d12.h>

namespace Renderer::DX12
{
	class DepthStencilFactoryDefault final : public DepthStencilFactory
	{
		
		private: struct State
		{
			State() : depthComparisonFunction{ D3D12_COMPARISON_FUNC_LESS } {}
			
			D3D12_COMPARISON_FUNC depthComparisonFunction;
						
		}stateCurrent, stateDefault;


		public: DepthStencilFactoryDefault() { RestoreDefaults();}
				
		public: void RestoreDefaults() { stateCurrent = stateDefault;  }

		public: void SaveCurrentStateAsDefault() { stateDefault = stateCurrent; }
		
		public: D3D12_DEPTH_STENCIL_DESC MakeDepthStencilDesc() const override
		{
			D3D12_DEPTH_STENCIL_DESC desc{};

			desc.StencilEnable = false;

			desc.DepthEnable = true;
			desc.DepthFunc = stateCurrent.depthComparisonFunction;
			desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;			
			desc.BackFace = desc.FrontFace = MakeDefaultStencilDesc();			
			
			return desc;
			
		}

		static constexpr D3D12_DEPTH_STENCILOP_DESC MakeDefaultStencilDesc()
		{
			return { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
			
		}

		public: void SetDepthComparisonFunction(D3D12_COMPARISON_FUNC function) { stateCurrent.depthComparisonFunction = function; }
		
	};

	
}


