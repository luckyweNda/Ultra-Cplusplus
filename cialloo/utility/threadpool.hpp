#ifndef CIALLOO_UTILITY_THREADPOOL_HPP
#define CIALLOO_NETWORK_TCP_SERVER_HPP

#include <thread>
#include <atomic>

namespace cialloo {
namespace utility {

class threadpool
{
public:
    explicit threadpool(int num);
    ~threadpool();

    threadpool(const threadpool&) = delete;
    threadpool& operator=(const threadpool&) = delete;
};

inline threadpool::threadpool(int num)
{
}

inline threadpool::~threadpool()
{
}

} // namespace utility
} // namespace cialloo


#endif