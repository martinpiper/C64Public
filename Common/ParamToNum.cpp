#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ParamToNum(const char *arg)
{
	int num;

	if(arg[0]=='$')
	{
		num = strtol(arg+1,NULL,16);
	}
	else if((arg[0]=='0')&&((arg[1]&0xdf)=='X'))
	{
		num = strtol(arg+2,NULL,16);
	}
	else
	{
		num = strtol(arg,NULL,10);
	}

	return num;
}
