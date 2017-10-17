#include "Sprite.hpp"

#include <vector>

#pragma warning(push, 3)
#	include <ponder/classbuilder.hpp>
#pragma warning(pop)

#include "dormouse-engine/graphics/Buffer.hpp"
#include "dormouse-engine/graphics/ShaderCompiler.hpp"
#include "dormouse-engine/essentials/policy/creation/None.hpp"
#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/essentials/debug.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/math/Vector.hpp"
#include "../command/CommandBuffer.hpp"
#include "../command/DrawCommand.hpp"
#include "../control/Control.hpp"
#include "../control/Sampler.hpp"
#include "../control/RenderState.hpp"
#include "../shader/Technique.hpp"
#include "../shader/MergedProperty.hpp"
#include "../shader/NamedProperty.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::d2;

namespace /* anonymous */ {

class SpriteCommon final :
	public essentials::Singleton<SpriteCommon, essentials::policy::creation::None<SpriteCommon>>
{
public:

	SpriteCommon(graphics::Device& graphicsDevice, essentials::ConstBufferView shaderCode) :
		vertexBuffer_(createVertexBuffer_(graphicsDevice)),
		technique_(createTechnique_(graphicsDevice, std::move(shaderCode))),
		sampler_(graphicsDevice, control::Sampler::CLAMPED_LINEAR),
		renderState_(graphicsDevice, control::RenderState::OPAQUE)
	{
	}

	void render(
		command::DrawCommand& cmd,
		const Sprite& sprite,
		const shader::Property& properties,
		const Control& renderControl
		) const
	{
		cmd.setRenderControl(renderControl);

		auto spriteProperty = shader::Property(reflection::Object(essentials::make_observer(&sprite)));
		auto spriteEntry = shader::Property(shader::NamedProperty("sprite", essentials::make_observer(&spriteProperty)));

		auto mergedProperty = shader::MergedProperty(
			essentials::make_observer(&spriteEntry),
			essentials::make_observer(&properties)
			);

		technique_.render(cmd, mergedProperty);

		cmd.setVertexBuffer(vertexBuffer_, 4u, 2 * sizeof(math::Vec2));
		cmd.setPrimitiveTopology(graphics::PrimitiveTopology::TRIANGLE_STRIP);
		cmd.setTechnique(essentials::make_observer(&technique_));
	}

	const control::Sampler& sampler() const {
		return sampler_;
	}

private:

	const graphics::Buffer vertexBuffer_;

	const shader::Technique technique_;

	const control::Sampler sampler_;

	const control::RenderState renderState_;

	static graphics::Buffer createVertexBuffer_(graphics::Device& graphicsDevice) {
		auto configuration = graphics::Buffer::Configuration();

		auto initialData = std::vector<math::Vec2> {
			{ -1.0f, +1.0f }, { 0.0f, 0.0f }, // pos, texcoord
			{ +1.0f, +1.0f }, { 1.0f, 0.0f },
			{ -1.0f, -1.0f }, { 0.0f, 1.0f },
			{ +1.0f, -1.0f }, { 1.0f, 1.0f }
			};

		configuration.allowCPURead = false;
		configuration.allowGPUWrite = false;
		configuration.allowModifications = false;
		configuration.purpose = graphics::Buffer::CreationPurpose::VERTEX_BUFFER;
		configuration.size = initialData.size() * sizeof(initialData.front());

		return graphics::Buffer(graphicsDevice, std::move(configuration), essentials::viewBuffer(initialData));
	}

	static shader::Technique createTechnique_(
		graphics::Device& graphicsDevice, essentials::ConstBufferView shaderCode)
	{
		auto technique = shader::Technique();

		auto flags = graphics::ShaderCompiler::CompilerFlags();

		if (essentials::IS_DEBUG) {
			flags = graphics::ShaderCompiler::FULL_DEBUG_MASK;
		}

		auto shaderCompiler = graphics::ShaderCompiler(flags);

		{
			auto compiledVertexShader =
				shaderCompiler.compile(shaderCode, "sprite", "vs", graphics::ShaderType::VERTEX);
			technique.setShader(
				shader::VertexShader(graphicsDevice, essentials::viewBuffer(compiledVertexShader)));

			auto inputLayout = shader::InputLayout(graphicsDevice, essentials::viewBuffer(compiledVertexShader));
			technique.setInputLayout(std::move(inputLayout));
		}

		{
			auto compiledPixelShader =
				shaderCompiler.compile(shaderCode, "sprite", "ps", graphics::ShaderType::PIXEL);
			technique.setShader(
				shader::PixelShader(graphicsDevice, essentials::viewBuffer(compiledPixelShader)));
		}

		return technique;
	}

};

} // anonymous namespace

void Sprite::initialiseSystem(graphics::Device& device, essentials::ConstBufferView shaderCode) {
	SpriteCommon::setInstance(std::make_unique<SpriteCommon>(device, std::move(shaderCode)));
}

void Sprite::render(
	command::CommandBuffer& commandBuffer,
	const shader::Property& properties,
	const Control& renderControl
	) const
{
	// TODO: 0 will not work if render called multiple times per frame
	auto& cmd = commandBuffer.create(command::CommandBuffer::CommandId{ this, 0 });
	SpriteCommon::instance()->render(cmd, *this, properties, renderControl);
}

control::Sampler Sprite::sampler() const noexcept {
	return SpriteCommon::instance()->sampler();
}

void detail::declareSprite() {
	const Layout& (Sprite::*constLayout)() const = &Sprite::layout;

	ponder::Class::declare<Sprite>("dormouse_engine::renderer::d2::Sprite")
		.property("texture", &Sprite::texture)
		.property("sampler", &Sprite::sampler)
		.property("layout", constLayout)
		;
}
