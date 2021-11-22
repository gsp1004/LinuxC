#ifndef MYPIPE_H__
#define MYPIPE_H__

#define PIPESIZE		(1024)
#define MYPIPE_READ		(0x1UL)
#define MYPIPE_WRITE	(0x2UL)


typedef void mypipe_t;

mypipe_t *mypipe_init(void);

int mypipe_register(mypipe_t *p, int opmap);

int mypipe_unregister(mypipe_t *p, int opmap);

int mypipe_read(mypipe_t *p, void *buf, size_t count);

int mypipe_write(mypipe_t *p, const void *buf, size_t count);

int mypipe_destroy(mypipe_t *p);

#endif
