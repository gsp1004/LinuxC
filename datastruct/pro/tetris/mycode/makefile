target := main
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

CFLAGS := -g -Wall

#CFLAGS += -I~/gsp_lib/include
#CFLAGS += -L~/gsp_lib/lib
#CFLAGS += -lqueue

CFLAGS += -I/home/gsp/gsp_lib/include
CFLAGS += -L/home/gsp/gsp_lib/lib

LIBS := -lqueue -lpthread

all:ctags $(target)

$(target):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

ctags:
	ctags -R *.c *.h

clean:
	rm -rf $(target) $(OBJS) tags
