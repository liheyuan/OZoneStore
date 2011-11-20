#include "ozsort.h"
#include <stdio.h>
#include <string.h>

//Buffer
OZRecord buffer[OZSORT_PER_LINE];

int ozsort_work(OZSort* param)
{
    if(ozsort_split(param))
    {
        return 1;
    }
	
	if(ozsort_merge(param))
	{
		return 2;
	}
		
    ozsort_clear(param);

    return 0;
}

int cmp_oz_record(const void* a, const void* b)
{
    OZRecord* x = (OZRecord*)a;
    OZRecord* y = (OZRecord*)b;

    return strcmp(x->_key, y->_key);
}

int ozsort_split(OZSort* param)
{
    /* Global Var */
    FILE* fpsrc = NULL;
    FILE* fp = NULL;
    char fn[OZ_BUF_SIZE];
    char key[OZ_KEY_MAX];
    int i;
    long cnt;
    int ret;
	
	/* open original file */
    fpsrc = fopen(param->_src, "r");
    if(!fpsrc)
    {
        return 1;
    }

    /* print base file name */
    sprintf(fn, "/tmp/ozsort_%d_", time(NULL));
    param->_nsplits = 0;
    cnt = 0;

    while(1)
    {
        /* Read OZSORT_PER_LINE lines unless EOF or reach the split count */
        for(i = 0, cnt = 0; i < OZSORT_PER_LINE; i++)
        {
            ret = fscanf(fpsrc, "%s\t%llu\t%u\n", key, &buffer[cnt]._offset, &buffer[cnt]._length);
			/* only read 3 items is considered succ */
            if(ret==3)
            {
                buffer[cnt]._key = malloc(sizeof(char) * (strlen(key) + 1));
                strcpy(buffer[cnt]._key ,key);
                cnt++;    
            }
            if(ret==EOF)
            {
                break;    
            }
        }

        /* Make new split filename and open it */
        if(param->_nsplits>=OZSORT_MAX_SPLITS)
        {
            return 3;    
        }
        sprintf(param->_splits[param->_nsplits], "%s%d", fn, param->_nsplits);
        fp = fopen(param->_splits[param->_nsplits], "w");
        if(!fp)
        {
            return 2;    
        }
        param->_nsplits++;

        /* Sort & output */
        qsort(buffer, cnt, sizeof(OZRecord), cmp_oz_record);
        for(i=0; i<cnt; i++)
        {
            fprintf(fp, "%s\t%llu\t%u\n", buffer[i]._key, buffer[i]._offset, buffer[i]._length);
        }
        fclose(fp);
        fp = NULL;
		
		/* finish all lines */
        if(ret==EOF)
        {
            break;    
        }

    }// while 1

    return 0;
}

void ozsort_clear(OZSort* param)
{
    int i;
    for(i=0; i<param->_nsplits; i++)
    {
        unlink(param->_splits[i]);
    }
    param->_nsplits = 0;
}

int ozsort_merge(OZSort* param)
{
	/* Var */
	FILE* fps[OZSORT_MAX_SPLITS];
	FILE* out;
	OZRecord datas[OZSORT_MAX_SPLITS];
	OZRecord prev;
	OZRecord* minr;
	int flags[OZSORT_MAX_SPLITS];
	char fn[OZ_BUF_SIZE];
	char key[OZ_KEY_MAX];
	int i;
	int minIdx;
	int ret;
	int hasMin;
	int last;

	/* open each split file -> fps */
	for(i=0; i<param->_nsplits; i++)
	{
		fps[i] = fopen(param->_splits[i], "rb");
		if(!fps[i])
		{
			/* fail open one, close previous fps */
			while(i>=0)
			{
				if(fps[i])
				{
					fclose(fps[i]);
				}
				i--;
			}
			return 1;
		}
	}

	/* make merge filename and open it */
	snprintf(fn, OZ_BUF_SIZE, "/tmp/ozsort_merge_%d", time(NULL));
	out = fopen(fn, "w");
	if(!out)
	{
		return 2;
	}

	/* first read, each split one record */
	for(i=0; i<param->_nsplits; i++)
	{
		if(fscanf(fps[i], "%s\t%llu\t%u\n", key, &datas[i]._offset, &datas[i]._length)!=EOF)
		{
			datas[i]._key = malloc(sizeof(char) * (strlen(key) + 1));
			strcpy(datas[i]._key ,key);
			flags[i] = 1;
		}
		else
		{
			flags[i] = 0;	
		}
	}
	
	/* do until all splits is EOF */
	while(1)
	{
		/* select min */
		sprintf(key, "~");
		hasMin = 0;
		for(i=0; i<param->_nsplits; i++)
		{
			if(flags[i] && strcmp(datas[i]._key, key)<0)
			{
				strcpy(key, datas[i]._key);	
				minIdx = i;
				hasMin = 1;
			}
		}
		/* all eof */
		if(!hasMin)
		{
			/* output last one */
			fprintf(out, "%s\t%llu\t%u\n", prev._key, prev._offset, prev._length);
			break;
		}
		
		/* filter duplicate keys but smaller offset */
		if(prev._key==NULL)
		{
			prev._key = (char*)malloc(sizeof(char) * OZ_KEY_MAX);
			strcpy(prev._key, datas[minIdx]._key);	
			prev._offset = datas[minIdx]._offset;
			prev._length = datas[minIdx]._length;
		}
		else if(strcmp(prev._key, datas[minIdx]._key) ==0 )
		{
			if(datas[minIdx]._offset >= prev._offset)
			{
				/* replace if same key but large offset */
				prev._offset = datas[minIdx]._offset;
				prev._length = datas[minIdx]._length;
			}
		}
		else
		{
			/* different key, output previous */
			fprintf(out, "%s\t%llu\t%u\n", prev._key, prev._offset, prev._length);
			strcpy(prev._key, datas[minIdx]._key);	
			prev._offset = datas[minIdx]._offset;
			prev._length = datas[minIdx]._length;
		}

		/* read next in channel */
		if(fscanf(fps[minIdx], "%s\t%llu\t%u\n", key, &datas[minIdx]._offset, &datas[minIdx]._length)!=EOF)
		{
			datas[minIdx]._key = malloc(sizeof(char) * (strlen(key) + 1));
			strcpy(datas[minIdx]._key ,key);
		}
		else
		{
			flags[minIdx] = 0;
		}
	}

	/* close all split file and merge file */
	free(prev._key);
	for(i=0; i<param->_nsplits; i++)
	{
		if(fps[i])
		{
			fclose(fps[i]);
		}
	}
	fclose(out);
}
