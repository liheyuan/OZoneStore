/*
 * main.c
 *
 *  Created on: 2011-11-11
 *      Author: liheyuan
 */

#include "ozsort.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

void test_ozsort_work()
{
    OZSort param;
    strcpy(param._src, "/tmp/test_db/key.dat");
    printf("%d\n", ozsort_work(&param));
	printf("%ld\n", param._mlines);
}

void test_ozsort_lines()
{
	printf("Lines:\n", ozsort_lines("/tmp/ozsort_merge_1321782715"));
}

int main()
{
    test_ozsort_work();
	//test_ozsort_lines();

	sleep(10);
	return 0;
}
