#ifndef _DORMOUSEENGINE_RENDERER_SHADER_NAMEDPROPERTY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_NAMEDPROPERTY_HPP_

#include <cassert>

#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "Property.hpp"

namespace dormouse_engine::renderer::shader {

class NamedProperty {
public:

	NamedProperty(essentials::StringId id, essentials::observer_ptr<Property> property) :
		id_(std::move(id)),
		property_(std::move(property))
	{
	}

	bool has(essentials::StringId id) const {
		return id == id_;
	}

	Property get(essentials::StringId id) const {
		if (id != id_) {
			assert(!"Property not bound");
		}
		return *property_;
	}

private:

	essentials::StringId id_;

	essentials::observer_ptr<Property> property_;

};

bool hasShaderProperty(const NamedProperty& namedProperty, essentials::StringId id) {
	return namedProperty.has(std::move(id));
}

Property getShaderProperty(const NamedProperty& namedProperty, essentials::StringId id) {
	return namedProperty.get(std::move(id));
}

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_NAMEDPROPERTY_HPP_ */
