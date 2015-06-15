Backlog Sample
----------------------------------------------------------------

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


Measuring Time Slices w/ systemtap
----------------------------------------------------------------

I wrote a simple systemtap script to record a target program's time slices.

Setup Systemtap

    sudo yum install systemtap systemtap-runtime systemtap-sdt-devel
    sudo yum install kernel-debuginfo

    # Add user to stap groups:
    stapusr:x:156:citrusleaf
    stapsys:x:157:citrusleaf
    stapdev:x:158:citrusleaf

Run it like this:

    stap slice.stp -c ./sample -o slices.log

A sample excerpt from the slices.log is:

```
1434071285708267 30237  6000 uSec
1434071285709266 30249  6000 uSec
1434071285710266 30252  6001 uSec
1434071285711320 30224  3407 uSec
1434071285711603 30248  3337 uSec
1434071285712266 30250  6000 uSec
1434071285712276 30241  6000 uSec
1434071285713544 30233  5625 uSec
1434071285713557 30236  2233 uSec
1434071285714132 30246  4866 uSec
1434071285714267 30239  6000 uSec
1434071285716266 30238  6000 uSec
1434071285716857 30227  3291 uSec
1434071285718266 30242  6660 uSec
1434071285718276 30231  6000 uSec
1434071285718286 30235  6020 uSec                                                                 
```
