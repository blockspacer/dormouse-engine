#ifndef _DORMOUSEENGINE_MATH_PLANE_HPP_
#define _DORMOUSEENGINE_MATH_PLANE_HPP_

#include "Vector.hpp"
#include "ScalarEqual.hpp"

namespace dormouse_engine::math {

class Plane {
public:

	Plane() = default;

	Plane(Vec3 normal, const Vec3& point) noexcept {
		normal.normalise();
		equation_ = Vec4(normal, -normal.dot(point));
	}

	Plane(const Vec3& normal, float distanceFromOrigin) noexcept :
		equation_(normal.normalised(), distanceFromOrigin)
	{
	}

	explicit Plane(Vec4 equation) noexcept :
		equation_(std::move(equation))
	{
		equation_ /= equation_.xyz().length();
	}

	const Vec3 normal() const noexcept {
		return equation_.xyz();
	}

	const float signedDistanceToOrigin() const noexcept {
		return equation_.w();
	}

	const float signedDistanceToPoint(const Vec3& point) const noexcept {
		return equation_.dot(Vec4(point, 1.0f));
	}

private:

	Vec4 equation_; // <Nx, Ny, Nz, D>

};

} // namespace math

namespace dormouse_engine {

using math::Plane;

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_MATH_PLANE_HPP_ */
