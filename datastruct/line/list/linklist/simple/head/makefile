target := main
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

all:$(target) ctags

$(target):$(OBJS)
	$(CC) -o $@ $^

ctags:
	ctags -R *.c *.h

clean:
	rm -rf $(target) $(OBJS)
