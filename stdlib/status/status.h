/**
 * @file    status.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   TODO
 */

#ifndef __STATUS_H__
#define __STATUS_H__

/** Status code definitions */
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
