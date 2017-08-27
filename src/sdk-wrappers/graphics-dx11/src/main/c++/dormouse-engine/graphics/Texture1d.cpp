#include "graphics.pch.hpp"

#include "Texture1d.hpp"

#include <cstring>

#include "dormouse-engine/essentials/Range.hpp"
#include "Device.hpp"
#include "DirectXError.hpp"
#include "Image.hpp" // TODO: will only need Image when it gets extracted

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

Texture1d::Texture1d(Device& renderer, const Configuration& configuration) {
	initialise(renderer, configuration);
}

Texture1d::Texture1d(Device& renderer, const Image& image) {
	Configuration config;
	config.width = image.size().first;
	config.arraySize = image.arraySize();
	config.mipLevels = image.mipLevels();
	config.pixelFormat = image.pixelFormat();
	config.allowModifications = false;
	config.allowCPURead = false;
	config.allowGPUWrite = false;
	config.purposeFlags = CreationPurpose::SHADER_RESOURCE;
	config.initialData = image.pixels();

	initialise(renderer, config);
}

void Texture1d::initialise(Device& renderer, const Configuration& configuration) {
	reset();

	D3D11_TEXTURE1D_DESC desc;
	std::memset(&desc, 0, sizeof(desc));

	// TODO: extract common configuration elements to superclass (when implementing other texture types)
	desc.Width = static_cast<UINT>(configuration.width);
	desc.MipLevels = static_cast<UINT>(configuration.mipLevels);
	desc.ArraySize = static_cast<UINT>(configuration.arraySize);
	desc.Format = static_cast<DXGI_FORMAT>(configuration.pixelFormat.id());
	desc.BindFlags = configuration.purposeFlags.integralValue();
	desc.MiscFlags = configuration.arraySize == 6 ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0u; // TODO

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

	std::vector<D3D11_SUBRESOURCE_DATA> subresourceData;
	D3D11_SUBRESOURCE_DATA* subresourceDataPtr = nullptr;

	if (configuration.initialData) {
		subresourceData.resize(configuration.arraySize * configuration.mipLevels);
		std::memset(subresourceData.data(), 0, subresourceData.size() * sizeof(D3D11_SUBRESOURCE_DATA));

		const auto pixelSize = configuration.pixelFormat.pixelSize();
		const auto* data = reinterpret_cast<const std::uint8_t*>(configuration.initialData);

		for (const auto textureIndex : dormouse_engine::range(size_t(0), configuration.arraySize)) {
			for (const auto mipIndex : dormouse_engine::range(size_t(0), configuration.mipLevels)) {
				const auto subresourceIndex = textureIndex * configuration.mipLevels + mipIndex;
				const auto textureWidth = configuration.width >> mipIndex;
				const auto rowPitch = configuration.pixelFormat.rowPitch(textureWidth);
				const auto slicePitch = configuration.pixelFormat.slicePitch(1u, rowPitch);

				subresourceData[subresourceIndex].pSysMem = data;
				// TODO: this will not work for compressed formats
				subresourceData[subresourceIndex].SysMemPitch = static_cast<UINT>(rowPitch);
				subresourceData[subresourceIndex].SysMemSlicePitch = static_cast<UINT>(slicePitch);

				data += slicePitch;
			}
		}

		subresourceDataPtr = subresourceData.data();
	}

	auto texture = system::windows::COMWrapper<ID3D11Texture1D>();
	checkDirectXCall(
		renderer.internalDevice().CreateTexture1D(&desc, subresourceDataPtr, &texture.get()),
		"Failed to create a 1D texture"
	);
	resource_ = std::move(texture);

	Texture::initialise(renderer, configuration.purposeFlags);
}

void Texture1d::initialise(
	Device& renderer,
	dormouse_engine::Mask<CreationPurpose> purposeFlags,
	system::windows::COMWrapper<ID3D11Texture1D> texture
)
{
	reset();

	resource_ = std::move(texture);

	Texture::initialise(renderer, purposeFlags);
}
