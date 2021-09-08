all:main


SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

target := main

$(target): $(OBJS)
	$(CC) $^ -o $@

ctags:
	ctags -R *.c *.h

clean:
	rm $(target) $(OBJS) -rf
