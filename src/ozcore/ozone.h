/*
 * ozone.h
 *
 *  Created on: 2011-11-11
 *      Author: liheyuan
 */

#ifndef OZONE_H_
#define OZONE_H_

/* Define for big File */
//#define _FILE_OFFSET_BITS 64

/* Public Define */
#define OZ_KEY_MAX 128
#define OZ_VALUE_MAX 8192
#define OZ_BUF_SIZE 2048
#define OZ_KEY_FILENAME "key.dat"
#define OZ_VALUE_FILENAME "value.dat"

/* Include headers*/
#include "ozread.h"
#include "ozwrite.h"

#endif /* OZONE_H_ */
