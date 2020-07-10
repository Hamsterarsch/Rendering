#pragma once
#include "Asset.hpp"
#include <string>
#include <vector>
#include "Resources/SerializeTarget.hpp"
#include "RendererFacade.hpp"


namespace Renderer { class RendererFacade; }


namespace App::Assets
{
	class ConstantsResourceSlot final : public assetSystem::Archivable
	{
		public: unsigned shaderRegister;
		
		public: unsigned sizeInVector4;

		

		public: ConstantsResourceSlot() : ConstantsResourceSlot{ 0, 0 } {}
		
		public: ConstantsResourceSlot(unsigned shaderRegister, unsigned sizeInVector4) : shaderRegister{ shaderRegister }, sizeInVector4{ sizeInVector4 } {}
		
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
		
	};


	
	
	class TextureResourceSlot final : public assetSystem::Archivable
	{
		public: unsigned shaderRegister;

		

		public: TextureResourceSlot() : TextureResourceSlot{ 0 } {}
		
		public: TextureResourceSlot(unsigned shaderRegister) : shaderRegister{ shaderRegister } {}
		
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;
				
	};


		
	class ShaderAsset : public assetSystem::Asset
	{				
		private: std::string shaderCode;

		private: std::vector<ConstantsResourceSlot> constantsSlots;

		private: std::vector<TextureResourceSlot> textureSlots;
				 
		private: Renderer::SerializeTarget compiledCode;

		
		
		public: ShaderAsset() = default;

		public: ShaderAsset(const char *code) : shaderCode{ code } {}


		public: void Compile(Renderer::RendererFacade &renderer);
		
			private: virtual void CompileInternal(Renderer::RendererFacade &renderer, Renderer::SerializeTarget &outCode) = 0;
		
		public: assetSystem::io::Archive &Serialize(assetSystem::io::Archive &archive) override;

			private: void SerializeResourceSlots(assetSystem::io::Archive &archive);
				
		public: void SetCode(const std::string &code) { shaderCode = code; }
		
		public: const char *GetCode() const { return shaderCode.c_str(); }

		public: size_t GetCodeLength() const { return shaderCode.size(); }

		
		public: const unsigned char *GetCompiledCode() const { return compiledCode.GetData(); }

		public: size_t GetCompiledCodeSizeInBytes() const { return compiledCode.GetSizeInBytes(); }


		public: void ClearResourceSlots() { constantsSlots.clear(); textureSlots.clear(); }
		
		public: void AddConstantsSlot(const ConstantsResourceSlot &slot) { constantsSlots.emplace_back(slot); }

		public: size_t GetNumConstantsSlots() const { return constantsSlots.size(); }

		public: ConstantsResourceSlot GetConstantsSlotAt(const size_t index) const { return constantsSlots.at(index); }
		
		public: void AddTextureSlot(const TextureResourceSlot &slot) { textureSlots.emplace_back(slot); }

		public: size_t GetNumTextureSlots() const { return textureSlots.size(); }

		public: TextureResourceSlot GetTextureSlotAt(const size_t index) const { return textureSlots.at(index); }

		
		public: static const char *GetAssetClassExtension();
						

	};

	class PixelShaderAsset final : public ShaderAsset
	{
		public: static const char *GetAssetClassExtension()
		{
			static const std::string extension{ std::string{"ps."} + ShaderAsset::GetAssetClassExtension() };

			return extension.c_str();
			
		}

		private: void CompileInternal(Renderer::RendererFacade &renderer, Renderer::SerializeTarget &outCode) override
		{
			renderer.CompilePixelShader(GetCode(), GetCodeLength(), outCode);
			
		}
		
	};

	class VertexShaderAsset final : public ShaderAsset
	{
		public: static const char *GetAssetClassExtension()
		{
			static const std::string extension{ std::string{"vs."} + ShaderAsset::GetAssetClassExtension() };

			return extension.c_str();
			
		}

		private: void CompileInternal(Renderer::RendererFacade &renderer, Renderer::SerializeTarget &outCode) override
		{
			renderer.CompileVertexShader(GetCode(), GetCodeLength(), outCode);
			
		}
		
	};

	class ComputeShaderAsset final : public ShaderAsset
	{
		public: static const char *GetAssetClassExtension()
		{
			static const std::string extension{ std::string{"cs."} + ShaderAsset::GetAssetClassExtension() };

			return extension.c_str();
			
		}

		private: void CompileInternal(Renderer::RendererFacade &renderer, Renderer::SerializeTarget &outCode) override
		{
			renderer.CompileComputeShader(GetCode(), GetCodeLength(), outCode);
			
		}
		
	};
	
	
}
