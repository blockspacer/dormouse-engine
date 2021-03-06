#ifndef _DORMOUSEENGINE_RENDERER_CONTROL_DEPTHSTENCILVIEW_HPP_
#define _DORMOUSEENGINE_RENDERER_CONTROL_DEPTHSTENCILVIEW_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/graphics/CommandList.hpp"

namespace dormouse_engine::renderer::control {

class DepthStencilView final {
public:

	static void initialiseSystem(graphics::Device& graphicsDevice);

	DepthStencilView() = default;

	DepthStencilView(const graphics::Texture& texture);

	graphics::DepthStencilView get() const;

private:

	static constexpr size_t INVALID_DEPTHSTENCIL_VIEW_ID = static_cast<size_t>(-1);

	size_t depthStencilViewId_ = INVALID_DEPTHSTENCIL_VIEW_ID;

	friend bool operator==(const DepthStencilView& lhs, const DepthStencilView& rhs) {
		return lhs.depthStencilViewId_ == rhs.depthStencilViewId_;
	}

	friend bool operator!=(const DepthStencilView& lhs, const DepthStencilView& rhs) {
		return !(lhs == rhs);
	}

};

} // namespace dormouse_engine::renderer::control

#endif /* _DORMOUSEENGINE_RENDERER_CONTROL_DEPTHSTENCILVIEW_HPP_ */
