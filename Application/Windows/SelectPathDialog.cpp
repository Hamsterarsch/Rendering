#include "Windows/SelectPathDialog.hpp"
#include <ShObjIdl_core.h>
#include <wrl/client.h>



namespace App::Windows
{
	
	class FileDialogEventHandler final : public IFileDialogEvents
	{
		private: unsigned refcount{ 1 };
		
		public:	HRESULT QueryInterface(const IID& riid, void** ppvObject) override
		{
			if(!ppvObject)
			{
				return E_INVALIDARG;
			}

			*ppvObject = nullptr;
			if(riid == IID_IUnknown || riid == IID_IFileDialogEvents)
			{
				*ppvObject = this;
				AddRef();
				return NOERROR;
			}
			
			return E_NOINTERFACE;
			
		}

		
		ULONG AddRef() override
		{
			InterlockedIncrement(&refcount);
			return refcount;
			
		}
		
		ULONG Release() override
		{
			auto newRefcount{ InterlockedDecrement(&refcount) };
			if(newRefcount == 0)
			{
				delete this;
				
			}
			return newRefcount;
			
		}
		
		HRESULT OnFileOk(IFileDialog* pfd) override { return E_NOTIMPL; }
		HRESULT OnFolderChanging(IFileDialog* pfd, IShellItem* psiFolder) override { return E_NOTIMPL; }
		HRESULT OnFolderChange(IFileDialog* pfd) override { return E_NOTIMPL; }
		HRESULT OnSelectionChange(IFileDialog* pfd) override { return E_NOTIMPL; }
		HRESULT OnShareViolation(IFileDialog* pfd, IShellItem* psi, FDE_SHAREVIOLATION_RESPONSE* pResponse) override { return E_NOTIMPL; }
		HRESULT OnTypeChange(IFileDialog* pfd) override { return E_NOTIMPL; }
		HRESULT OnOverwrite(IFileDialog* pfd, IShellItem* psi, FDE_OVERWRITE_RESPONSE* pResponse) override { return E_NOTIMPL; }
	};

	template<class T>
	HRESULT CreateInstance(REFIID riid, void **ppv)
	{	
		*ppv = nullptr;
		auto *instance{ new(std::nothrow) T() };

		if(instance)
		{
			const auto result{ instance->QueryInterface(riid, ppv) };
			instance->Release();
			return result;			
		}
		return E_OUTOFMEMORY;
		
	}
	
	SelectPathDialog::SelectPathDialog()
	{
		CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		
		Microsoft::WRL::ComPtr<IFileDialog> dialog{};
		auto result
		{
			CoCreateInstance
			(
				CLSID_FileOpenDialog,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&dialog)
			)
		};
				
		if(FAILED(result))
		{
			return;
			
		}

		
		Microsoft::WRL::ComPtr<IFileDialogEvents> eh;
		CreateInstance<FileDialogEventHandler>(IID_PPV_ARGS(&eh));
		
		DWORD cookie;		
		if(FAILED( dialog->Advise(eh.Get(), &cookie) ))
		{
			return;
		}
		
		FILEOPENDIALOGOPTIONS options;
		if(FAILED( dialog->GetOptions(&options) ))
		{
			return;
		}

		if(FAILED( dialog->SetOptions(options | FOS_PICKFOLDERS) ))
		{
			return;
		}

		//COMDLG_FILTERSPEC::
		//dialog->SetFileTypes()
		//set file type index
		//set default extension

		if(FAILED( dialog->Show(nullptr) ))
		{
			return;
		}

		IShellItem *selected;
		if(FAILED( dialog->GetResult(&selected) ))
		{
			return;
		}

		wchar_t *getResult;
		if(FAILED( selected->GetDisplayName(SIGDN_FILESYSPATH, &getResult) ))
		{
			return;
		}

		selectedItem = std::filesystem::path{ getResult };
		CoTaskMemFree(getResult);
		selected->Release();

		dialog->Unadvise(cookie);
		
		CoUninitialize();
		
	}
	
}
