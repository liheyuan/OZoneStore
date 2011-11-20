#ifndef OZ_SORT_H
#define OZ_SORT_H

/* Public include */
#include "../ozcore/ozread.h"

/* Public defines  */
#define OZSORT_PER_LINE 10240
#define OZSORT_MAX_SPLITS 100

/* Public struct */

/* 
 * Internal structure for manage external file
 *
 */
typedef struct
{
    /* source file */
    char _src[OZ_BUF_SIZE];

    /* external file names */
    char _splits[OZ_BUF_SIZE][OZSORT_MAX_SPLITS];

    /* number of actual used fps */
    int _nsplits;

}OZSort;

/*
 * ozsort main entry
 * 
 * @param param a bundle of param using throughout ozsort
 * @return 0:succ, 1:split fail, 2:sort fail
 */
int ozsort_work(OZSort* param);

/* 
 * split src_file into many temp files, write files handle into 
 * every OZSORT_MAX_SPLITS lines
 * 
 * @param param a bundle of param using throughout ozsort
 * @return 0:succ, 1:src file open fail, 2:tmp file create fail, 3:more split than OZSORT_MAX_SPLITS
 */
int ozsort_split(OZSort* param);

/*
 * merge splits file into one, and filter the replicated ones
 * 
 * @param param a bundle of param using throughout ozsort
 * @return 0:succ, 1:
 */
int ozsort_merge(OZSort* param);

/*
 * remove all temp files during split
 * 
 * @param a bundle of param using throughout ozsort
 * @return void, always succ
 */
void ozsort_clear(OZSort* param);

#endif
