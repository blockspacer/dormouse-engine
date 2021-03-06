#ifndef DORMOUSEENGINE_LOGGER_SYSTEMLOGGER_HPP_
#define DORMOUSEENGINE_LOGGER_SYSTEMLOGGER_HPP_

#include <boost/thread/mutex.hpp>

#include "dormouse-engine/essentials/policy/creation/New.hpp"

#include "dormouse-engine/essentials/Singleton.hpp"

#include "Logger.hpp"

namespace dormouse_engine::logger {

class SystemLogger :
	public essentials::Singleton<
		Logger,
		essentials::policy::creation::New<SystemLogger>
		>,
	public Logger
{
public:

	SystemLogger();

};

} // namespace dormouse_engine::logger

#endif /* DORMOUSEENGINE_LOGGER_SYSTEMLOGGER_HPP_ */
