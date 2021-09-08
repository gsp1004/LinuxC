#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"poly.h"
#include"gsp.h"
/*
多项式的合并
5 + 2x + 8x^8 + 3x^16
6x + 16x^6 - 8x^8
*/
int main()
{
	list *p1, *p2;
	int a[][2] = {{5,0},{2,1},{8,8},{3,16}};
	int b[][2] = {{6,1},{16,6},{-8,8}};


	p1 = poly_create(a,sizeof(a)/sizeof(a[0]));
	p2 = poly_create(b,sizeof(b)/sizeof(b[0]));

	poly_show(p1);
	poly_show(p2);

	poly_union(p1,p2);
	poly_show(p1);
}
