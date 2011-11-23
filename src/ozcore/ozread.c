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
	/* Variable */
	FILE* out;
	long cnt = 0;
	char cmd[OZ_BUF_SIZE];
	char key[OZ_KEY_BUF_SIZE];
	uint64_t offset;
	uint32_t length;

	/* Use sort twice to sort/uniqure keyfile -> pipe to FILE* */
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

	/* Read lines and datas */
	if (fscanf(out, "%ld", &handle->_nrecs) != 1 || handle->_nrecs == 0)
	{
		fclose(out);
		return 2;
	}

	handle->_recs = (OZRecord*) malloc(sizeof(OZRecord) * handle->_nrecs);
	while (fscanf(out, "%s\t%llu\t%u\n", key, &offset, &length) != EOF)
	{
		/* Copy to table */
		handle->_recs[cnt]._offset = offset;
		handle->_recs[cnt]._length = length;
		handle->_recs[cnt]._key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
		strcpy(handle->_recs[cnt]._key, key);
		cnt++;
	}

	/* Close */
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

	//Allocate memory for value
	param->_value = (char*)malloc(sizeof(char)*( rec->_length + 1));
	if(!param->_value)
	{
		return 5;
	}

	//Get Value in file
	if ((ret = ozread_get_value(handle, rec, param->_value)))
	{
		return 4;
	}

	//Get key & value succ
	return 0;
}

int ozread_get_key(OZRead* handle, const char* key, OZRecord** rec)
{
	/* Use Binary Search to locate Record */
	long low = 0;
	long high = handle->_nrecs - 1;
	long mid = 0;
	while (low <= high)
	{
		mid = (low + high) / 2;
		int val = strcmp(handle->_recs[mid]._key, key);
		if (val == 0)
		{
			/* Found */
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
	/* Not Found */
	*rec = NULL;
	return 1;
}

int ozread_get_value(OZRead* handle, const OZRecord* rec, char* buf)
{
	int ret;
	
	/* fseeko */
	if (fseeko(handle->_fpval, rec->_offset, SEEK_SET))
	{
		return 2;
	}

	/* fread */
	if ((ret = fread(buf, rec->_length, 1, handle->_fpval)) != 1)
	{
		return 3;
	}

	return 0;
}

int ozread_gets_init(OZRead_Gets* param ,long n)
{
	/* n */
	param->_nkeys = n;

	/* malloc keys pointer */
	param->_keys = (char**)malloc(sizeof(char*)*n);
	if(!param->_keys)
	{
		return 1;
	}
	param->_keys = (char**)malloc(sizeof(char*)*n);

	/* malloc values pointer */
	param->_values = (char**)malloc(sizeof(char*)*n);
	if(!param->_values)
	{
		return 1;
	}
	memset(param->_values, 0, sizeof(char*)*n);

	/* malloc cookie buffer */
	param->_cookies = (OZRead_Cookie*)malloc( sizeof(OZRead_Cookie) * n );
	if(!param->_cookies)
	{
		return 1;
	}

	return 0;
}

void ozread_gets_free(OZRead_Gets* param)
{
	long i;

	/* free keys pointer */
	if(param->_keys)
	{
		free(param->_keys);
		param->_keys = NULL;
	}

	/* free values pointer and malloc space */
	for(i=0; i<param->_nkeys; i++)
	{
		if(param->_values[i])
		{
			free(param->_values[i]);
			param->_values[i] = NULL;
		}
	}
	if(param->_values)
	{
		free(param->_values);
		param->_values = NULL;
	}

	/* malloc cookie buffer */
	if(param->_cookies)
	{
		free(param->_cookies);
		param->_cookies = NULL;
	}

	/* n */
	param->_nkeys = 0;
}

int ozread_gets(OZRead* handle, OZRead_Gets* param)
{
	/* Var */
	long i;

	/* Check handle */
	if (!handle)
	{
		return 1;
	}
	if (!param)
	{
		return 2;
	}

	/* Get keys */
	if(ozread_gets_keys(handle, param))
	{
		return 3;
	}

	/* malloc space for each values according cookies[i]_length */
	for(i=0; i<param->_ncookies; i++)
	{
		param->_values[param->_cookies[i]._index] = (char*)malloc(sizeof(char)*(param->_cookies[i]._length + 1));
		if(!param->_values[param->_cookies[i]._index])
		{
			return 1;
		}
	}

	/* Get values, set into param->values */
	ozread_gets_values(handle, param);

	/* All succ */
	return 0;
}

int cmp_ozread_gets_keys(const void* a, const void* b)
{
	long long x, y;
	x = ((OZRead_Cookie*) a)->_offset;
	y = ((OZRead_Cookie*) b)->_offset;
	/* Compare */
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
	/* Var */
	int i, cnt;
	OZRecord* rec = NULL;

	/* Read all keys */
	for (cnt = 0, i = 0; i < param->_nkeys; i++)
	{
		/* get key & skip null key */
		if (param->_keys[i] && !ozread_get_key(handle, param->_keys[i], &rec))
		{
			/* Found */
			param->_cookies[cnt]._offset = rec->_offset;
			param->_cookies[cnt]._length = rec->_length;
			param->_cookies[cnt]._index = i;
			cnt++;
		}
	}
	param->_ncookies = cnt;


	/* sort by _offset */
	qsort(param->_cookies, param->_ncookies, sizeof(OZRead_Cookie),
			cmp_ozread_gets_keys);

	/* Test only */
	/*for (i = 0; i < param->_ncookies; i++)
	{
		printf("%s %llu %u\n", param->_keys[param->_cookies[i]._index],
				param->_cookies[i]._offset, param->_cookies[i]._length);
	}*/

	return 0;
}

int ozread_gets_values(OZRead* handle, OZRead_Gets* param)
{
	/* Var */
	int i;
	OZRead_Cookie* pc;

	/* Get each value according to and set it into param->_values */
	for (i = 0; i < param->_ncookies; i++)
	{
		pc = &(param ->_cookies[i]);
		/* fseek */
		if (fseeko(handle->_fpval, pc->_offset, SEEK_SET))
		{
			/* fseek fail */
			continue;
		}

		/* Check buffer avaliable */
		if (!param->_values[pc->_index])
		{
			continue;
		}

		/* fread */
		if (fread(param->_values[pc->_index], pc->_length, 1, handle->_fpval) != 1)
		{
			/* fread fail */
			param->_values[pc->_index][0] = '\0';
			continue;
		}
		param->_values[pc->_index][pc->_length] = '\0';
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

void ozread_get_free(OZRead_Get* param)
{
	param->_key = NULL;
	if(param->_value)
	{
		free(param->_value);
		param->_value = NULL;
	}
}
