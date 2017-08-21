#ifndef _DORMOUSEENGINE_GRAPHICS_INDEXBUFFER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_INDEXBUFFER_HPP_

#include <cassert>
#include <cstdint>

#include "Buffer.hpp"
#include "PixelFormat.hpp"

namespace dormouse_engine::graphics {

class IndexBuffer : public Buffer {
public:

	template <class IntegralType>
	static IntegralType degenerateIndex();

	IndexBuffer() = default;

	IndexBuffer(Device& renderer, const Configuration& configuration, const void* initialData = 0) :
		Buffer(renderer, CreationPurpose::INDEX_BUFFER, configuration, initialData),
		pixelFormat_(configuration.stride == 2 ? FORMAT_R16_UINT : FORMAT_R32_UINT)
	{
		assert(configuration.stride == 2 || configuration.stride == 4);
	}

	PixelFormat pixelFormat() const {
		return pixelFormat_;
	}

private:

	PixelFormat pixelFormat_;

};

template <>
inline std::uint16_t IndexBuffer::degenerateIndex<std::uint16_t>() {
	return static_cast<std::uint16_t>(-1);
}

template <>
inline std::uint32_t IndexBuffer::degenerateIndex<std::uint32_t>() {
	return static_cast<std::uint32_t>(-1);
}

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_INDEXBUFFER_HPP_ */
