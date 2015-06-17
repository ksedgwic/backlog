#include "backlog.h"

#if defined(USE_SYSTEMTAP)
#include <sys/sdt.h>
#include "probes.h"
#endif

int
backlog_init(backlog_t * blp, long limit)
{
    blp->limit = limit;
    pthread_mutex_init(&blp->mutex, NULL);
    pthread_cond_init(&blp->cond, NULL);
    blp->count = 0;
    return 0;	// FIXME - status?
}

int
backlog_incr(backlog_t * blp, long delta)
{
    pthread_mutex_lock(&blp->mutex);
    blp->count += delta;
    pthread_mutex_unlock(&blp->mutex);
    return 0;	// FIXME - status?
}

int
backlog_decr(backlog_t * blp, long delta)
{
#if defined(USE_SYSTEMTAP)
    BACKLOG_DECR_STARTING(blp->count, delta);
#endif

    pthread_mutex_lock(&blp->mutex);

    // Decrease the count.
    blp->count -= delta;

    // Did we cross the threshold?
    if (blp->count + delta > blp->limit &&
        blp->count <= blp->limit)
    {
#if defined(USE_SYSTEMTAP)
        BACKLOG_DECR_BROADCAST(blp->count, delta);
#endif
        // Wake any waiters
        pthread_cond_broadcast(&blp->cond);
    }

    pthread_mutex_unlock(&blp->mutex);

#if defined(USE_SYSTEMTAP)
    BACKLOG_DECR_FINISHED(blp->count, delta);
#endif

    return 0;	// FIXME - status?
}

int
backlog_wait(backlog_t * blp)
{
#if defined(USE_SYSTEMTAP)
    BACKLOG_WAIT_STARTING(blp->count);
#endif

    pthread_mutex_lock(&blp->mutex);

    while (blp->count > blp->limit) {
#if defined(USE_SYSTEMTAP)
        BACKLOG_WAIT_SLEEPING(blp->count);
#endif
        pthread_cond_wait(&blp->cond, &blp->mutex);
#if defined(USE_SYSTEMTAP)
        BACKLOG_WAIT_AWOKEN(blp->count);
#endif
    }
    
    pthread_mutex_unlock(&blp->mutex);

#if defined(USE_SYSTEMTAP)
    BACKLOG_WAIT_FINISHED(blp->count);
#endif

    return 0;	// FIXME - status?
}
