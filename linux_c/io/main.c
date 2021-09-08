#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int myline(char **line, int *n, FILE *fp)
{
	int size = 120;
	char tmp[120] = {0};
	char * s = malloc(size);
	if(NULL == s || NULL == fgets(s, size, fp)) // char *fgets(char *s, int size, FILE *stream);
	{
		fprintf(stderr, "malloc or fgets error\n");
		exit(1);
	}

	while(NULL != fgets(tmp, sizeof(tmp), fp)){
		int size_new = size + strlen(tmp);
		char *t = realloc(s, size_new);
		if(NULL == t){
			fprintf(stderr, "malloc or fgets error\n");
			exit(1);
		}
		//memcpy(t+size, tmp, size_new - size);
		strcat(t, tmp);

		s = t;
		size = size_new;
	}
	*line = s;
	*n = size;
	return size;
}


int main(int argc, char *argv[])
{
	char s[50] = {0};
	memset(s,0,sizeof(s));

	int count = 0;
	int tmp = 13;
	fclose(fopen("t.txt", "w"));
	while(tmp--){
		FILE *fp = fopen("t.txt", "a+");
		if(NULL == fp)
			exit(1);
		fseek( fp, 0, SEEK_END);

		memset(s, '0'+(++count%10),sizeof(s)-1);
		if(fputs(s, fp)<0){
			fprintf(stderr,"fputs error\n");
		}
		fclose(fp);

		char * line = NULL;
		int size = 0;

		fp = fopen("t.txt", "r");
		//getline(&line, &size, fp);
		myline(&line, &size, fp);

		printf("(%4d)%s\n", size, line);
		fclose(fp);
	}
	
	exit(0);
}
