#ifndef OZ_SORT_H
#define OZ_SORT_H

/* Public include */
#include "../ozcore/ozread.h"

/* Public defines  */
#define OZSORT_PER_LINE 204800
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

	/* merge file */
	char _merge[OZ_BUF_SIZE];

}OZSort;

/*
 * ozsort main entry
 * 
 * @param param a bundle of param using throughout ozsort
 * @return 0:succ, 1:split&sort fail, 2:merge&sort fail
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
 * @return 0:succ, 1:open split file fail, 2:open out file fail
 */
int ozsort_merge(OZSort* param);

#endif
