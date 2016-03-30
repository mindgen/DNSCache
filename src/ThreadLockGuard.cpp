//
// Created by eugene on 26.03.16.
//

#include "ThreadLockGuard.h"

ReadThreadLockGuard::ReadThreadLockGuard(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_rdlock(rwlock);
    m_prwlock = rwlock;
}

ReadThreadLockGuard::~ReadThreadLockGuard()
{
    pthread_rwlock_unlock(m_prwlock);
}

ReadWriteThreadLockGuard::ReadWriteThreadLockGuard(pthread_rwlock_t *rwlock)
{
    pthread_rwlock_wrlock(rwlock);
    m_prwlock = rwlock;
}

ReadWriteThreadLockGuard::~ReadWriteThreadLockGuard()
{
    pthread_rwlock_unlock(m_prwlock);
}


