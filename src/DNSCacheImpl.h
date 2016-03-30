//
// Created by eugene on 26.03.16.
//

#ifndef DNSCACHE_DNSCACHEIMPL_H
#define DNSCACHE_DNSCACHEIMPL_H

#include <string>
#include <list>
#include <unordered_map>
#include <memory>
#include <pthread.h>

class DNSCache final
{
private:
    DNSCache(const DNSCache& value) = delete;
    DNSCache& operator=( const DNSCache& ) = delete;

public:
    explicit DNSCache(size_t max_size);
    ~DNSCache();

    void update(const std::string& name, const std::string& ip);
    std::string resolve(const std::string& name) const;

    static DNSCache& getInstance();

private:
    inline void packCache(size_t count = 1);
    size_t getMaxSize() const { return m_nMaxSize; }

    struct DNSItem;
    typedef std::shared_ptr<DNSItem> DNSItemPtr;
    typedef std::unordered_map<std::string, DNSItemPtr> DNSIndex;
    typedef std::list<DNSIndex::const_iterator> DNSList;

    struct DNSItem
    {
        std::string ip;
        DNSList::iterator listRef;
        DNSItem(const std::string& _ip, DNSList::iterator& it) : ip(_ip), listRef(it) {};
    };

    DNSList m_DNSList;
    DNSIndex m_HashMap;
    size_t m_nMaxSize;

    mutable pthread_rwlock_t m_prwlock;
};

#endif //DNSCACHE_DNSCACHEIMPL_H
