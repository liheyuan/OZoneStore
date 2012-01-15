/*
 * oztrav.h
 *
 *  Created on: 2012-01-15
 *      Author: liheyuan
 */

#ifndef OZTRAV_H_
#define OZTRAV_H_

/* Include headers*/
#include "ozone.h"
#include <stdint.h>
#include <stdio.h>

/* Public defines  */

/* Public Struct */

/*
 * Param structure need by oztrav_traverse_*
 * Need to be free by oztrav_traverse_free
 */
typedef struct OZTrav_Cursor
{
	/* Single key (Output)*/
	char* _key;

	/* length for _length  */
	uint32_t _key_len;

	/* alloc length for _length  */
	uint32_t _key_len_alloc;

	/* Value Buffer (Output) */
	char* _value;

	/* length of _value */
	uint32_t _value_len;

	/* alloc length for _value  */
	uint32_t _value_len_alloc;

} OZTrav_Cursor;

/*
 * Read record handle
 */
typedef struct OZTrav
{
	/* Records table in RAM for faster lookup */
	OZRecord* _recs;

	/* Number of _recs */
	long _nrecs;

	/* File handler for value file (mode:rb)*/
	FILE* _fpval;

} OZTrav;

/* Public Functions */

/*
 * Init resources needed by oztrav_* functions
 *
 * @param handle The handler used throughout oztarv
 * @param dbpath path that store the db
 *
 * @return 0:success, 1:other fail, 3:flock fail
 */
int oztrav_open(OZTrav* handle, const char* dbpath);

/*
 * Open value file
 *
 * @param handle The handler used throughout OZTrav
 * @param dbpath path that store the db
 *
 * @return 0:success, 1:value file open error, 2:can't get sorted/uniqed lines
 */
int oztrav_open_vf(OZTrav* handle, const char* dbpath);

/*
 * Open key file, store key && length into memory
 *
 * @param handle The handler used throughout oztrav
 * @param dbpath path that store the db
 *
 * @return 0:success, 1:key file open error, 2:
 */
int oztrav_open_kf(OZTrav* handle, const char* dbpath);

/*
 * Release resources used by handle
 *
 * @param handle The handler used throughout OZTrav
 *
 */
void oztrav_close(OZTrav* handle);

#endif /* OZONEREAD_H_ */
