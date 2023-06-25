#include "statusbar.h"

void statusbar_render(Framebuffer *fb){
    framebuffer_rect(fb, 0, 0, fb->Width, fb->Height, nordPalette.background);
}