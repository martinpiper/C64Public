#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

// Unimplemented
template <typename T>
static T strtoTT(char const* _String, char** _EndPtr, int _Radix);

// Explicit specialisations
template <>
int strtoTT<int>(char const* _String, char** _EndPtr, int _Radix)
{
	return strtol(_String, _EndPtr, _Radix);
}

template <>
unsigned int strtoTT<unsigned int>(char const* _String, char** _EndPtr, int _Radix)
{
	return strtoul(_String, _EndPtr, _Radix);
}

template <typename T>
static T ParamToNumSimple(const char *arg)
{
	T num;

	if(arg[0]=='$')
	{
		num = strtoTT<T>(arg+1,NULL,16);
	}
	else if((arg[0]=='0')&&((arg[1]&0xdf)=='X'))
	{
		num = strtoTT<T>(arg+2,NULL,16);
	}
	else
	{
		num = strtoTT<T>(arg,NULL,10);
	}

	return num;
}

template <typename T>
static T ParamToNumEvaluate(const char* arg)
{
	// Quick simple case
	if (arg[0] != '=')
	{
		return ParamToNumSimple<T>(arg);
	}

	// Evaluate
	T num = 0;
	std::string working = arg;
	working.erase(0, 1);
	const std::string pattern = " \t|";
	while (!working.empty())
	{
		size_t pos = working.find_first_of(pattern);
		std::string ret = working.substr(0, pos);

		if (!ret.empty())
		{
			num |= ParamToNumSimple<T>(ret.c_str());
		}
		else
		{
			break;
		}

		working.erase(0, ret.length());
		pos = working.find_first_not_of(pattern);
		if (std::string::npos != pos)
		{
			working = working.substr(pos);
		}
	}

	return num;
}

int ParamToNum(const char* arg)
{
	return ParamToNumEvaluate<int>(arg);
}

unsigned int ParamToUNum(const char *arg)
{
	return ParamToNumEvaluate<unsigned int>(arg);
}
