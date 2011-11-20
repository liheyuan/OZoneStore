/*
 * ozoneread.h
 *
 *  Created on: 2011-11-11
 *      Author: liheyuan
 */

#ifndef OZONEREAD_H_
#define OZONEREAD_H_

/* Include headers*/
#include "ozone.h"
#include <stdint.h>
#include <stdio.h>

/* Public defines  */
#define OZREAD_GETS_MAX 512
//#define OZREAD_MALLOC_STEP 1024
#define OZREAD_TMP_PATTERN "OZONE_READ_XXXXXX"

/* Public Struct */

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

/*
 * Record snippet for oz_gets sort
 * No key will be stored
 */
typedef struct OZRead_Cookie
{
	/* offset in value file */
	uint64_t _offset;

	/* length start from _offset */
	uint32_t _length;

	/* index in bansui array */
	long _index;

} OZRead_Cookie;

/*
 *
 */
typedef struct OZRead_Get
{
	/* Multiple keys*/
	char* _key;

	/* Value Buffer */
	char _value[OZ_VALUE_MAX];

} OZRead_Get;

/*
 *
 */
typedef struct OZRead_Gets
{
	/* Multiple keys*/
	char _keys[OZREAD_GETS_MAX][OZ_KEY_MAX];

	/* How many keys in _keys */
	long _nkeys;

	/* Values Buffer */
	char _values[OZREAD_GETS_MAX][OZ_VALUE_MAX];

	/* Temporary buffer for sort  */
	OZRead_Cookie _cookies[OZREAD_GETS_MAX];

	/* Actual usage of _cookies */
	long _ncookies;

} OZRead_Gets;

/*
 * Read record handle
 */
typedef struct OZRead
{
	/* Records table in RAM for faster lookup */
	OZRecord* _recs;

	/* Number of _recs */
	long _nrecs;

	/* File handler for value file (mode:rb)*/
	FILE* _fpval;

} OZRead;

/* Public Functions */

/*
 * Init resources needed by ozread_* functions
 *
 * @param handle The handler used throughout ozread
 * @param dbpath path that store the db
 *
 * @return 0:success,
 */
int ozread_open(OZRead* handle, const char* dbpath);

/*
 * Open value file
 *
 * @param handle The handler used throughout ozread
 * @param dbpath path that store the db
 *
 * @return 0:success, 1:value file open error, 2:can't get sorted/uniqed lines
 */
int ozread_open_vf(OZRead* handle, const char* dbpath);

/*
 * Sort/uniq key file, load it into memory
 *
 * @param handle The handler used throughout ozread
 * @param dbpath path that store the db
 *
 * @return 0:success, 1:shell sort/unique pipe error, 2:
 */
int ozread_open_kf(OZRead* handle, const char* dbpath);

/*
 * Get value according to a single key
 *
 * @param handle The handler used throughout ozread
 * @param param User input key, value bufeer etc.
 *
 * @return 0:success, 1:invalid handle, 2:invalid param, 3:key not found, 4:get value fail
 */
int ozread_get(OZRead* handle, OZRead_Get* param);

/*
 * Use Binary Search to location key OZRecord Structure in RAM table
 *
 * @param handle The handler used throughout ozread
 * @param rec The found rec result
 *
 * @return 0:succ found, 1:not found
 */
int ozread_get_key(OZRead* handle, const char* key, OZRecord** rec);

/*
 * Random access in value file
 *
 * @param handle The handler used throughout ozread
 * @param rec The user request key's OZRecord Structure in RAM
 * @param buf The buffer for place value
 * @param buf_len The length of buf
 *
 * @return 0:succ get, 1:buf not enough, 2:fseek fail, 3:fread fail
 */
int ozread_get_value(OZRead* handle, const OZRecord* rec, char* buf,
		long buf_len);

/*
 * Get values according to keys
 *
 * @param handle The handler used throughout ozread
 * @param param User input keys, values bufeer, cookies buffer etc.
 *
 * @return 0:success, 1:invalid handle, 2:invalid param, 3:
 */
int ozread_gets(OZRead* handle, OZRead_Gets* param);

/*
 * Search all keys, sort them in _offset asc order and store in param->_cookies
 *
 * @param handle The handler used throughout ozread
 * @param param User input keys, values bufeer, cookies buffer etc.
 *
 * @return 0:Always
 */
int ozread_gets_keys(OZRead* handle, OZRead_Gets* param);

/*
 * Get all values, using cookie to 'inc' fseeko and get each value
 *
 * @param handle The handler used throughout ozread
 * @param param User input keys, values bufeer, cookies buffer etc.
 *
 */
int ozread_gets_values(OZRead* handle, OZRead_Gets* param);

/*
 * Compare function for ozread_gets_keys
 *
 * @param a Cookie a
 * @param b Cookie b
 *
 * @return 1, -1, 0
 */
int cmp_ozread_gets_keys(const void* a, const void* b);

/*
 * Release resources used by handle
 *
 * @param handle The handler used throughout ozread
 *
 */
void ozread_close(OZRead* handle);

#endif /* OZONEREAD_H_ */
