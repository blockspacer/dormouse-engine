#ifndef _DORMOUSEENGINE_GRAPHICS_SHADER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_SHADER_HPP_

#include <memory>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/ShaderType.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "detail/detailfwd.hpp"

namespace dormouse_engine::graphics {

class Device;

namespace detail {

template <class InternalShaderType>
class Shader {
public:

	static const auto SHADER_TYPE = shaderTypeFromShader<InternalShaderType>();

	Shader() = default;

	Shader(Device& device, essentials::ConstBufferView shaderData);

private:

	system::windows::COMWrapper<InternalShaderType> shader_;

	friend struct detail::Internals;

};

} // namespace detail

using VertexShader = detail::Shader<ID3D11VertexShader>;
using GeometryShader = detail::Shader<ID3D11GeometryShader>;
using HullShader = detail::Shader<ID3D11HullShader>;
using DomainShader = detail::Shader<ID3D11DomainShader>;
using PixelShader = detail::Shader<ID3D11PixelShader>;
using ComputeShader = detail::Shader<ID3D11ComputeShader>;

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_SHADER_HPP_ */
