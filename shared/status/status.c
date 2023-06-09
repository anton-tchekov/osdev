/**
 * @file    status.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    25.05.2023
 */

#include <status.h>

const char *status_str(Status code)
{
	static const char *_status_strs[] =
	{
		"Ok",
		"Unknown error",
		"Timeout",
		"Out of bounds access",
		"No such file or directory",
		"Invalid path format",
		"No space left on device",
		"Not implemented",
	};

	return _status_strs[code];
}
