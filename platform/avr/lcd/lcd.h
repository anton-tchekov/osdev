/**
 * @file    asynclcd.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    12.04.2023
 * @brief   LCD-Treiber, der die Programmausfuehrung nicht
 *          blockiert. Stattdessen werden alle Zeichenbefehle
 *          zunaechst in einer Queue gespeichert und spaeter
 *          stueckweise von der Update-Funktion abgearbeitet.
 */

#ifndef __ASYNCLCD_H__
#define __ASYNCLCD_H__

#include <types.h>

/** Farbdefinitionen im RGB565 Format */
#define COLOR_RED      0xF800
#define COLOR_YELLOW   0xFFE0
#define COLOR_GREEN    0x07E0
#define COLOR_CYAN     0x07FF
#define COLOR_BLUE     0x001F
#define COLOR_MAGENTA  0xF81F

#define COLOR_WHITE    0xFFFF
#define COLOR_BLACK    0x0000

/** Breite und Hoehe des Displays */
#define DISPLAY_HEIGHT 320
#define DISPLAY_WIDTH  480

/**
 * @brief LCD Initialisieren
 */
void asynclcd_init(void);

/**
 * @brief Wandelt eine Farbe von RGB888 in RGB565 um
 *
 * @param r Rot-Wert von 0-255
 * @param g Gruen-Wert von 0-255
 * @param b Blau-Wert von 0-255
 * @retval Wert in RGB565, das LCD-Interne Format
 */
u16 asynclcd_color(u8 r, u8 g, u8 b);

/**
 * Zeichnet ein Rechteck auf das Display
 *
 * @param x X-Koordinate der oberen linken Ecke in Pixeln
 * @param y Y-Koordinate der oberen linken Ecke in Pixeln
 * @param w Breite des Rechtecks in Pixeln
 * @param w Hoehe des Rechtecks in Pixeln
 * @param color Farbe (RGB565)
 */
void asynclcd_rect(i32 x, i32 y, i32 w, i32 h, u16 color);

/**
 * @brief Zeichnet ein Zeichen auf das Display
 *
 * @param x X-Koordinate der oberen linken Ecke in Pixeln
 * @param y Y-Koordinate der oberen linken Ecke in Pixeln
 * @param fg Vordergrundfarbe (RGB565)
 * @param bg Hintergrundfarbe (RGB565)
 * @param c Ein ASCII Zeichen
 * @retval Die Breite des Zeichens in Pixeln
 */
i32 asynclcd_char(i32 x, i32 y, u16 fg, u16 bg, i32 c);

/**
 * @brief Zeichnet eine Zeichenkette auf das Display.
 *        Benutzt intern `asynclcd_char`
 *
 * @param x X-Koordinate der oberen linken Ecke in Pixeln
 * @param y Y-Koordinate der oberen linken Ecke in Pixeln
 * @param fg Vordergrundfarbe (RGB565)
 * @param bg Hintergrundfarbe (RGB565)
 * @param s Nullterminierte Zeichenkette
 */
void asynclcd_string(i32 x, i32 y, u16 fg, u16 bg, const char *s);

/**
 * @brief Arbeitet die gespeicherten Zeichenbefehle stueckweise ab.
 *        Sollte im Superloop stets aufgerufen werden.
 */
void asynclcd_update(void);

/**
 * @brief Gibt zurueck, ob alle Zeichenbefehle abgeschlossen sind
 *
 * @retval true, wenn mit dem Zeichnen fertig
 */
bool asynclcd_ready(void);

#endif /* __LCD_H__ */
