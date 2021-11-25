// HString.cpp
//
// By NightHz
//

#include "HString.h"
#include <string.h>


int Rehenz::stringSearch(const char *strPar, const char *strChi)
{
	int i, j;
	for (i = 0; strPar[i]; i++)
	{
		for (j = 0; strChi[j]; j++)
		{
			if (strPar[i + j] == strChi[j])
				continue;
			else
				break;
		}
		if (strChi[j])
			continue;
		else
			return i;
	}
	return -1;
}

int Rehenz::stringSearchKMP(const char * strPar, const char * strChi)
{
	// Get next[]
	int i, j;
	int * next = new int[strlen(strChi)];
	next[0] = -1;
	for (i = 1, j = 0; strChi[i]; i++)
	{
		
		if (strChi[i] == strChi[j])
		{
			next[i] = next[j];
			j++;
			continue;
		}
		else
		{
			next[i] = j;
			while (true)
			{
				j = next[j];
				if (j == -1)
				{
					j = 0;
					break;
				}
				else if (strChi[i] == strChi[j])
				{
					j++;
					break;
				}
			}
			continue;
		}
	}
	// String Search
	for (i = 0, j = 0; strPar[i] && strChi[j];)
	{
		if (strPar[i] == strChi[j])
		{
			i++;
			j++;
			continue;
		}
		else if (next[j] == -1)
		{
			i++;
			j = 0;
			continue;
		}
		else
		{
			j = next[j];
			continue;
		}
	}
	delete next;
	if (!strChi[j])
		return i - j;
	return -1;
}

