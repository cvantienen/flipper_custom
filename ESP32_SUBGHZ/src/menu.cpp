#include <U8g2lib.h>
#include <menu.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void menuSetup(){
    // Initialize display for the menu
    u8g2.setColorIndex(1);
    u8g2.begin();
    u8g2.setBitmapMode(1);
    u8g2.clearBuffer();
}

