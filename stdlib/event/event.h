/**
 * @file    event.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    29.04.2023
 * @brief   Event handling functions
 */

#ifndef __EVENT_H__
#define __EVENT_H__

#include <types.h>
#include <event-types.h>

/**
 * @brief Finish an event handler. Do NOT call this function.
 */
void finish(void);

/**
 * @brief Setup function that is called at program start.
 *        Must be implemented by user.
 */
void setup(void);

/**
 * @brief Loop function that is called repeatedly
 *        Must be implemented by user.
 */
void loop(void);

/**
 * @brief Register an event. Do NOT call this function.
 *
 * @param type Event Type, see enum above
 * @param event Event handler function address
 */
void event_register(Event type, u32 event);

#endif /* __EVENT_H__ */
