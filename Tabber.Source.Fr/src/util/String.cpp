/*
 * Tabber - Guitar Tablature Editor, freeware and open-source
 * Copyright (C) 2004  Yvan Razafindramanana
 *
 * Additionnal Contributors:
 * <place your name here :)>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "String.h"

char String::_conversionBuffer[20] = "";


/**
 * Conversion from integer to string
 * @return a pointer to internal structure containing the result of the conversion. Client should not rely on permanent availability of this structure and should rather copy result in its own structure.
 */
const char* String::toString(int value)
{
	wsprintf(_conversionBuffer, "%d", value);
	return (const char*)_conversionBuffer;
}


/**
 * Case insensitive strstr
 */
char* String::stristr(const char* str1, const char* str2)
{
	char *s1 = const_cast<char*>(str1), *s2 = const_cast<char*>(str2);
	char *p, *ret;
	int len1 = 0, len2 = 0, c1, c2, matches = 0;

	while(s1[len1]) len1++;
	while(s2[len2]) len2++;
	if(len2 > len1) return 0;
	p = ret = s1;
	while(*p)
	{
		if((c1 = toupper((unsigned char)*p++))
			== (c2 = toupper((unsigned char)s2[matches++])))
		{
			if(matches == len2) return ret;
			continue;
		}
		ret = p;
		if(matches = (c1 == toupper((unsigned char)*s2)) ? 1 : 0) ret--;
	}
	if(s2[matches]) return 0;
	return ret;
}


