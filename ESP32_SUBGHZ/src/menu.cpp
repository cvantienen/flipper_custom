#include <U8g2lib.h>
#include <menu.h>
#include <icon.h>


extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 384)
const unsigned char *bitmap_icons[8] = {
    bitmap_icon_3dcube,
    bitmap_icon_battery,
    bitmap_icon_dashboard,
    bitmap_icon_fireworks,
    bitmap_icon_gps_speed,
    bitmap_icon_knob_over_oled,
    bitmap_icon_parksensor,
    bitmap_icon_turbo};
// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 8320)
const unsigned char *bitmap_screenshots[8] = {
    bitmap_screenshot_3dcube,
    bitmap_screenshot_battery,
    bitmap_screenshot_gauges,
    bitmap_screenshot_fireworks,
    bitmap_screenshot_gps_speed,
    bitmap_screenshot_knob_over_oled,
    bitmap_screenshot_parksensor,
    bitmap_screenshot_turbo};
// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 8320)
const unsigned char *bitmap_qr_codes[8] = {
    bitmap_qr_3dcube,
    bitmap_qr_battery,
    bitmap_qr_gauges,
    bitmap_qr_fireworks,
    bitmap_qr_gps_speed,
    bitmap_qr_knob_over_oled,
    bitmap_qr_parksensor,
    bitmap_qr_turbo};

void menuSetup(){
    // Initialize display for the menu
    u8g2.setColorIndex(1);
    u8g2.begin();
    u8g2.setBitmapMode(1);

    // Define pins for menu buttons
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);

    u8g2.clearBuffer();
}

