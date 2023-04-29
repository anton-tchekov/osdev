/**
 * @file    colors.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   HTML Color List in ABGR format
 */

#ifndef __COLORS_H__
#define __COLORS_H__

//! @cond Doxygen_Suppress

#define COLOR_TRANSPARENT                            0x00000000

/* RED */
#define COLOR_INDIAN_RED                             0xCD5C5CFF
#define COLOR_LIGHT_CORAL                            0xF08080FF
#define COLOR_SALMON                                 0xFA8072FF
#define COLOR_DARK_SALMON                            0xE9967AFF
#define COLOR_LIGHT_SALMON                           0xFFA07AFF
#define COLOR_CRIMSON                                0xDC143CFF
#define COLOR_RED                                    0xFF0000FF
#define COLOR_FIRE_BRICK                             0xB22222FF
#define COLOR_DARK_RED                               0x8B0000FF

/* PINK */
#define COLOR_PINK                                   0xFFC0CBFF
#define COLOR_LIGHT_PINK                             0xFFB6C1FF
#define COLOR_HOT_PINK                               0xFF69B4FF
#define COLOR_DEEP_PINK                              0xFF1493FF
#define COLOR_MEDIUM_VIOLET_RED                      0xC71585FF
#define COLOR_PALE_VIOLET_RED                        0xDB7093FF

/* ORANGE */
#define COLOR_LIGHT_SALMON                           0xFFA07AFF
#define COLOR_CORAL                                  0xFF7F50FF
#define COLOR_TOMATO                                 0xFF6347FF
#define COLOR_ORANGE_RED                             0xFF4500FF
#define COLOR_DARK_ORANGE                            0xFF8C00FF
#define COLOR_ORANGE                                 0xFFA500FF

/* YELLOW */
#define COLOR_GOLD                                   0xFFD700FF
#define COLOR_YELLOW                                 0xFFFF00FF
#define COLOR_LIGHT_YELLOW                           0xFFFFE0FF
#define COLOR_LEMON_CHIFFON                          0xFFFACDFF
#define COLOR_LIGHT_GOLDENROD_YELLOW                 0xFAFAD2FF
#define COLOR_PAPAYA_WHIP                            0xFFEFD5FF
#define COLOR_MOCCASIN                               0xFFE4B5FF
#define COLOR_PEACH_PUFF                             0xFFDAB9FF
#define COLOR_PALE_GOLDENROD                         0xEEE8AAFF
#define COLOR_KHAKI                                  0xF0E68CFF
#define COLOR_DARK_KHAKI                             0xBDB76BFF

/* PURPLE */
#define COLOR_LAVENDER                               0xE6E6FAFF
#define COLOR_THISTLE                                0xD8BFD8FF
#define COLOR_PLUM                                   0xDDA0DDFF
#define COLOR_VIOLET                                 0xEE82EEFF
#define COLOR_ORCHID                                 0xDA70D6FF
#define COLOR_FUCHSIA                                0xFF00FFFF
#define COLOR_MAGENTA                                0xFF00FFFF
#define COLOR_MEDIUM_ORCHID                          0xBA55D3FF
#define COLOR_MEDIUM_PURPLE                          0x9370DBFF
#define COLOR_REBECCA_PURPLE                         0x663399FF
#define COLOR_BLUE_VIOLET                            0x8A2BE2FF
#define COLOR_DARK_VIOLET                            0x9400D3FF
#define COLOR_DARK_ORCHID                            0x9932CCFF
#define COLOR_DARK_MAGENTA                           0x8B008BFF
#define COLOR_PURPLE                                 0x800080FF
#define COLOR_INDIGO                                 0x4B0082FF
#define COLOR_SLATE_BLUE                             0x6A5ACDFF
#define COLOR_DARK_SLATE_BLUE                        0x483D8BFF
#define COLOR_MEDIUM_SLATE_BLUE                      0x7B68EEFF

/* GREEN */
#define COLOR_GREEN_YELLOW                           0xADFF2FFF
#define COLOR_CHARTREUSE                             0x7FFF00FF
#define COLOR_LAWN_GREEN                             0x7CFC00FF
#define COLOR_LIME                                   0x00FF00FF
#define COLOR_LIME_GREEN                             0x32CD32FF
#define COLOR_PALE_GREEN                             0x98FB98FF
#define COLOR_LIGHT_GREEN                            0x90EE90FF
#define COLOR_MEDIUM_SPRING_GREEN                    0x00FA9AFF
#define COLOR_SPRING_GREEN                           0x00FF7FFF
#define COLOR_MEDIUM_SEA_GREEN                       0x3CB371FF
#define COLOR_SEA_GREEN                              0x2E8B57FF
#define COLOR_FOREST_GREEN                           0x228B22FF
#define COLOR_GREEN                                  0x008000FF
#define COLOR_DARK_GREEN                             0x006400FF
#define COLOR_YELLOW_GREEN                           0x9ACD32FF
#define COLOR_OLIVE_DRAB                             0x6B8E23FF
#define COLOR_OLIVE                                  0x808000FF
#define COLOR_DARK_OLIVE_GREEN                       0x556B2FFF
#define COLOR_MEDIUM_AQUA_MARINE                     0x66CDAAFF
#define COLOR_DARK_SEA_GREEN                         0x8FBC8BFF
#define COLOR_LIGHT_SEA_GREEN                        0x20B2AAFF
#define COLOR_DARK_CYAN                              0x008B8BFF
#define COLOR_TEAL                                   0x008080FF

