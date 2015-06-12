
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include <openssl/sha.h>

#include "backlog.h"

#define NCONSUMERS	16
#define NPRODUCERS	16

#define NUMBLOCKS	64
#define NPITER		(1024 * 1024)
#define NCITER		(2 * NPITER)

static void
produce_block(int blocknum)
{
    // Waste some CPU cycles computing the SHA256 of some integers.
    uint8_t md[SHA256_DIGEST_LENGTH];
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    for (int ii = 0; ii < NPITER; ++ii)
        SHA256_Update(&ctx, &ii, sizeof(ii));
    SHA256_Final(md, &ctx);
}

static void
consume_block(int blocknum)
{
    // Waste some CPU cycles computing the SHA256 of some integers.
    uint8_t md[SHA256_DIGEST_LENGTH];
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    for (int ii = 0; ii < NCITER; ++ii)
        SHA256_Update(&ctx, &ii, sizeof(ii));
    SHA256_Final(md, &ctx);
}

backlog_t g_backlog;

static void *
producer_thread(void * arg)
{
    long id = (long) arg;
    printf("producer %2ld starting\n", id);

    for (int ii = 0; ii < NUMBLOCKS; ++ii)
    {
#if defined(USE_BACKLOG)
        backlog_wait(&g_backlog);
#endif

        if (ii % 10 == 0)
            printf("producer %2ld, block %3d\n", id, ii);

        produce_block(ii);

#if defined(USE_BACKLOG)
        backlog_incr(&g_backlog, 1);
#endif
    }
    
    printf("producer %2ld finished\n", id);
    return NULL;
}

static void *
consumer_thread(void * arg)
{
    long id = (long) arg;
    printf("consumer %2ld starting\n", id);

    for (int ii = 0; ii < NUMBLOCKS; ++ii)
    {
        if (ii % 10 == 0)
            printf("consumer %2ld, block %3d\n", id, ii);

        consume_block(ii);

#if defined(USE_BACKLOG)
        backlog_decr(&g_backlog, 1);
#endif
    }
    
    printf("consumer %2ld finished\n", id);
    return NULL;
}

static uint64_t
now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * (uint64_t) 1000000) + tv.tv_usec;
}

int
main(int argc, char ** argv)
{
    pthread_t ct[NPRODUCERS];
    pthread_t pt[NPRODUCERS];

#if defined(USE_BACKLOG)
    backlog_init(&g_backlog, 16);
#endif

    uint64_t t0 = now();
    
    for (long ii = 0; ii < NCONSUMERS; ++ii)
        pthread_create(&ct[ii], NULL, consumer_thread, (void *) ii);

    for (long ii = 0; ii < NPRODUCERS; ++ii)
        pthread_create(&pt[ii], NULL, producer_thread, (void *) ii);


    for (long ii = 0; ii < NPRODUCERS; ++ii)
        pthread_join(pt[ii], NULL);

    printf("---- ALL PRODUCERS FINISHED ----\n");

    for (long ii = 0; ii < NCONSUMERS; ++ii)
        pthread_join(ct[ii], NULL);

    uint64_t t1 = now();

#if defined(USE_BACKLOG)
    printf("  using backlog program took %f mSec\n", (double)(t1 - t0) / 1000.0);
#else
    printf("without backlog program took %f mSec\n", (double)(t1 - t0) / 1000.0);
#endif

    return 0;
}
