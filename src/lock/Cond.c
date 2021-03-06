#include "swoole.h"

int swCond_create(swCond *cond)
{
	if (pthread_cond_init(&cond->cond, NULL) < 0)
	{
		swWarn("pthread_cond_init fail. Error: %s [%d]", strerror(errno), errno);
		return SW_ERR;
	}
	if (swMutex_create(&cond->lock, 0) < 0)
	{
		return SW_ERR;
	}
	return SW_OK;
}

int swCond_notify(swCond *cond)
{
	return pthread_cond_signal(&cond->cond);
}

int swCond_broadcast(swCond *cond)
{
	return pthread_cond_broadcast(&cond->cond);
}

int swCond_timewait(swCond *cond, long sec, long nsec)
{
	int ret;
	struct timespec timeo;

	timeo.tv_sec = sec;
	timeo.tv_nsec = nsec;

	ret = pthread_cond_timedwait(&cond->cond, &cond->lock.object.mutex._lock, &timeo);
	return ret;
}

int swCond_wait(swCond *cond)
{
	int ret;
	ret = pthread_cond_wait(&cond->cond, &cond->lock.object.mutex._lock);
	return ret;
}

void swCond_free(swCond *cond)
{
	pthread_cond_destroy(&cond->cond);
	cond->lock.free(&cond->lock);
}
