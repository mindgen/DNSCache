//
// Created by eugene on 26.03.16.
//

#ifndef DNSCACHE_THREADLOCKGUARD_H
#define DNSCACHE_THREADLOCKGUARD_H

#include <pthread.h>

class ReadThreadLockGuard
{
public:
    ReadThreadLockGuard(pthread_rwlock_t* rwlock);
    virtual ~ReadThreadLockGuard();

private:
    pthread_rwlock_t* m_prwlock;
};

class ReadWriteThreadLockGuard
{
public:
    ReadWriteThreadLockGuard(pthread_rwlock_t* rwlock);
    virtual ~ReadWriteThreadLockGuard();

private:
    pthread_rwlock_t* m_prwlock;
};


#endif //DNSCACHE_THREADLOCKGUARD_H
