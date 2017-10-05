#ifndef _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_
#define _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_

#pragma warning(push, 3)
#	include <ponder/pondertype.hpp>
#pragma warning(pop)

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/math/Transform.hpp"
#include "../control/ResourceView.hpp"
#include "../control/Viewport.hpp"
#include "../control/RenderTargetView.hpp"
#include "../control/DepthStencilView.hpp"
#include "../command/CommandBuffer.hpp"
#include "../command/DrawCommand.hpp"
#include "../shader/Property.hpp"
#include "Layout.hpp"

namespace dormouse_engine::renderer::d2 {

class Sprite final {
public:

	// TODO: temp - don't pass shader code here
	static void initialiseSystem(graphics::Device& device, essentials::ConstBufferView shaderCode);

	Sprite(const graphics::Texture& texture) :
		textureView_(texture)
	{
	}

	void render(
		command::CommandBuffer& commandBuffer,
		const shader::Property& properties,
		control::Viewport viewport, // TODO: probably shouldn't be passed to render like that?
		control::RenderTargetView renderTarget,
		control::DepthStencilView depthStencil
		) const;

	control::ResourceView texture() const noexcept {
		return textureView_;
	}

	control::Sampler sampler() const noexcept;

	const Layout& layout() const noexcept {
		return layout_;
	}

	Layout& layout() noexcept {
		return layout_;
	}

private:

	mutable command::DrawCommand cmd_;

	control::ResourceView textureView_;

	Layout layout_;

};

namespace detail { void declareSprite(); }

} // namespace dormouse_engine::renderer::d2

PONDER_AUTO_TYPE(dormouse_engine::renderer::d2::Sprite, &dormouse_engine::renderer::d2::detail::declareSprite);

#endif /* _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_ */
