#include "graphics.pch.hpp"

#include "Texture.hpp"

#include "dormouse-engine/essentials/Range.hpp"
#include "detail/Internals.hpp"
#include "Device.hpp"
#include "Image.hpp"
#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */ {

template <class DescType>
void setupDescCommon(DescType& desc, const Texture::Configuration1d& configuration) {
	desc.Width = static_cast<UINT>(configuration.width);
	desc.MipLevels = static_cast<UINT>(configuration.mipLevels);
	desc.ArraySize = static_cast<UINT>(configuration.arraySize);
	desc.Format = static_cast<DXGI_FORMAT>(configuration.pixelFormat.id());
	desc.BindFlags = configuration.purposeFlags.integralValue();
	desc.MiscFlags = (configuration.arraySize == 6) ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0u;

	if (configuration.allowModifications) {
		if (configuration.allowCPURead) {
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		} else {
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
	} else {
		if (configuration.allowCPURead) {
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		} else {
			if (configuration.allowGPUWrite) {
				desc.Usage = D3D11_USAGE_DEFAULT;
			} else {
				desc.Usage = D3D11_USAGE_IMMUTABLE;
			}
		}
	}
}

std::vector<D3D11_SUBRESOURCE_DATA> createSubresourceData(
	essentials::ConstBufferView initialData,
	const Texture::Configuration1d& configuration,
	size_t textureHeight
	)
{
	auto result = std::vector<D3D11_SUBRESOURCE_DATA>();

	if (initialData.data()) {
		result.resize(configuration.arraySize * configuration.mipLevels);
		std::memset(result.data(), 0, result.size() * sizeof(D3D11_SUBRESOURCE_DATA));

		const auto pixelSize = configuration.pixelFormat.pixelSize();
		const auto* data = initialData.data();

		for (const auto textureIndex : range<size_t>(0u, configuration.arraySize)) {
			for (const auto mipIndex : range<size_t>(0u, configuration.mipLevels)) {
				const auto subresourceIndex = textureIndex * configuration.mipLevels + mipIndex;
				const auto textureWidth = configuration.width >> mipIndex;
				const auto rowPitch = configuration.pixelFormat.rowPitch(textureWidth);
				const auto slicePitch = configuration.pixelFormat.slicePitch(textureHeight, rowPitch);

				result[subresourceIndex].pSysMem = data;
				result[subresourceIndex].SysMemPitch = static_cast<UINT>(rowPitch);
				result[subresourceIndex].SysMemSlicePitch = static_cast<UINT>(slicePitch);

				data += slicePitch;
			}
		}
	}

	return result;
}

system::windows::COMWrapper<ID3D11Resource> createTexture1dResource(
	Device& device,
	const Texture::Configuration1d& configuration,
	essentials::ConstBufferView initialData
	)
{
	auto desc = D3D11_TEXTURE1D_DESC();
	std::memset(&desc, 0, sizeof(desc));

	setupDescCommon(desc, configuration);

	auto* subresourceDataPtr = static_cast<D3D11_SUBRESOURCE_DATA*>(nullptr);

	auto subresourceData = createSubresourceData(initialData, configuration, 1u);
	if (!subresourceData.empty()) {
		subresourceDataPtr = subresourceData.data();
	}

	auto texture = system::windows::COMWrapper<ID3D11Texture1D>();
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateTexture1D(&desc, subresourceDataPtr, &texture.get()),
		"Failed to create a 1D texture"
		);

	return texture;
}

system::windows::COMWrapper<ID3D11Resource> createTexture2dResource(
	Device& device,
	const Texture::Configuration2d& configuration,
	essentials::ConstBufferView initialData
	)
{
	auto desc = D3D11_TEXTURE2D_DESC();
	std::memset(&desc, 0, sizeof(desc));

	setupDescCommon(desc, configuration);

	desc.Height = static_cast<UINT>(configuration.height);
	desc.SampleDesc.Count = static_cast<UINT>(configuration.sampleCount);
	desc.SampleDesc.Quality = static_cast<UINT>(configuration.sampleQuality);

	auto* subresourceDataPtr = static_cast<D3D11_SUBRESOURCE_DATA*>(nullptr);

	auto subresourceData = createSubresourceData(initialData, configuration, configuration.height);
	if (!subresourceData.empty()) {
		subresourceDataPtr = subresourceData.data();
	}

	auto texture = system::windows::COMWrapper<ID3D11Texture2D>();
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateTexture2D(&desc, subresourceDataPtr, &texture.get()),
		"Failed to create a 2D texture"
	);

	return texture;
}

system::windows::COMWrapper<ID3D11Resource> createImageResource(Device& device, const Image& image) {
	auto configuration = Texture::Configuration2d();
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.arraySize = image.arraySize();
	configuration.width = image.size().first;
	configuration.height = image.size().second;
	configuration.mipLevels = image.mipLevels();
	configuration.pixelFormat = image.pixelFormat();
	configuration.purposeFlags = Texture::CreationPurpose::SHADER_RESOURCE;
	
	return createTexture2dResource(device, configuration, image.pixels());
}

} // anonymous namespace

Texture::Texture(
	Device& device,
	const Configuration1d& configuration,
	essentials::ConstBufferView initialData
	) :
	Resource(createTexture1dResource(device, configuration, initialData))
{
}

Texture::Texture(
	Device& device,
	const Configuration2d& configuration,
	essentials::ConstBufferView initialData
	) :
	Resource(createTexture2dResource(device, configuration, initialData))
{
}

Texture::Texture(
	Device& device,
	const Image& image
	) :
	Resource(createImageResource(device, image))
{
}

graphics::PixelFormat Texture::pixelFormat() const {
	auto dimension = D3D11_RESOURCE_DIMENSION();

	auto& dxResource = *detail::Internals::dxResourcePtr(*this);
	dxResource.GetType(&dimension);

	switch (dimension) {
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D: {
			auto desc = D3D11_TEXTURE1D_DESC();
			static_cast<ID3D11Texture1D&>(dxResource).GetDesc(&desc);
			return PixelFormat(static_cast<PixelFormatId>(desc.Format));
		}
		break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D: {
			auto desc = D3D11_TEXTURE2D_DESC();
			static_cast<ID3D11Texture2D&>(dxResource).GetDesc(&desc);
			return PixelFormat(static_cast<PixelFormatId>(desc.Format));
		}
		break;
	default:
		throw exceptions::LogicError("Invalid resource dimension");
	}
}

RenderTargetView::RenderTargetView(const Texture& texture) {
	checkDirectXCall(
		detail::Internals::dxDevice(texture).CreateRenderTargetView(
			detail::Internals::dxResourcePtr(texture), nullptr, &renderTargetView_.get()),
		"Failed to create a render target view of texture"
		);
}

Resource::Id RenderTargetView::resourceId() const {
	auto* dxResource = static_cast<ID3D11Resource*>(nullptr);
	renderTargetView_->GetResource(&dxResource);
	return reinterpret_cast<std::uintptr_t>(dxResource);
}

DepthStencilView::DepthStencilView(const Texture& texture) {
	checkDirectXCall(
		detail::Internals::dxDevice(texture).CreateDepthStencilView(
			detail::Internals::dxResourcePtr(texture), nullptr, &depthStencilView_.get()),
		"Failed to create a depth stencil view of texture"
		);
}

Resource::Id DepthStencilView::resourceId() const {
	auto* dxResource = static_cast<ID3D11Resource*>(nullptr);
	depthStencilView_->GetResource(&dxResource);
	return reinterpret_cast<std::uintptr_t>(dxResource);
}
