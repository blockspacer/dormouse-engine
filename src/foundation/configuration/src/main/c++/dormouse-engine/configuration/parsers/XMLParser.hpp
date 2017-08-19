#ifndef DORMOUSEENGINE_CONFIGURATION_PARSERS_XMLPARSER_HPP_
#define DORMOUSEENGINE_CONFIGURATION_PARSERS_XMLPARSER_HPP_

#include "PropertyTreeParser.hpp"

namespace dormouse_engine {
namespace configuration {
namespace parsers {

class XMLParser : public PropertyTreeParser {
protected:

	void doParse(std::istream& is, boost::property_tree::ptree* tree) const;

};

} // namespace parsers
} // namespace configuration
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_CONFIGURATION_PARSERS_XMLPARSER_HPP_ */
