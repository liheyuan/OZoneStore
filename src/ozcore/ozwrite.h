/*
 * ozwrite.h
 *
 *  Created on: 2011-11-14
 *      Author: liheyuan
 */

#ifndef OZWRITE_H_
#define OZWRITE_H_

#include "ozone.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Public Struct */

/*
 * Write Records handle
 */
typedef struct OZWrite
{
	/* File handler for key file mode(a) */
	FILE* _fpkey;

	/* File handler for value file mode(ab) */
	FILE* _fpval;

} OZWrite;

/*
 * Init resources needed by ozwrite_* functions
 *
 * @param handle The handler used throughout ozwrite
 * @param dbpath The path for db
 *
 * @return 0:succ, 1: invalid handle, 2:open key file fail, 3:open value file fail, 4:mutex_lock init fail
 */
int ozwrite_open(OZWrite* handle, const char* dbpath);

/*
 * Put key value pair into db
 *
 * @param handle The handler used throughout ozwrite
 * @param key The key
 * @param value The value
 *
 * @return 0:succ, 1:invalid handle, 2:write key fail ,3:write value fail, 
 *                 4:ftello fail, 5:value length is zero, 6:key length is zero
 *                 7:lock fail, 8:unlock fail
 * 6:value
 */
int ozwrite_put(OZWrite* handle, const char* key, const char* value);

/*
 * Release resources used by handle
 *
 * @param handle Handler used throughout ozwrite
 *
 */
void ozwrite_close(OZWrite* handle);
#endif /* OZWRITE_H_ */
