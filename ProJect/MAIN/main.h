#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_spi.h"
#include "stdbool.h"

#include "delay.h"

#include "arm_math.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define abs(x) ((x)>0? (x):(-(x)))
//#define ToChar(x) #@x
//#define ToString(x) #x

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

#define Latitude_At_ShenZhen 22.57025f

#endif

