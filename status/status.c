#include <status.h>

static const char *_status_strs[] =
{
	"Ok",
	"Unknown error",
	"Timeout"
};

const char *status_str(Status code)
{
	return _status_strs[code];
}
