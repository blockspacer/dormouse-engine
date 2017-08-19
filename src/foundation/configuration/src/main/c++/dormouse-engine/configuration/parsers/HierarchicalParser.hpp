#ifndef DORMOUSEENGINE_CONFIGURATION_PARSERS_HIERARCHICALPARSER_HPP_
#define DORMOUSEENGINE_CONFIGURATION_PARSERS_HIERARCHICALPARSER_HPP_

#include <functional>

#include <boost/filesystem/path.hpp>

namespace dormouse_engine {
namespace configuration {
namespace parsers {

class HierarchicalParser {
public:

	typedef std::function<void (const std::string&, const std::string&)> NewChildCallback;

	typedef std::function<void ()> ChildrenEndCallback;

	virtual ~HierarchicalParser() {
	}

	virtual void parse(std::istream& is, NewChildCallback newChildCallback,
			ChildrenEndCallback childrenEndCallback) const = 0;

	void parse(const boost::filesystem::path& path, NewChildCallback newChildCallback,
			ChildrenEndCallback childrenEndCallback) const;

};

}  // namespace parsers
}  // namespace configuration
}  // namespace dormouse_engine

#endif /* DORMOUSEENGINE_CONFIGURATION_PARSERS_HIERARCHICALPARSER_HPP_ */
