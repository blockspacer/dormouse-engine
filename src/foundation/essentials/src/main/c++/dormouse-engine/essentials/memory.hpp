#ifndef DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_
#define DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_

#include <array>
#include <vector>
#include <string>
#include <cstdint>
#include <type_traits>

namespace dormouse_engine::essentials {

namespace detail {

template <class SourceT, class DestT>
using CopyConstness_t = std::conditional_t<std::is_const_v<SourceT>, std::add_const_t<DestT>, DestT>;

template <class ByteT>
class BufferView {
public:

	using ByteType = ByteT;

	template <class T>
	BufferView(T* elements, size_t elementCount) :
		data_(reinterpret_cast<ByteT*>(elements)),
		size_(elementCount * sizeof(T))
	{
	}

	BufferView(const BufferView<ByteT>&) = default;

	BufferView(BufferView<ByteT>&&) = default;

	template <class OtherT>
	BufferView(const BufferView<OtherT>& other) :
		data_(other.data()),
		size_(other.size())
	{
	}

	ByteType* data() const {
		return data_;
	}

	size_t size() const {
		return size_ * sizeof(ByteType);
	}

private:

	ByteType* const data_;

	const size_t size_;

};

} // namespace detail

using Byte = std::uint8_t;
using ByteVector = std::vector<Byte>;

using BufferView = detail::BufferView<Byte>;
using ConstBufferView = detail::BufferView<const Byte>;

template <class T>
auto viewBuffer(T* data, size_t size) {
	return detail::BufferView<detail::CopyConstness_t<T, Byte>>(data, size);
}

template <class T, size_t S>
auto viewBuffer(T (&a)[S]) {
	return detail::BufferView<detail::CopyConstness_t<T, Byte>>(a, S);
}

template <class T, size_t S>
auto viewBuffer(std::array<T, S>& a) {
	return detail::BufferView<detail::CopyConstness_t<T, Byte>>(a.data(), a.size());
}

template <class T, size_t S>
auto viewBuffer(const std::array<T, S>& a) {
	return detail::BufferView<const Byte>(a.data(), a.size());
}

template <class T>
auto viewBuffer(std::vector<T>& v) {
	return detail::BufferView<Byte>(v.data(), v.size());
}

template <class T>
auto viewBuffer(const std::vector<T>& v) {
	return detail::BufferView<const Byte>(v.data(), v.size());
}

template <class C, class T, class A>
auto viewBuffer(std::basic_string<C, T, A>& s) {
	return detail::BufferView<Byte>(s.data(), s.size());
}

template <class C, class T, class A>
auto viewBuffer(const std::basic_string<C, T, A>& s) {
	return detail::BufferView<const Byte>(s.data(), s.size());
}

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_ */
