#include <pthread.h>

typedef struct
{
    long			limit;
    pthread_mutex_t	mutex;	
    pthread_cond_t	cond;
    long			count;
    
} backlog_t;

// Initialize a backlog object.
int backlog_init(backlog_t * blp, long limit);

// Increase/decrease the backlog count.
int backlog_incr(backlog_t * blp, long delta);
int backlog_decr(backlog_t * blp, long delta);

// Throttle caller if backlog exceeds threshold.
int backlog_wait(backlog_t * blp);
