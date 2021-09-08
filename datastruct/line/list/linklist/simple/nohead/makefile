target := main
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

all:ctags $(target)

$(target):$(OBJS)
	$(CC) -o $@ $^

ctags:
	ctags -R *.c *.h

clean:
	rm -rf $(target) $(OBJS) tags
