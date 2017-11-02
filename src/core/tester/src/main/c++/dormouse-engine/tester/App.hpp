#ifndef _DORMOUSEENGINE_TESTER_APP_HPP_
#define _DORMOUSEENGINE_TESTER_APP_HPP_

#include "dormouse-engine/essentials/policy/creation/New.hpp"
#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/engine/app/App.hpp"
#include "TextureComparator.hpp"

namespace dormouse_engine::tester {

class App {
public:

	App();

	void frame();

	void compareWithReferenceScreen(size_t index);

	engine::App& engineApp() noexcept {
		return engineApp_;
	}

	graphics::Device& graphicsDevice() noexcept {
		return engineApp_.graphicsDevice();
	}

	renderer::control::Viewport fullscreenViewport() const noexcept {
		return fullscreenViewport_;
	}

private:

	engine::App engineApp_;

	renderer::control::Viewport fullscreenViewport_; // TODO: this needs to be easily accessible from somewhere else

	TextureComparator textureComparator_;

	graphics::Texture screenshotGPU_;

	graphics::Texture screenshotCPU_;

};

class GlobalApp :
	public essentials::Singleton<
		GlobalApp,
		essentials::policy::creation::New<GlobalApp>
		>,
	public App
{
};

} // namespace dormouse_engine::tester

#endif /* _DORMOUSEENGINE_TESTER_APP_HPP_ */
