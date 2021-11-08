#ifndef MYTBF_H__
#define MYTBF_H__

typedef void mytbf_t;

mytbf_t *mytbf_init(int cps, int burst);

int mytbf_fetchtoken(mytbf_t *, int);

int mytbf_returntoken(mytbf_t *, int);

int mytbf_destory(mytbf_t *ptr);

#endif
