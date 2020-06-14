#include "Windows/SelectPathDialog.hpp"
#include <ShObjIdl_core.h>
#include <wrl/client.h>

#include <vector>
#include <string>

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

	


	SelectPathDialog::SelectPathDialog() : SelectPathDialog(nullptr, 0)		
	{
	}
	
		SelectPathDialog::SelectPathDialog(const wchar_t **allowedFileExtensions, const unsigned numExtensions)
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
			

			if(numExtensions > 0)
			{
				COMDLG_FILTERSPEC filter{};
				filter.pszName = L"";

				std::wstring filterPattern{};
				for(auto extensionIndex{ 0 }; extensionIndex < numExtensions; ++extensionIndex)
				{
					((filterPattern += L"*.") += allowedFileExtensions[extensionIndex]) += L";";
				}
				filter.pszSpec = filterPattern.c_str();
				
				dialog->SetFileTypes(1, &filter);

				if(FAILED(dialog->SetOptions(options | FOS_FILEMUSTEXIST)))
				{
					return;					
				}
			}
			else
			{
				if(FAILED( dialog->SetOptions(options | FOS_PICKFOLDERS) ))
				{
					return;
				}
			}

			if(FAILED( dialog->Show(nullptr) ))
			{
				return;
			}

			IShellItem *selected{ nullptr };
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