/* BLUE */
#define COLOR_AQUA                                   0x00FFFFFF
#define COLOR_CYAN                                   0x00FFFFFF
#define COLOR_LIGHT_CYAN                             0xE0FFFFFF
#define COLOR_PALE_TURQUOISE                         0xAFEEEEFF
#define COLOR_AQUA_MARINE                            0x7FFFD4FF
#define COLOR_TURQUOISE                              0x40E0D0FF
#define COLOR_MEDIUM_TURQUOISE                       0x48D1CCFF
#define COLOR_DARK_TURQUOISE                         0x00CED1FF
#define COLOR_CADET_BLUE                             0x5F9EA0FF
#define COLOR_STEEL_BLUE                             0x4682B4FF
#define COLOR_LIGHT_STEEL_BLUE                       0xB0C4DEFF
#define COLOR_POWDER_BLUE                            0xB0E0E6FF
#define COLOR_LIGHT_BLUE                             0xADD8E6FF
#define COLOR_SKY_BLUE                               0x87CEEBFF
#define COLOR_LIGHT_SKY_BLUE                         0x87CEFAFF
#define COLOR_DEEP_SKY_BLUE                          0x00BFFFFF
#define COLOR_DODGER_BLUE                            0x1E90FFFF
#define COLOR_CORNFLOWER_BLUE                        0x6495EDFF
#define COLOR_MEDIUM_SLATE_BLUE                      0x7B68EEFF
#define COLOR_ROYAL_BLUE                             0x4169E1FF
#define COLOR_BLUE                                   0x0000FFFF
#define COLOR_MEDIUM_BLUE                            0x0000CDFF
#define COLOR_DARK_BLUE                              0x00008BFF
#define COLOR_NAVY                                   0x000080FF
#define COLOR_MIDNIGHT_BLUE                          0x191970FF

/* BROWN */
#define COLOR_CORNSILK                               0xFFF8DCFF
#define COLOR_BLANCHED_ALMOND                        0xFFEBCDFF
#define COLOR_BISQUE                                 0xFFE4C4FF
#define COLOR_NAVAJO_WHITE                           0xFFDEADFF
#define COLOR_WHEAT                                  0xF5DEB3FF
#define COLOR_BURLY_WOOD                             0xDEB887FF
#define COLOR_TAN                                    0xD2B48CFF
#define COLOR_ROSY_BROWN                             0xBC8F8FFF
#define COLOR_SANDY_BROWN                            0xF4A460FF
#define COLOR_GOLDENROD                              0xDAA520FF
#define COLOR_DARK_GOLDENROD                         0xB8860BFF
#define COLOR_PERU                                   0xCD853FFF
#define COLOR_CHOCOLATE                              0xD2691EFF
#define COLOR_SADDLE_BROWN                           0x8B4513FF
#define COLOR_SIENNA                                 0xA0522DFF
#define COLOR_BROWN                                  0xA52A2AFF
#define COLOR_MAROON                                 0x800000FF

/* WHITE */
#define COLOR_WHITE                                  0xFFFFFFFF
#define COLOR_SNOW                                   0xFFFAFAFF
#define COLOR_HONEY_DEW                              0xF0FFF0FF
#define COLOR_MINT_CREAM                             0xF5FFFAFF
#define COLOR_AZURE                                  0xF0FFFFFF
#define COLOR_ALICE_BLUE                             0xF0F8FFFF
#define COLOR_GHOST_WHITE                            0xF8F8FFFF
#define COLOR_WHITE_SMOKE                            0xF5F5F5FF
#define COLOR_SEA_SHELL                              0xFFF5EEFF
#define COLOR_BEIGE                                  0xF5F5DCFF
#define COLOR_OLD_LACE                               0xFDF5E6FF
#define COLOR_FLORAL_WHITE                           0xFFFAF0FF
#define COLOR_IVORY                                  0xFFFFF0FF
#define COLOR_ANTIQUE_WHITE                          0xFAEBD7FF
#define COLOR_LINEN                                  0xFAF0E6FF
#define COLOR_LAVENDER_BLUSH                         0xFFF0F5FF
#define COLOR_MISTY_ROSE                             0xFFE4E1FF

/* GRAY */
#define COLOR_GAINSBORO                              0xDCDCDCFF
#define COLOR_LIGHT_GRAY                             0xD3D3D3FF
#define COLOR_SILVER                                 0xC0C0C0FF
#define COLOR_DARK_GRAY                              0xA9A9A9FF
#define COLOR_GRAY                                   0x808080FF
#define COLOR_DIM_GRAY                               0x696969FF
#define COLOR_LIGHT_SLATE_GRAY                       0x778899FF
#define COLOR_SLATE_GRAY                             0x708090FF
#define COLOR_DARK_SLATE_GRAY                        0x2F4F4FFF
#define COLOR_BLACK                                  0x000000FF

//! @endcond

#endif /* __COLORS_H__ */
