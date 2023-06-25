/**
 * @file    status.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Status code definitions and error handling
 */

#ifndef __STATUS_H__
#define __STATUS_H__

/** Status code definitions */
typedef enum
{
	STATUS_OK,
	STATUS_FAIL,
	STATUS_TIMEOUT,
	STATUS_OUT_OF_BOUNDS,
	STATUS_NO_SUCH_FILE_OR_DIR,
	STATUS_PATH_FORMAT_INVALID,
	STATUS_NO_SPACE_LEFT_ON_DEVICE,
	STATUS_NOT_IMPLEMENTED,
} Status;

/**
 * @brief Propagate error codes to caller
 *
 * @param STATUS If error (non-zero), return value
 */
#define RETURN_IF(STATUS) \
	do \
	{ \
		Status status = STATUS; \
		if(status) \
		{ \
			return status; \
		} \
	} \
	while(0);

/**
 * @brief Returns a human-readable description for a Status
 *
 * @param code Statuscode to get a explanation for
 * @return Human-friendly description of the status code
 */
const char *status_str(Status code);

#endif /* __STATUS_H__ */
