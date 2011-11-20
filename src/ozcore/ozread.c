/*
 * ozread.c
 *
 *  Created on: 2011-11-11
 *      Author: liheyuan
 */

#include "ozread.h"
#include <string.h>
#include <stdlib.h>


int ozread_open(OZRead* handle, const char* dbpath)
{
	int ret_kf = ozread_open_kf(handle, dbpath);
	if (ret_kf)
	{
		return ret_kf;
	}
	else
	{
		return ozread_open_vf(handle, dbpath);
	}
}

int ozread_open_kf(OZRead* handle, const char* dbpath)
{
	//Variable
	FILE* out;
	long cnt = 0;
	char cmd[OZ_BUF_SIZE];
	char key[OZ_KEY_MAX];
	uint64_t offset;
	uint32_t length;

	//Use sort twice to sort/uniqure keyfile -> pipe to FILE*
	snprintf(
			cmd,
			OZ_BUF_SIZE,
			"./OZoneSort %s/%s",
			dbpath, OZ_KEY_FILENAME);
	out = popen(cmd, "r");
	if (!out)
	{
		return 1;
	}
	printf("%s\n", cmd);

	//Read lines and datas
	if (fscanf(out, "%ld", &handle->_nrecs) != 1 || handle->_nrecs == 0)
	{
		fclose(out);
		return 2;
	}
	//printf("%ld\n", handle->_nrecs);
	handle->_recs = (OZRecord*) malloc(sizeof(OZRecord) * handle->_nrecs);
	//printf("a\n");
	while (fscanf(out, "%s\t%llu\t%u\n", key, &offset, &length) != EOF)
	{
		//Copy to table
		handle->_recs[cnt]._offset = offset;
		handle->_recs[cnt]._length = length;
		handle->_recs[cnt]._key = malloc(sizeof(char) * (strlen(key) + 1));
		strcpy(handle->_recs[cnt]._key, key);
		cnt++;
	}
	//printf("%ld\n", cnt);

	//Close
	fclose(out);

	return 0;
}

int ozread_open_vf(OZRead* handle, const char* dbpath)
{
	char buf[OZ_BUF_SIZE];
	snprintf(buf, OZ_BUF_SIZE, "%s/%s", dbpath, OZ_VALUE_FILENAME);
	handle->_fpval = fopen(buf, "rb");
	if (handle->_fpval)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int ozread_get(OZRead* handle, OZRead_Get* param)
{
	//Var
	OZRecord* rec;
	int ret;

	//Check handle & param
	if (!handle)
	{
		return 1;
	}
	if (!param)
	{
		return 2;
	}

	//Get Key Structure in RAM
	if (ozread_get_key(handle, param->_key, &rec))
	{
		//Not found key
		return 3;
	}
	//printf("%s %llu %u\n", rec->_key, rec->_offset, rec->_length);

	//Get Value in file
	if ((ret = ozread_get_value(handle, rec, param->_value, OZ_VALUE_MAX)))
	{
		//printf("%d\n", ret);
		return 4;
	}

	//Get key & value succ
	return 0;
}

int ozread_get_key(OZRead* handle, const char* key, OZRecord** rec)
{
	//Use Binary Search to locate Record
	long low = 0;
	long high = handle->_nrecs - 1;
	long mid = 0;
	while (low <= high)
	{
		mid = (low + high) / 2;
		int val = strcmp(handle->_recs[mid]._key, key);
		if (val == 0)
		{
			//Found
			*rec = &(handle->_recs[mid]);
			return 0;
		}
		else if (val > 0)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	//Not Found
	*rec = NULL;
	return 1;
}

int ozread_get_value(OZRead* handle, const OZRecord* rec, char* buf,
		long buf_len)
{
	int ret;
	//Check value length and buffer
	if (rec->_length >= buf_len)
	{
		return 1;
	}

	if (fseeko(handle->_fpval, rec->_offset, SEEK_SET))
	{
		return 2;
	}

	//Read
	if ((ret = fread(buf, rec->_length, 1, handle->_fpval)) != 1)
	{
		printf("%d\n", ret);
		return 3;
	}

	return 0;
}

int ozread_gets(OZRead* handle, OZRead_Gets* param)
{
	//Check handle
	if (!handle)
	{
		return 1;
	}
	if (!param)
	{
		return 2;
	}

	//Get keys and sort them in _offset asc order, store in param->cookies

	//Get keys
	ozread_gets_keys(handle, param);

	//Memset clear param->_values
	memset(param->_values, 0, OZ_VALUE_MAX * OZREAD_GETS_MAX);

	//Get values, set into param->values
	ozread_gets_values(handle, param);

	//All succ
	return 0;
}

int cmp_ozread_gets_keys(const void* a, const void* b)
{
	long long x, y;
	x = ((OZRead_Cookie*) a)->_offset;
	y = ((OZRead_Cookie*) b)->_offset;
	//Compare
	if (x > y)
	{
		return 1;
	}
	else if (x < y)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int ozread_gets_keys(OZRead* handle, OZRead_Gets* param)
{
	//Var
	int i, cnt;
	OZRecord* rec = NULL;

	//Read all keys
	for (cnt = 0, i = 0; i < param->_nkeys; i++)
	{
		if (!ozread_get_key(handle, param->_keys[i], &rec))
		{
			//Found
			param->_cookies[cnt]._offset = rec->_offset;
			param->_cookies[cnt]._length = rec->_length;
			param->_cookies[cnt]._index = i;
			cnt++;
		}
	}
	param->_ncookies = cnt;

	//Sort by _offset
	qsort(param->_cookies, param->_ncookies, sizeof(OZRead_Cookie),
			cmp_ozread_gets_keys);

	//Test only
	for (i = 0; i < param->_ncookies; i++)
	{
		printf("%s %llu %u\n", param->_keys[param->_cookies[i]._index],
				param->_cookies[i]._offset, param->_cookies[i]._length);
	}

	return 0;
}

int ozread_gets_values(OZRead* handle, OZRead_Gets* param)
{
	//Var
	char buf[OZ_VALUE_MAX];
	int i;
	OZRead_Cookie* pc;

	//Get each value according to and set it into param->_values
	for (i = 0; i < param->_ncookies; i++)
	{
		pc = &(param ->_cookies[i]);
		memset(buf, 0, sizeof(char) * OZ_VALUE_MAX);
		//fseek
		if (fseeko(handle->_fpval, pc->_offset, SEEK_SET))
		{
			//fseek fail
			printf("fseek fail\n");
			continue;
		}

		//Check size
		if (pc->_length >= OZ_VALUE_MAX)
		{
			//Value size bigger than pre-define
			printf("check size\n");
			continue;
		}

		//fread
		if (fread(buf, pc->_length, 1, handle->_fpval) != 1)
		{
			//fread	fail
			printf("fread fail\n");
			continue;
		}

		//set to _values
		memcpy(param->_values[pc->_index], buf, pc->_length);
	}

	return 0;
}

void ozread_close(OZRead* handle)
{
	if (handle)
	{
		if (handle->_recs)
		{
			free(handle->_recs);
		}
		handle->_nrecs = 0;
		if (handle->_fpval)
		{
			fclose(handle->_fpval);
		}
	}
}
