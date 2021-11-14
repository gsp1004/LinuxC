target := main
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

LDFLAGS := -lpthread

CFLAGS := -g -Wall

CFLAGS += -I~/gsp_lib/include
CFLAGS += -L~/gsp_lib/lib

all:$(target)

$(target):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

ctags:
	ctags -R *.c *.h

clean:
	rm -rf $(target) $(OBJS) tags
