/**
 * @file    event-types.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    29.05.2023
 * @brief   Event types
 */

#ifndef __EVENT_TYPES_H__
#define __EVENT_TYPES_H__

/** Event Type */
typedef enum
{
	/** Main loop event */
	EVENT_LOOP,

	/** Keyboard event */
	EVENT_KEY,

	/** Number of events */
	EVENT_COUNT
} Event;

#endif /* __EVENT_TYPES_H__ */
