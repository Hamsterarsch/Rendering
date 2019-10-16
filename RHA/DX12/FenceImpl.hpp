#pragma once
#include "DX12/Fence.hpp"
#include "DX12/DeviceResources.hpp"



namespace RHA
{
	namespace DX12
	{
		class FenceImpl : public Fence
		{
			private: DxPtr<ID3D12Fence> fence;



			public: FenceImpl(DeviceResources *resource);

				private: static void CheckFenceCreation(HRESULT result);

			
			public: inline virtual size_t GetValue() const override { return fence->GetCompletedValue(); }
			
			public: virtual void Signal(size_t value) override;

			public: virtual void Signal(size_t value, class Queue *queue) override;
								
		};
		
		
	}
	
	
}
