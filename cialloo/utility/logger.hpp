#ifndef CIALLOO_UTILITY_LOGGER_HPP
#define CIALLOO_UTILITY_LOGGER_HPP

#include <fstream>
#include <iostream>
#include <string>

namespace cialloo {
namespace utility {

class logger
{
public:
    logger() = default;
    logger(std::string file_path);
    ~logger();

    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;

private:
};

inline logger::logger()
{

}

inline logger::logger(std::string file_path)
{
}

inline logger::~logger()
{
}


} // namespace utility
} // namespace cialloo


#endif