
CFLAGS += -std=gnu99 -Wall -Werror
LDFLAGS += -lcrypto -lpthread

ifeq ($(USE_BACKLOG),1)
CFLAGS +=	-DUSE_BACKLOG
endif

all:	sample

sample: sample.c backlog.h backlog.c

clean:
	rm -f sample
