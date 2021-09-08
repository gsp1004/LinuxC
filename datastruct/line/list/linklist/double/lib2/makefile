target := main
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

CFLAGS := -g -Wall

CFLAGS += -I~/gsp_lib/include
CFLAGS += -L~/gsp_lib/lib

all:ctags $(target)

$(target):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

ctags:
	ctags -R *.c *.h

clean:
	rm -rf $(target) $(OBJS) tags
