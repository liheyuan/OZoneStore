#include "ozsort.h"
#include <stdio.h>
#include <string.h>

//Buffer
OZRecord split[OZSORT_PER_LINE];

int ozsort_work(OZSort* param)
{
    if(ozsort_split(param))
    {
        return 1;
    }

    ozsort_close(param);

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
    long cnt;
    long value;
    char key[OZ_KEY_MAX];
    char fn[OZ_BUF_SIZE];
    int i;
    int ret;

    fpsrc = fopen(param->_src, "r");
    if(!fpsrc)
    {
        return 1;
    }

    /* sort split one by one */
    param->_nfiles = 0;
    cnt = 0;

    /* print base */
    sprintf(fn, "/tmp/ozsort_%d_", time(NULL));

    while(1)
    {
        /* Read OZSORT_PER_LINE lines unless EOF */
        cnt = 0;
        for(i=0; i<OZSORT_PER_LINE; i++)
        {
            ret = fscanf(fpsrc, "%s\t%llu\t%u\n", key, &split[cnt]._offset, &split[cnt]._length);
            if(ret==3)
            {
                split[cnt]._key = malloc(sizeof(char) * (strlen(key) + 1));
                strcpy(split[cnt]._key ,key);
                cnt++;    
            }
            if(ret==EOF)
            {
                break;    
            }
        }

        //Make filename and open it
        if(param->_nfiles>=OZSORT_MAX_SPLITS)
        {
            return 3;    
        }
        sprintf(param->_files[param->_nfiles], "%s%d", fn, param->_nfiles);
        fp = fopen(param->_files[param->_nfiles], "w");
        if(!fp)
        {
            return 2;    
        }
        //Sort/output
        qsort(split, cnt, sizeof(OZRecord), cmp_oz_record);
        for(i=0; i<cnt; i++)
        {
            fprintf(fp, "%s\t%llu\t%u\n", split[i]._key, split[i]._offset, split[i]._length);
        }
        param->_nfiles++;
        fclose(fp);
        fp = NULL;

        if(ret==EOF)
        {
            break;    
        }

    }// while 1

    return 0;
}

void ozsort_close(OZSort* param)
{
    int i;
    for(i=0; i<param->_nfiles; i++)
    {
        unlink(param->_files[i]);
    }
    param->_nfiles = 0;
}
