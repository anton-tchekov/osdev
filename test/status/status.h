#ifndef __STATUS_H__
#define __STATUS_H__

typedef enum
{
	STATUS_OK,
	STATUS_FAIL
} Status;

/**
 * @brief Returns a human-readable description for a Status
 *
 * @param code Statuscode to get a explanation for
 * @return Human-friendly description of the status code
 */
const char *status_str(Status code);

#endif /* __STATUS_H__ */
