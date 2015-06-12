#include "backlog.h"

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
    pthread_mutex_lock(&blp->mutex);

    // Decrease the count.
    blp->count -= delta;

    // Did we cross the threshold?
    if (blp->count + delta > blp->limit &&
        blp->count <= blp->limit)
    {
        // Wake any waiters
        pthread_cond_broadcast(&blp->cond);
    }

    pthread_mutex_unlock(&blp->mutex);

    return 0;	// FIXME - status?
}

int
backlog_wait(backlog_t * blp)
{
    pthread_mutex_lock(&blp->mutex);

    while (blp->count > blp->limit)
        pthread_cond_wait(&blp->cond, &blp->mutex);
    
    pthread_mutex_unlock(&blp->mutex);

    return 0;	// FIXME - status?
}
