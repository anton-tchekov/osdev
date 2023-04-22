#include <status.h>

static const char *_status_strs[] =
{
    "OK",
    "unknown error",
};

const char* status_str(Status code) {
    return _status_strs[code];
}
