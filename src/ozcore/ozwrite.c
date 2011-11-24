/*
 * ozwrite.c
 *
 *  Created on: 2011-11-14
 *      Author: liheyuan
 */

#include "ozwrite.h"
#include <string.h>
#include <sys/stat.h>

int ozwrite_open(OZWrite* handle, const char* dbpath)
{
	//Var
	char buf[OZ_BUF_SIZE];

	//Check handle
	if (!handle)
	{
		return 1;
	}

	//Make dir
	mkdir(dbpath, 0755);

	//Open Key file
	snprintf(buf, OZ_BUF_SIZE, "%s/%s", dbpath, OZ_KEY_FILENAME);
	handle->_fpkey = fopen(buf, "a");
	if (!handle->_fpkey)
	{
		return 2;
	}

	//Open Value file
	snprintf(buf, OZ_BUF_SIZE, "%s/%s", dbpath, OZ_VALUE_FILENAME);
	handle->_fpval = fopen(buf, "ab");
	if (!handle->_fpval)
	{
		return 2;
	}

	//Succ
	return 0;
}

int ozwrite_put(OZWrite* handle, const char* key, const char* value)
{
	//Var
	off_t off;
	uint32_t len;

	//Check handle
	if (!handle)
	{
		return 1;
	}

	//Check value length
	if(strlen(value)<1)
	{
		return 5;
	}

	//Before Write value, get current offset
	off = ftello(handle->_fpval);
	if (off == -1)
	{
		return 4;
	}

	//Write Value
	len = strlen(value);
	if (fwrite(value, len, 1, handle->_fpval) != 1)
	{
		return 3;
	}

	//Check key length
	if(strlen(key)<1)
	{
		return 6;
	}

	//Write Key
	if(fprintf(handle->_fpkey, "%s\t%lld\t%u\n", key, off, len) == -1)
	{
		return 2;
	}


	//Flush Key and Value
	fflush(handle->_fpkey);
	fflush(handle->_fpval);

	//All Succ
	return 0;
}

void ozwrite_close(OZWrite* handle)
{
	fclose(handle->_fpkey);
	fclose(handle->_fpval);
}
