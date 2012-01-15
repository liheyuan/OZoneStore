#include "oztrav.h"
#include <string.h>

int oztrav_open(OZTrav* handle, const char* dbpath)
{
	//Open key & value dat
	int ret_kf = oztrav_open_kf(handle, dbpath);
	if (ret_kf)
	{   
		return ret_kf;
	}   
	else
	{   
		return oztrav_open_vf(handle, dbpath);
	}  	
}

int oztrav_open_kf(OZTrav* handle, const char* dbpath)
{
	/* Variable */
	FILE* fpkey;
	long cnt = 0;
	char buf[OZ_BUF_SIZE];
	char key[OZ_KEY_BUF_SIZE];
	uint64_t offset;
	uint32_t length;
	int ret;

	/* Open key file */
	snprintf(buf, OZ_BUF_SIZE, "%s/%s", dbpath, OZ_KEY_FILENAME);
	fpkey = fopen(buf, "r");
	if(!fpkey)
	{
		return 1;
	}

	/* count lines */
	handle->_nrecs = 0;
	while(1)
	{
		ret = fscanf(fpkey, "%s\t%llu\t%u\n", key, &offset, &length);
		/* only read 3 items is considered succ */
		if(ret==3)
		{
			handle->_nrecs++;
		}
		if(ret==EOF)
		{
			break;
		}
	}

	/* Read key length offet into memory */
	rewind(fpkey);
	handle->_recs = (OZRecord*) malloc(sizeof(OZRecord) * handle->_nrecs);
	cnt = 0;
	while (cnt < handle->_nrecs)
	{
		ret = fscanf(fpkey, "%s\t%llu\t%u\n", key, &offset, &length);
		if(ret==3)
		{
			/* Copy to table */
			handle->_recs[cnt]._offset = offset;
			handle->_recs[cnt]._length = length;
			handle->_recs[cnt]._key = (char*)malloc(sizeof(char)* (strlen(key) + 1));
			strcpy(handle->_recs[cnt]._key, key);
			cnt++;
		}
		else if(ret==EOF)
		{
			break;
		}
	}

	/* Close */
	fclose(fpkey);

	return 0;
}


int oztrav_open_vf(OZTrav* handle, const char* dbpath)
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

void oztrav_close(OZTrav* handle)
{
	if (handle)
	{   
		if (handle->_recs)
		{   
			long i;
			for(i=0; i< handle->_nrecs; i++)
			{
				free(handle->_recs[i]._key);
			}
			free(handle->_recs);
		}   
		handle->_nrecs = 0;
		if (handle->_fpval)
		{   
			fclose(handle->_fpval);
		}   
	}   
}
