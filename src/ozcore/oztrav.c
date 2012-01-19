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

	/* Init handle */
	handle->_recs = NULL;
	handle->_nrecs = 0;
	handle->_fpval = NULL;

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
	if(!handle->_nrecs)
	{
		/* 0 key in key file */
		fclose(fpkey);
		return 2;
	}

	/* Read key length offset into memory */
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

void oztrav_cursor_init(OZTrav_Cursor* cursor)
{
	/* Init key */
	cursor->_key = NULL;
	cursor->_key_len = cursor->_key_len_alloc = 0;

	/* Init value */
	cursor->_value = NULL;
	cursor->_value_len = cursor->_value_len_alloc = 0;

	/* Init cursor */
	cursor->_cur = 0;
}

void oztrav_cursor_free(OZTrav_Cursor* cursor)
{
	/* Free key */
	if(cursor->_key)
	{
		free(cursor->_key);
		cursor->_key = NULL;
	}
	cursor->_key_len = cursor->_key_len_alloc = 0;

	/* Free value */
	if(cursor->_value)
	{
		free(cursor->_value);
		cursor->_value = NULL;
	}
	cursor->_value_len = cursor->_value_len_alloc = 0;

	/* Cursor */
	cursor->_cur = 0;
}

int oztrav_next(OZTrav* handle, OZTrav_Cursor* cursor)
{
	if(cursor->_cur>=handle->_nrecs)
	{
		/* Reach end */
		return 1;
	}
	else
	{
		/* Locate OZRecord*/
		OZRecord* prec = &(handle->_recs[cursor->_cur]);
		int ret;
		size_t len;

		/* value:fseeko */
		if (fseeko(handle->_fpval, prec->_offset, SEEK_SET))
		{   
			return 2;
		}   

		/* value:check buffer */
		if( cursor->_value_len_alloc <= prec->_length )
		{
			cursor->_value = (char*)realloc(cursor->_value, prec->_length+1);
			if(!cursor->_value)
			{
				return 3;
			}
			printf("alloc_value: %ld\n", prec->_length+1);
			cursor->_value_len_alloc = prec->_length+1;
		}

		/* value:fread */
		if ((ret = fread(cursor->_value, prec->_length, 1, handle->_fpval)) != 1)
		{   
			return 2;
		}   
		cursor->_value[prec->_length] = '\0';
		cursor->_value_len = prec->_length; 

		/* key:check buffer */
		len = strlen(prec->_key);
		if( cursor->_key_len_alloc <= len )
		{
			cursor->_key = (char*)realloc(cursor->_key, len+1);
			if(!cursor->_key)
			{
				return 3;
			}
			printf("alloc_key: %ld\n", len+1);
			cursor->_key_len_alloc = len+1;
		}
		cursor->_key_len = len;

		/* key:copy */
		strcpy(cursor->_key, prec->_key);

		/* Inc */
		cursor->_cur++;

		return 0;
	}// end if not end
}
