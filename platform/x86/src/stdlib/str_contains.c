#include "str_contains.h"
#include "ctype.h"
#include "strlen.h"

bool str_contains(char *s, char *t)
{
	bool match;
	u32 i, j, string_length, search_term_length;

	string_length = strlen(s);
	search_term_length = strlen(t);
	for(i = 0; i <= string_length - search_term_length; ++i)
	{
		match = true;
		for(j = 0; j < search_term_length; ++j)
		{
			if(tolower(s[i + j]) != tolower(t[j]))
			{
				match = false;
				break;
			}
		}

		if(match)
		{
			return true;
		}
	}

	return false;
}
