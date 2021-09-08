target := main
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

CFLAGS := -g

all:ctags $(target)

$(target):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

ctags:
	ctags -R *.c *.h

clean:
	rm -rf $(target) $(OBJS) tags
