#include "../../include/library.hpp"

/**
 * @brief Obtains ass std::string the current time data for displaying
 */
std::string ObtainCurrentTime()
{
	time_t now = time(NULL);
	struct tm* tm_info = localtime(&now);
	char buffer[64];
	strftime(buffer, sizeof(buffer), "\033[32m[%Y-%m-%d %H:%M:%S]\033[0m ", tm_info);
	return buffer;
}

/**
 * @brief Pseudo printf function implemented to make logs with the current time
 * @param oss the ostringstream that indicate when to print / write the message
 * @param message the message in a c++ way to write
 */
void PrintLog(std::ostream& oss, const std::string message, ...)
{
	char msg[512];
	va_list args;
	va_start(args, message);
	vsnprintf(msg, sizeof(msg), message.c_str(), args);
	va_end(args);

	std::string fullLog = std::string(ObtainCurrentTime()) + msg;
	oss << fullLog << std::endl;
}
