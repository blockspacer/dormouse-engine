#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_SAMPLER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_SAMPLER_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/enums.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"

namespace dormouse_engine {
namespace graphics {

class Renderer;

class Sampler {
public:

	DE_MEMBER_ENUM_VALUES(
		AddressMode,
		(CLAMP)(D3D11_TEXTURE_ADDRESS_CLAMP)
		(WRAP)(D3D11_TEXTURE_ADDRESS_WRAP)
		(MIRROR)(D3D11_TEXTURE_ADDRESS_MIRROR)
		(MIRROR_ONCE)(D3D11_TEXTURE_ADDRESS_MIRROR_ONCE)
		);

	DE_MEMBER_ENUM_VALUES(
		Filter,
		(MIN_MAG_MIP_POINT)(D3D11_FILTER_MIN_MAG_MIP_POINT)
		(MIN_MAG_MIP_LINEAR)(D3D11_FILTER_MIN_MAG_MIP_LINEAR)
		(MIN_MAG_LINEAR_MIP_POINT)(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT)
		(ANISOTROPIC)(D3D11_FILTER_ANISOTROPIC)
		);

	struct Configuration {

		AddressMode addressModeU;

		AddressMode addressModeV;

		AddressMode addressModeW;

		Filter filter;

	};

	Sampler() {
	}

	Sampler(Renderer& renderer, const Configuration& configuration);

	ID3D11SamplerState& internalSamplerState() {
		return *samplerState_;
	}

private:

	system::windows::COMWrapper<ID3D11SamplerState> samplerState_;

};

} // namespace graphics
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_SAMPLER_HPP_ */
