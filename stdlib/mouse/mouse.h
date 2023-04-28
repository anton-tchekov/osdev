/**
 * @file    mouse.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    27.04.2023
 * @brief   Mouse input using event callbacks or polling
 */

#ifndef __MOUSE_H__
#define __MOUSE_H__

/* TODO: Scroll, check state directly */

/**
 * @brief Register an OnMouseButton event
 *
 * @param event Function pointer to a function that takes
 *              the mouse button and its state
 */
void mouse_register_event_button(void (*event)(MouseButton b, bool up));

/**
 * @brief Register an OnMouseMove event
 *
 * @param event Function pointer to a function that takes
 *              the mouse position
 */
void mouse_register_event_move(void (*event)(i32 x, i32 y));

#endif /* __MOUSE_H__ */
