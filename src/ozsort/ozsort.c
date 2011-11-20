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

    //ozsort_clear(param);

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

}
