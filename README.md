Backlog Sample
================================================================

The sample program creates 16 producer threads and 16 consumer threads.

Both producers and consumers perform CPU-bound work (computing digests).

The producers are twice as fast as the consumers (smaller digests).

Without the backlog api the producers finish up well before the consumers.

The backlog api throttles the producers so they are no more then 16
blocks ahead of the consumers at any point; they finish together.

    # Build and run w/o backlog API.
    make clean all
    ./sample

    # Build and run with the backlog API.
    make USE_BACKLOG=1 clean all
    ./sample
