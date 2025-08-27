#ifndef SM_MY_DEBUG_H
#define SM_MY_DEBUG_H


//#include "sys_usart.h"
#include <stdio.h>
#include "system.h"
#include <string.h>

void PrintClockInfo(void);
void RegularPrint(char* str, int n,double running_time);
void JsonPrint(char* str, int n,double running_time);

#endif
