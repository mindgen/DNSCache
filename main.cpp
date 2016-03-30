#include "src/DNSCacheImpl.h"

#include <algorithm>
#include <unistd.h>

#define TST_CNT 10

void* threadProc(void* pData)
{
    std::array<std::string, TST_CNT> names;
    std::array<std::string, TST_CNT> ips;
    for(size_t nItm = 0; nItm < TST_CNT; ++nItm)
    {
        names[nItm] = "Host" + std::to_string(nItm);
        ips[nItm] = "IP" + std::to_string(nItm);
    }

    sleep(rand() % 5);

    for (size_t n = 0; n < 100; n++)
    {
        DNSCache& cache = DNSCache::getInstance();
        if (rand() % 2)
        {
            std::string s_name = names[rand() % TST_CNT];
            std::string s_ip = ips[rand() % TST_CNT];
            cache.update(s_name, s_ip);
        }
        else
        {
            std::string s_name = names[rand() % TST_CNT];
            std::string s_ip = cache.resolve(s_name);
        }
    }
}

int main()
{
    //test DNSCache
    std::array<pthread_t, 10> threads;
    std::for_each(threads.begin(), threads.end(), [](pthread_t &thread)
    { pthread_create(&thread, nullptr, threadProc, nullptr); });

    std::for_each(threads.begin(), threads.end(), [](pthread_t &thread)
    { pthread_join(thread, nullptr); });

    return 0;
};