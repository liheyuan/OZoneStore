/*
 * ozone.h
 *
 *  Created on: 2011-11-11
 *      Author: liheyuan
 */

#ifndef OZONE_H_
#define OZONE_H_

#include <stdio.h>
#include <stdint.h>

/* Public Define */
#define OZ_KEY_BUF_SIZE 128
#define OZ_BUF_SIZE 1024
#define OZ_KEY_FILENAME "key.dat"
#define OZ_VALUE_FILENAME "value.dat"
#define OZ_FLOCK_FILENAME "flock"

/*
 * Record In RAM for faster lookup
 * key -> offset & length
 */
typedef struct OZRecord
{
	/* key string must \0 terminated */
	char* _key;

	/* offset in value file */
	uint64_t _offset;

	/* length start from _offset */
	uint32_t _length;

} OZRecord;

/* Include headers*/
#include "ozread.h"
#include "ozwrite.h"
#include "oztrav.h"


#endif /* OZONE_H_ */
