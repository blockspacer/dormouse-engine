#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/graphics/Image.hpp"
#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/tester/RenderingFixture.hpp"
#include "dormouse-engine/renderer/d2/Sprite.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::d2;

using namespace std::string_literals;

namespace /* anonymous */ {

BOOST_FIXTURE_TEST_SUITE(RendererSpriteTestSuite, tester::RenderingFixture);

BOOST_AUTO_TEST_CASE(RendersSprites) {
	auto texturePath = "test/renderer/sprite-texture.png"s;
	auto textureData = essentials::test_utils::readBinaryFile(texturePath);
	auto textureImage = graphics::Image::load(essentials::viewBuffer(textureData), texturePath);
	auto texture = graphics::Texture(graphicsDevice(), textureImage);
	auto sprite = Sprite(texture);

	sprite.layout().width() = Ndc(0.5f);
	sprite.layout().height() = RatioKeeping(essentials::make_observer(&sprite.layout().width()), 0.5f);

	sprite.layout().anchor() = Layout::Anchor(Layout::HorizontalAnchor::LEFT, Layout::VerticalAnchor::BOTTOM);
	sprite.layout().positionX() = WindowRelative(0.0f);
	sprite.layout().positionY() = WindowRelative(0.0f);

	for (;;) {
		graphicsDevice().beginScene();

		auto commandBuffer = command::CommandBuffer();
		sprite.render(
			commandBuffer,
			shader::Property(),
			fullscreenViewport(),
			graphicsDevice().backBuffer(),
			graphicsDevice().depthStencil()
			);

		commandBuffer.submit(graphicsDevice().getImmediateCommandList());

		graphicsDevice().endScene();
	}
}

BOOST_AUTO_TEST_SUITE_END(/* RendererSpriteTestSuite */);

} // anonymous namespace
