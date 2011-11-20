/*
 * main.c
 *
 *  Created on: 2011-11-11
 *      Author: liheyuan
 */

#include "ozone.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

void test_ozread_open()
{
	OZRead or;
	int ret;
	ret = ozread_open(&or, "/home/liheyuan/workspace_cpp_yq/kesdb/testd_db/");
	printf("test_ozread_open() %d\n", ret);
}

void test_ozread_get()
{
	OZRead or = {NULL, 0, NULL};
	if (ozread_open(&or, "/home/liheyuan/workspace_cpp_yq/kesdb/test_db/"))
	{
		printf("open db fail\n");
	}
	else
	{
		OZRead_Get get;
		int ret;
		get._key = "17196044";
		ret = ozread_get(&or, &get);
		if (!ret)
		{
			printf("ozread_get() succ\n");
			printf("%s\n", get._value);
		}
		else
		{
			printf("ozread_get() fail %d\n", ret);
		}
	}

	//Close
	ozread_close(&or);
}

void test_ozread_gets()
{
	OZRead or = {NULL, 0, NULL};
	const char* fn = "/tmp/test_db";

	if (ozread_open(&or, fn))
	{
		printf("open db fail\n");
	}
	else
	{
		OZRead_Gets gets;
		int ret, i;
		struct timeval start, end;
		int n = 10;

		printf("open db succ\n");
		//Get 20 keys
		for (i = 0; i < n; i++)
		{
			if(scanf("%s", gets._keys[i])!=1)
			{
				printf("Read key error\n");
			}
		}

		gettimeofday(&start, NULL);
		gets._nkeys = n;
		ret = ozread_gets(&or, &gets);
		if (!ret)
		{
			printf("ozread_gets() succ\n");
			for (i = 0; i < gets._nkeys; i++)
			{
				printf("%s\n", gets._keys[i]);
				printf("%d\n", strlen(gets._values[i]));
				//printf("%s\n", gets._values[i]);
			}
		}
		else
		{
			printf("ozread_gets() fail %d\n", ret);
		}
		gettimeofday(&end, NULL);
		printf("time %lf(s)\n", ((end.tv_sec * 1000000 + end.tv_usec)
				- (start.tv_sec * 1000000 + start.tv_usec)) / (float) (1000000));
	}

	//Close
	ozread_close(&or);
}

void test_ozwrite_put()
{
	OZWrite ow;
	char key[OZ_KEY_MAX];
	char value[OZ_VALUE_MAX];
	int i, j;

	if (ozwrite_open(&ow, "/tmp/test_db"))
	{
		printf("open db fail\n");
	}
	else
	{
		printf("open db succ\n");
		srandom(time(NULL));
		for (i = 0; i < 1000000; i++)
		{
			long k = random() % 10000000 + 10000000;
			snprintf(key, OZ_KEY_MAX, "%ld", k);
			value[0] = '\0';
			for(j = 0; j < 500; j++)
			{
				strcat(value, " ");
				strcat(value, key);
			}
			if(ozwrite_put(&ow, key, value))
			{
				printf("write %s fail.\n", key);
			}
		}
	}

	//Close
	ozwrite_close(&ow);
}


int main()
{
	//test_ozread_open();
	//test_ozread_get();
	test_ozread_gets();
	//test_ozwrite_put();


	return 0;
}
