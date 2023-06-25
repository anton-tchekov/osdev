#include "statusbar.h"

void statusbar_render(Framebuffer *fb){
    framebuffer_rect(fb, 0, 0, fb->Width, fb->Height, nordPalette.background);
    font_string(fb, 10, font_noto->Size / 2, "Imaginary OS", font_noto, nordPalette.text);
}