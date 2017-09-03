#ifndef _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_

#include <vector>

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/graphics/Shader.hpp"
#include "../command/commandfwd.hpp"
#include "Properties.hpp"
#include "PropertyId.hpp"

namespace dormouse_engine::renderer::shader {

namespace detail {

class ShaderBase {
protected:

	void doRender(command::DrawCommand& cmd, const Properties& properties, graphics::ShaderType shaderType) const;

private:

	struct Resource {

		PropertyDescriptor descriptor;

		size_t slot;

	};

	using Resources = std::vector<Resource>;

	Resources resources_;

};

template <class GraphicsShaderType>
class Shader : public ShaderBase {
public:

	void bind(graphics::CommandList& commandList) const {
		commandList.setShader(shader_);
	}

	void render(command::DrawCommand& cmd, const Properties& properties) const {
		doRender(cmd, properties, GraphicsShaderType::SHADER_TYPE);
	}

private:

	GraphicsShaderType shader_;

};

} // namespace detail

using VertexShader = detail::Shader<graphics::VertexShader>;
using GeometryShader = detail::Shader<graphics::GeometryShader>;
using HullShader = detail::Shader<graphics::HullShader>;
using DomainShader = detail::Shader<graphics::DomainShader>;
using PixelShader = detail::Shader<graphics::PixelShader>;

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_ */
