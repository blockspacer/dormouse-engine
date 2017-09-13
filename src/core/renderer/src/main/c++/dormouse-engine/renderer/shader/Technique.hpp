#ifndef _DORMOUSEENGINE_RENDERER_SHADER_TECHNIQUE_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_TECHNIQUE_HPP_

#include <array>

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/graphics/InputLayout.hpp"
#include "../command/commandfwd.hpp"
#include "Shader.hpp"

namespace dormouse_engine::renderer::shader {

class Property;

class Technique {
public:

	Technique() = default;

	void bind(graphics::CommandList& commandList) const;

	void render(command::DrawCommand& cmd, const Property& root) const;

	void setInputLayout(graphics::InputLayout inputLayout) {
		inputLayout_ = std::move(inputLayout);
	}

	void setShader(VertexShader shader) {
		vertexShader_ = std::move(shader);
	}

	void setShader(GeometryShader shader) {
		geometryShader_ = std::move(shader);
	}

	void setShader(DomainShader shader) {
		domainShader_ = std::move(shader);
	}

	void setShader(HullShader shader) {
		hullShader_ = std::move(shader);
	}

	void setShader(PixelShader shader) {
		pixelShader_ = std::move(shader);
	}

private:

	graphics::InputLayout inputLayout_;

	VertexShader vertexShader_;

	GeometryShader geometryShader_;

	DomainShader domainShader_;

	HullShader hullShader_;

	PixelShader pixelShader_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_TECHNIQUE_HPP_ */
