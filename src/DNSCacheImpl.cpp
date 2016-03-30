//
// Created by eugene on 26.03.16.
//

#include "DNSCacheImpl.h"

#include "ThreadLockGuard.h"

#include <system_error>

#define DNS_CACHE_DEFAULT_SIZE 5 // for test
//#define DNS_CACHE_DEFAULT_SIZE 512

DNSCache::DNSCache(size_t max_size) : m_nMaxSize(max_size)
{
    if (0  == max_size || m_HashMap.max_size() < max_size) throw new std::invalid_argument("max_size");

    if (0 != pthread_rwlock_init(&m_prwlock, nullptr))
        throw new std::system_error(std::make_error_code(std::errc::operation_not_permitted));

    m_HashMap.max_load_factor(1);
    m_HashMap.reserve(max_size);
}

DNSCache::~DNSCache()
{
    while (EBUSY == pthread_rwlock_destroy(&m_prwlock)) {};
}

void DNSCache::update(const std::string &name, const std::string &ip)
{
    if (name.empty() || ip.empty()) return;

    ReadWriteThreadLockGuard Guard(&m_prwlock);

    auto res = m_HashMap.insert(std::make_pair(name, DNSItemPtr()));
    if (!res.second) // update ip and move list item to the end
    {
        DNSItemPtr& Item = res.first->second;
        Item->ip = ip;
        m_DNSList.erase(Item->listRef);
        Item->listRef  = m_DNSList.insert(m_DNSList.end(), res.first);
    }
    else // insert new value
    {
        if (m_HashMap.size() > this->getMaxSize()) packCache();
        DNSList::iterator listIter = m_DNSList.insert(m_DNSList.end(), res.first);
        DNSItemPtr& Item = res.first->second;
        Item.reset(new DNSItem(ip, listIter));
    }
}

std::string DNSCache::resolve(const std::string &name) const
{
    if (name.empty()) return std::string();

    ReadThreadLockGuard Guard(&m_prwlock);

    auto search_res = m_HashMap.find(name);
    if (m_HashMap.end() != search_res)
        return search_res->second->ip;

    return std::string();
}

void DNSCache::packCache(size_t count/* = 1*/)
{
    size_t nCnt = std::min(count, m_HashMap.size());
    while (nCnt--)
    {
        m_HashMap.erase(m_DNSList.front());
        m_DNSList.pop_front();
    }
}

DNSCache &DNSCache::getInstance()
{
    static DNSCache s_DnsCache(DNS_CACHE_DEFAULT_SIZE); // thread-safe with C++11

    return s_DnsCache;
}








