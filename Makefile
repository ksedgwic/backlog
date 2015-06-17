# Use Backlog?  [By default, yes.]
USE_BACKLOG = 1

# Use SystemTap?  [By default, yes.]
USE_SYSTEMTAP = 1

CFLAGS += -std=gnu99 -Wall -Werror
LDFLAGS += -lcrypto -lpthread

ifeq ($(USE_SYSTEMTAP),1)
CFLAGS +=	-DUSE_SYSTEMTAP
endif

ifeq ($(USE_BACKLOG),1)
CFLAGS +=	-DUSE_BACKLOG
endif

ifeq ($(USE_SYSTEMTAP),1)
SYSTEMTAP_PROBES_D = probes.d
SYSTEMTAP_PROBES_H = probes.h
SYSTEMTAP_PROBES_O = probes.o
endif

all:	sample

sample: $(SYSTEMTAP_PROBES_H) $(SYSTEMTAP_PROBES_O) sample.c backlog.h backlog.c

clean:
	rm -f sample $(SYSTEMTAP_PROBES_H) $(SYSTEMTAP_PROBES_O)

ifeq ($(USE_SYSTEMTAP),1)
$(SYSTEMTAP_PROBES_H):	$(SYSTEMTAP_PROBES_D)
	dtrace -h -s $< -o $@

$(SYSTEMTAP_PROBES_O):	$(SYSTEMTAP_PROBES_D)
	dtrace -G -s $< -o $@
endif
